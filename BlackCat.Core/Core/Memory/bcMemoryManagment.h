//  [8/30/2013 MRB]

#pragma once

#include "Core/CorePCH.h"
#include "Core/bcExport.h"
#include "Core/Utility/bcInitializable.h"
#include "Core/Memory/bcMemoryCRT.h"
#include "Core/Memory/bcMemoryFixedSize.h"
#include "Core/Memory/bcMemoryStack.h"
#include "Core/Memory/bcMemoryHeap.h"
#include "Core/Memory/bcMemMngHashMap.h"

namespace black_cat
{
	namespace core
	{
		enum class bc_mem_size
		{
			b = 1,
			kb = 1024,
			mb = 1048576
		};

		enum class bc_alloc_type : bcUBYTE
		{
			// Alloc in programm stack
			program,
			// Alloc in level stack
			level,
			// Alloc in frame stack
			frame,
			// Try to alloc in fixed size allocators, otherwise alloc in crt memory
			unknown,
			// Try to alloc in fixed size allocators, then in super heap that is defragmentable, otherwise in crt memory
			unknown_movale,
		};

#ifdef BC_MEMORY_ENABLE

		template< typename TMemory >
		class bc_memory_extender; // used forward decleration duo to dependencies between bcPtr and bcDelegate

		class BC_CORE_DLL bc_memmng : core_platform::bc_no_copy
		{
		public:
			bc_memmng() noexcept(true);

			~bc_memmng();

			void initialize(bcUINT32 p_fsa_start_size,
				bcUINT32 p_fsa_num,
				bcUINT32 p_fsa_num_allocations,
				bcUINT32 p_per_prg_heap_size,
				bcUINT32 p_per_lvl_heap_size,
				bcUINT32 p_per_frm_heap_size,
				bcUINT32 p_super_heap_size);

			void destroy() noexcept(true);

			static void startup(bcUINT32 p_fsa_start_size,
				bcUINT32 p_fsa_num,
				bcUINT32 p_fsa_num_allocations,
				bcUINT32 p_per_prg_heap_size,
				bcUINT32 p_per_lvl_heap_size,
				bcUINT32 p_per_frm_heap_size,
				bcUINT32 p_super_heap_size);

			static void close() noexcept(true);

			bcInline static bc_memmng& get() noexcept(true)
			{
				return bc_memmng::m_instance;
			};

			void* alloc(bcSIZE p_size, bc_alloc_type p_alloc_type, const bcCHAR* p_file, bcUINT32 p_line) noexcept(true);

			void free(void* p_pointer) noexcept(true);

			void* realloc(void* p_pointer, bcSIZE p_new_size, bc_alloc_type p_alloc_type, const bcCHAR* p_file, bcUINT32 p_line) noexcept(true);

			void* aligned_alloc(bcSIZE p_size, bcINT32 p_alignment, bc_alloc_type p_alloc_type, const bcCHAR* p_file, bcUINT32 p_line) noexcept(true);

			void aligned_free(void* p_pointer) noexcept(true);
			
			void* aligned_realloc(void* p_pointer, bcSIZE p_new_size, bcINT32 p_alignment, bc_alloc_type p_alloc_type, const bcCHAR* p_file, bcUINT32 p_line) noexcept(true);

#ifdef BC_MEMORY_DEFRAG
			bcInline void register_pointer_in_movable_allocators(void** p_pointer) noexcept(true)
			{
				if (m_super_heap->contain_pointer(*p_pointer))
				{
					m_super_heap->register_pointer(p_pointer);
				}
			};

			bcInline void unregister_pointer_in_movable_allocators(void** p_pointer) noexcept(true)
			{
				if (m_super_heap->contain_pointer(*p_pointer))
				{
					m_super_heap->unregister_pointer(p_pointer);
				}
			};
#endif

			void end_of_frame() noexcept(true);

			bcSIZE get_total_size();

			bcSIZE get_used_size();

			bcSIZE get_overhead_size();

			bcSIZE get_max_used_size();

#ifdef BC_MEMORY_LEAK_DETECTION
			bcUINT32 report_memory_leaks();
#endif

		protected:

		private:
			bcInline bcUINT32 _fsa_index_max_size(bcUINT32 p_index)
			{
				return m_fsa_allocators_start_size + p_index * s_fsa_step_size;
			};

			bcInline bcUINT32 _get_fsa_index(bcSIZE p_size)
			{
				bcUINT32 l_index = std::ceil((p_size - m_fsa_allocators_start_size) / s_fsa_step_size);
				return l_index;
			};

			// We don't use static pointer because it is possible after we free pointer on exit, some call to free be called
			static bc_memmng m_instance;
			static const bcUINT32 s_fsa_step_size = BC_MEMORY_MIN_ALIGN;

			bcUINT32 m_fsa_allocators_start_size;
			bcUINT32 m_fsa_num_allocators;
			bool m_initialized;
			bc_memory_extender<bc_memory_fixed_size>* m_fsa_allocators;
			bc_memory_stack* m_per_program_stack;
			bc_memory_stack* m_per_level_stack;
			bc_memory_stack* m_per_frame_stack;
			bc_memory_heap* m_super_heap;
			bc_memory_crt* m_crt_allocator;
#ifdef BC_MEMORY_LEAK_DETECTION
			bcSIZE m_allocation_count;
			bc_memmng_hashmap<bc_mem_block_leak_information, 15, 5>* m_leak_allocator;
#endif
		};

#endif

	}
}