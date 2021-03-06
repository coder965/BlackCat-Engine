// [11/28/2016 MRB]

#pragma once

#include "CorePlatform/bcType.h"
#include "Core/Container/bcAllocator.h"
#include "Core/Utility/bcLogger.h"
#include "Physics/bcPhysicsApi.h"
#include "Physics/Fundation/bcTask.h"

namespace black_cat
{
	namespace physics
	{
		class bc_iallocator
		{
		public:
			virtual ~bc_iallocator() = default;

			virtual void* allocate(bcSIZE p_byte, const bcCHAR* p_file, bcUINT32 p_line) = 0;

			virtual void free(void* p_memory) = 0;

			virtual void* temp_allocate(bcSIZE p_byte, const bcCHAR* p_file, bcUINT32 p_line) = 0;

			virtual void temp_free(void* p_memory) = 0;
		};

		class bc_ilogger
		{
		public:
			virtual ~bc_ilogger() = default;

			virtual void info(bcINT p_code, const bcCHAR* p_message, const bcCHAR* p_file, bcUINT32 p_line) const = 0;

			virtual void debug(bcINT p_code, const bcCHAR* p_message, const bcCHAR* p_file, bcUINT32 p_line) const = 0;

			virtual void error(bcINT p_code, const bcCHAR* p_message, const bcCHAR* p_file, bcUINT32 p_line) const = 0;
		};

		class bc_itask_dispatcher
		{
		public:
			virtual ~bc_itask_dispatcher() = default;

			virtual void add_task(bc_task&& p_task) = 0;

			virtual bcSIZE worker_count() const noexcept = 0;
		};

		template< class TAllocator, class TTempAllocator >
		class bc_default_allocator : public bc_iallocator
		{
		public:
			void* allocate(bcSIZE p_byte, const bcCHAR* p_file, bcUINT32 p_line) override
			{
				return core::bc_allocator_traits<TAllocator>::allocate(m_allocator, p_byte);
			}

			void free(void* p_memory) override
			{
				auto* l_pointer = reinterpret_cast< typename core::bc_allocator_traits< TAllocator>::value_type*>(p_memory);
				core::bc_allocator_traits< TAllocator >::deallocate(m_allocator, l_pointer);
			}

			void* temp_allocate(bcSIZE p_byte, const bcCHAR* p_file, bcUINT32 p_line) override
			{
				return core::bc_allocator_traits<TTempAllocator>::allocate(m_temp_allocator, p_byte);
			}

			void temp_free(void* p_memory) override
			{
				auto* l_pointer = reinterpret_cast< typename core::bc_allocator_traits< TAllocator>::value_type*>(p_memory);
				core::bc_allocator_traits< TTempAllocator >::deallocate(m_temp_allocator, l_pointer);
			}

		private:
			TAllocator m_allocator;
			TTempAllocator m_temp_allocator;
		};

		class bc_default_logger : public bc_ilogger
		{
		public:
			explicit bc_default_logger(core::bc_logger* p_logger)
				: m_logger(p_logger)
			{
			}

			void info(bcINT p_code, const bcCHAR* p_message, const bcCHAR* p_file, bcUINT32 p_line) const override
			{
				auto l_message = core::bc_to_estring_frame(p_message);
				m_logger->log_info(l_message.c_str());
			}

			void debug(bcINT p_code, const bcCHAR* p_message, const bcCHAR* p_file, bcUINT32 p_line) const override
			{
				auto l_message = core::bc_to_estring_frame(p_message);
				m_logger->log_debug(l_message.c_str());
			}

			void error(bcINT p_code, const bcCHAR* p_message, const bcCHAR* p_file, bcUINT32 p_line) const override
			{
				auto l_message = core::bc_to_estring_frame(p_message);
				m_logger->log_error(l_message.c_str());
			}

		private:
			core::bc_logger* m_logger;
		};

		class bc_default_task_dispatcher : public bc_itask_dispatcher
		{
		public:
			void add_task(bc_task&& p_task) override
			{
				core::bc_concurreny::start_task< void >
				(
					[t = std::move(p_task)]() mutable
					{
						t.run();
					},
					core::bc_task_creation_option::policy_none
				);
			}

			bcSIZE worker_count() const noexcept override
			{
				return core::bc_concurreny::worker_count();
			}

		protected:

		private:
		};
	}
}