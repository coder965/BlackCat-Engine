//  [8/24/2013 MRB]

#include "Core/CorePCH.h"
#include "CorePlatform/Memory/bcMemAlloc.h"
#include "Core/Memory/bcMemoryStack.h"

namespace black_cat
{
	namespace core
	{
#ifdef BC_MEMORY_ENABLE

		bc_memory_stack::bc_memory_stack() noexcept(true)
		{
		}

		bc_memory_stack::bc_memory_stack(bc_memory_stack::this_type&& p_other) noexcept(true) : bc_memory(std::move(p_other))
		{
			_move(std::move(p_other));
		}

		bc_memory_stack::~bc_memory_stack() noexcept(true)
		{
			if (m_initialized)
			{
				destroy();
			}
		}

		bc_memory_stack::this_type& bc_memory_stack::operator =(bc_memory_stack::this_type&& p_other) noexcept(true)
		{
			bc_memory::operator=(std::move(p_other));
			_move(std::move(p_other));

			return *this;
		}

		void bc_memory_stack::_initialize(bcUINT32 p_size, const bcCHAR* p_tag)
		{
			// We alloc mHeap by min align defined in CorePCH because we want all of our allocations have MIN_Align /
			m_heap = static_cast<bcUBYTE*>(core_platform::bc_mem_aligned_alloc(p_size * sizeof(bcUBYTE), BC_MEMORY_MIN_ALIGN));

			if (!m_heap)
			{
				throw std::bad_alloc();
			}

			m_size = p_size;
			m_top.store(m_heap, core_platform::bc_memory_order::relaxed);
			m_pop_thread_count.store(0, core_platform::bc_memory_order::relaxed);
			m_free_block_count.store(0, core_platform::bc_memory_order::relaxed);

			m_tracer.initialize(m_size);

			if (p_tag)
			{
				bc_memory::tag(p_tag);
			}
		}

		void bc_memory_stack::_destroy() noexcept(true)
		{
			core_platform::bc_mem_aligned_free(m_heap);
		}

		void* bc_memory_stack::push(bc_memblock* p_mem_block) noexcept(true)
		{
			void* l_result = nullptr;
			bcSIZE l_size = p_mem_block->size();

			bcAssert(l_size > 0);

			bcUBYTE* l_local_top = m_top.load(core_platform::bc_memory_order::seqcst);

			while (true)
			{
				bcUBYTE* l_new_top = l_local_top + l_size;
				bcSIZE l_byte_free = (m_heap + m_size - l_local_top) -
					(m_free_block_count.load(core_platform::bc_memory_order::seqcst) * sizeof(_bc_memory_stack_block));

				// No more free memory
				if (l_size > l_byte_free)
				{
					m_tracer.reject_alloc(l_size);

					return l_result;
				}

				if (m_top.compare_exchange_strong
				(
					&l_local_top,
					l_new_top,
					core_platform::bc_memory_order::seqcst,
					core_platform::bc_memory_order::seqcst
				))
				{
					l_result = l_local_top;

					m_tracer.accept_alloc(l_size);

					return l_result;
				}
			}
		}
		
		bool bc_memory_stack::pop(void* p_pointer, bc_memblock* p_mem_block) noexcept(true)
		{
			bcSIZE l_size = p_mem_block->size();
			bcUBYTE* l_local_top = m_top.load(core_platform::bc_memory_order::seqcst);
			bool l_is_freeing_top;

			if (l_local_top == m_heap) // it is free
			{
				bcAssert(false);
				return true;
			}

			l_is_freeing_top = l_local_top - l_size == p_pointer;

			if(!l_is_freeing_top)
			{
				// Check again if current block has become top block because after first compare it's 
				// possible duo to poping by other threads this block become top block
				l_local_top = m_top.load(core_platform::bc_memory_order::seqcst);
				l_is_freeing_top = l_local_top - l_size == p_pointer;

				if (!l_is_freeing_top)
				{
					return false;
				}
			}

			bcUBYTE* l_new_top = l_local_top - l_size;

			bool l_freed = m_top.compare_exchange_strong
			(
				&l_local_top,
				l_new_top,
				core_platform::bc_memory_order::seqcst,
				core_platform::bc_memory_order::seqcst
			);

			if(l_freed)
			{
				m_tracer.accept_free(l_size);

				return true;
			}

			return false;
		}

		void* bc_memory_stack::alloc(bc_memblock* p_memblock) noexcept(true)
		{
			return push(p_memblock);
		}

		void bc_memory_stack::free(void* p_pointer, bc_memblock* p_memblock) noexcept(true)
		{
			m_pop_thread_count.fetch_add(1U, core_platform::bc_memory_order::acquire);

			bool l_was_top = pop(p_pointer, p_memblock);

			if (!l_was_top)
			{
				core_platform::bc_shared_lock< core_platform::bc_shared_mutex > l_guard(m_free_block_mutex);
				{
					// Add current block to free list
					_add_free_block(p_pointer, p_memblock);
				}
			}

			auto l_poping_thread_count = m_pop_thread_count.fetch_sub(1U, core_platform::bc_memory_order::release);

			if (l_poping_thread_count == 1)
			{
				core_platform::bc_lock_guard< core_platform::bc_shared_mutex > l_guard(m_free_block_mutex);
				{
					if (m_pop_thread_count.load(core_platform::bc_memory_order::relaxed) == 0)
					{
						_reclaim_free_blocks();
					}
				}
			}
		}

		bool bc_memory_stack::contain_pointer(void* p_pointer) const noexcept(true)
		{
			return (p_pointer >= m_heap && p_pointer < m_heap + m_size) ? true : false;
		}

		void bc_memory_stack::clear() noexcept(true)
		{
			m_top.store(m_heap, core_platform::bc_memory_order::relaxed);
			m_free_block_count.store(0, core_platform::bc_memory_order::relaxed);

			m_tracer.accept_clear();
		}

		void bc_memory_stack::_add_free_block(void* p_pointer, bc_memblock* p_memblock)
		{
			auto l_free_block_count = m_free_block_count.fetch_add(1U, core_platform::bc_memory_order::relaxed) + 1;
			auto* l_free_block_top = reinterpret_cast< _bc_memory_stack_block* >(m_heap + m_size) - l_free_block_count;

			*l_free_block_top = _bc_memory_stack_block(p_pointer, p_memblock->size());

			// Use aquire ordering to get other threads changes to transitive them and release ordering to propagate 
			// all writes to _reclaim_free_blocks function
			m_free_block_count.fetch_or(0U, core_platform::bc_memory_order::acqrel);
		}

		void bc_memory_stack::_reclaim_free_blocks()
		{
			// Use acquire ordering to get all changes from _add_free_block function
			auto l_free_block_count = m_free_block_count.load(core_platform::bc_memory_order::acquire);

			if(l_free_block_count == 0)
			{
				return;
			}

			_bc_memory_stack_block* l_free_block_begin = reinterpret_cast< _bc_memory_stack_block* >(m_heap + m_size);
			_bc_memory_stack_block* l_free_block_top = l_free_block_begin - l_free_block_count;
			_bc_memory_stack_block* l_current_free_block = l_free_block_top;
			
			while(l_current_free_block != l_free_block_begin)
			{
				bcUBYTE* l_current_free_block_pointer = reinterpret_cast<bcUBYTE*>(l_current_free_block->m_address) + l_current_free_block->m_size;
				bcUBYTE* l_local_top = m_top.load(core_platform::bc_memory_order::relaxed);

				// If this free block is top
				if (l_current_free_block_pointer == l_local_top)
				{
					bc_memblock l_memblock;
					l_memblock.size(l_current_free_block->m_size);

					bool l_freed = pop(l_current_free_block->m_address, &l_memblock);

					if (!l_freed)
					{
						++l_current_free_block;
						continue;
					}

					// Mark this block as free
					l_current_free_block->m_address = nullptr;
					l_current_free_block->m_size = 0;

					// If current block is top block try to reclaim it and any other block that is marked with 
					// free flag
					if (l_current_free_block == l_free_block_top)
					{
						while (l_current_free_block->m_address == nullptr && l_current_free_block != l_free_block_begin)
						{
							m_free_block_count.fetch_sub(1U, core_platform::bc_memory_order::relaxed);
							l_free_block_top++;
							l_current_free_block++;
						}
					}
					// If current block isn't top reset searching because by poping this block other blocks
					// may become top block
					else
					{
						l_current_free_block = l_free_block_top;
					}
				}
				else
				{
					++l_current_free_block;
				}
			}
		}

		void bc_memory_stack::_move(this_type&& p_other)
		{
			bcUBYTE* l_top = p_other.m_top.load(core_platform::bc_memory_order::relaxed);
			bcUINT32 l_free_block_count = p_other.m_free_block_count.load(core_platform::bc_memory_order::relaxed);

			m_size = p_other.m_size;
			m_heap = p_other.m_heap;
			m_top.store(l_top, core_platform::bc_memory_order::relaxed);
			m_free_block_count.store(l_free_block_count, core_platform::bc_memory_order::relaxed);

			p_other.m_size = 0;
			p_other.m_heap = nullptr;
			p_other.m_top.store(0, core_platform::bc_memory_order::relaxed);
			p_other.m_free_block_count.store(0, core_platform::bc_memory_order::relaxed);
		}

#endif
	}
}
