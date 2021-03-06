//  [8/23/2013 MRB]

#pragma once

#include "Core/CorePCH.h"
#include "Core/Memory/bcMemBlock.h"
#include "Core/Memory/bcMemoryTracer.h"
#include "Core/Utility/bcDelegate.h"

namespace black_cat
{
	namespace core
	{
		class bc_memory : private core_platform::bc_no_copy
		{
		public:
			using this_type = bc_memory;

		public:
			bc_memory() {}

			bc_memory(this_type&& p_other) noexcept
			{
				_assign(std::move(p_other));
			}

			virtual ~bc_memory() {}

			this_type& operator =(this_type&& p_other) noexcept
			{
				_assign(std::move(p_other));

				return *this;
			}

			void tag(const bcCHAR* p_tag)
			{
				// In allocators we call this function only in theirs Initialize method, so it is safe /
				std::memcpy(m_tag, p_tag, std::min<bcSIZE>(bcSIZE(s_tag_lenght - 1), strlen(p_tag)));
				*(m_tag + std::min<bcSIZE>(bcSIZE(s_tag_lenght - 1), strlen(p_tag))) = '\0';
			}

			const bcCHAR* tag() const noexcept 
			{ 
				return m_tag; 
			}

			const bc_memory_tracer& tracer() const noexcept 
			{ 
				return m_tracer; 
			}

			virtual void* alloc(bc_memblock* p_memblock) noexcept = 0;

			virtual void free(void* p_pointer, bc_memblock* p_memblock) noexcept = 0;

			virtual bool contain_pointer(void* p_memory) const noexcept = 0;

			virtual void clear() noexcept = 0;

		protected:
			bc_memory_tracer m_tracer;

		private:
			static const bcUINT8 s_tag_lenght = 15;
			bcCHAR m_tag[s_tag_lenght];

			void _assign(this_type&& p_other)
			{
				tag(p_other.tag());
				m_tracer = const_cast< bc_memory_tracer& >(p_other.tracer());
			}
		};

		class bc_memory_movable : public bc_memory
		{
		public:
			using this_type = bc_memory_movable;
			using defrag_callback = bc_delegate<void(void*,void*)>;

		public:
			bc_memory_movable() {}

			bc_memory_movable(this_type&& p_other) noexcept
				: bc_memory(std::move(p_other))
			{
			}

			virtual ~bc_memory_movable() {}

			this_type& operator =(this_type&& p_other) noexcept
			{
				bc_memory::operator =(std::move(p_other));

				return *this;
			}

#ifdef BC_MEMORY_DEFRAG
			virtual void register_pointer(void** p_pointer, bc_memblock* p_memblock) noexcept = 0;

			virtual void unregister_pointer(void** p_pointer, bc_memblock* p_memblock) noexcept = 0;

			virtual void defragment(bcINT32 p_num_defrag, defrag_callback p_defrag_callback) noexcept = 0;
#endif

		protected:

		private:
		};
	}
}
