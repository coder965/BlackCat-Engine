// [10/11/2014 MRB]

#pragma once

#include "CorePlatform/CorePlatformPCH.h"
#include "CorePlatform/bcType.h"
#include "CorePlatform/bcCorePlatformUtility.h"
#include "CorePlatform/Concurrency/bcConcurrencyDef.h"
#include "CorePlatform/Concurrency/bcAtomicProvider.h"
#include "CorePlatform/Memory/bcMemAlloc.h"
#include <stack>
#include <map>
#include <atomic>

namespace black_cat
{
	namespace core_platform
	{
		template< bc_platform TP >
		class bc_thread_provider : private bc_no_copy
		{
		public:

		protected:

		private:

		};

		class bc_per_thread_data : private bc_no_copy
		{
		public:
			using this_type = bc_per_thread_data;

		public:
			bc_per_thread_data()
				: m_exiter()
			{
			};

			bc_per_thread_data(this_type&& p_other)
				: m_exiter(std::move(p_other.m_exiter))
			{
			}

			~bc_per_thread_data() = default;

			this_type& operator =(this_type&& p_other)
			{
				m_exiter = std::move(p_other.m_exiter);

				return *this;
			}

			void add_exit_func(std::function< void() > p_func)
			{
				m_exiter.add(p_func);
			}

		protected:

		private:
			class _thread_exiter : private bc_no_copy
			{
			public:
				using this_type = _thread_exiter;

			public:
				_thread_exiter() = default;

				_thread_exiter(this_type&& p_other)
					: m_exit_funcs(std::move(p_other.m_exit_funcs))
				{
				}

				~_thread_exiter()
				{
					while (!m_exit_funcs.empty())
					{
						m_exit_funcs.top()();
						m_exit_funcs.pop();
					}
				}

				this_type& operator =(this_type&& p_other)
				{
					m_exit_funcs = std::move(p_other.m_exit_funcs);

					return *this;
				}

				void add(std::function< void() > p_func)
				{
					m_exit_funcs.push(std::move(p_func));
				}

			protected:

			private:
				std::stack< std::function< void() > > m_exit_funcs;
			};

			_thread_exiter m_exiter;
		};

		template< class TProvider >
		class bc_thread_proxy : private bc_no_copy
		{
			using provider_type = TProvider;
			using this_type = bc_thread_proxy< provider_type >;

		public:
			bc_thread_proxy() noexcept(true)
				: m_provider()
			{
			}

			bc_thread_proxy(this_type&& p_other) noexcept(true)
				: m_provider(std::move(p_other.m_provider))
			{
			}

			bc_thread_proxy(const this_type& p_other) = delete;

			template< typename Callable, typename ...Args >
			explicit bc_thread_proxy(Callable&& pFunc, Args&&... pArgs)
				: m_provider(std::forward<Callable>(pFunc), std::forward<Args>(pArgs)...)
			{
			}

			~bc_thread_proxy()
			{
				this_type::s_static_data.remove_me();
			}

			this_type& operator =(this_type&& p_other) noexcept(true)
			{
				m_provider = std::move(p_other.m_provider);

				return *this;
			}

			bcInline void swap(this_type& p_other) noexcept(true)
			{
				m_provider.swap(p_other.m_provider);
			}

			bcInline void join()
			{
				m_provider.join();
			}

			bcInline void detach()
			{
				m_provider.detach();
			}

			bcInline bool joinable() const noexcept(true)
			{
				return m_provider.joinable();
			}

			bcInline bcUINT32 get_id() const noexcept(true)
			{
				return m_provider.get_id();
			}

			/*bcInline static bcUINT32 hardware_concurrency() noexcept(true)
			{
				return provider_type::hardware_concurrency();
			}*/

			bcInline static bcUINT32 current_thread_id() noexcept(true)
			{
				return provider_type::current_thread_id();
			}

			bcInline static void current_thread_sleep_for(bcUINT64 p_nano)
			{
				provider_type::current_thread_sleep_for(p_nano);
			}

			// make the processor available to other logical processors in a hyper threading enabled processor so that the other 
			// logical processors can make progress
			bcInline static void current_thread_yield() noexcept(true)
			{
				provider_type::current_thread_yield();
			}

			// reschedule the execution of current thread, allowing other threads to run
			bcInline static void current_thread_yield_switch() noexcept(true)
			{
				provider_type::current_thread_yield_switch();
			}

			bcInline static void current_thread_on_exit(std::function< void() > p_func)
			{
				//// http://stackoverflow.com/questions/20112221/invoking-a-function-automatically-on-stdthread-exit-in-c11
				//class thread_exiter : private bc_no_copy
				//{
				//public:
				//	thread_exiter() = default;

				//	~thread_exiter()
				//	{
				//		while (!m_exit_funcs.empty())
				//		{
				//			m_exit_funcs.top()();
				//			m_exit_funcs.pop();
				//		}
				//	}

				//	void add(std::function< void() > p_func)
				//	{
				//		m_exit_funcs.push(std::move(p_func));
				//	}

				//protected:

				//private:
				//	std::stack< std::function< void() > > m_exit_funcs;
				//};

				//thread_local static thread_exiter l_exiter;
				//l_exiter.add(std::move(p_func));

				this_type::s_static_data.me().add_exit_func(p_func);
			}

			// Call this function when main thread is about exit, to remove it's per-thread-data
			static void on_main_thread_exit()
			{
				this_type::s_static_data.remove_me();
			}

		protected:

		private:
			class _static_data
			{
			private:
				using map_type = std::map< bcUINT32, bc_per_thread_data >;

			public:
				_static_data() : m_flag(), m_threads()
				{
					m_flag.clear(std::memory_order::memory_order_relaxed);
				}

				~_static_data() {}

				void remove_me()
				{
					lock();

					m_threads.erase(this_type::current_thread_id());

					unlock();
				}

				bc_per_thread_data& me()
				{
					bcUINT32 l_thread_id = this_type::current_thread_id();

					lock_shared();

					auto l_iterator = m_threads.find(l_thread_id);
					
					unlock_shared();

					if (l_iterator == m_threads.end())
					{
						lock();

						l_iterator = m_threads.insert(map_type::value_type(l_thread_id, bc_per_thread_data())).first;

						unlock();
					}

					return *&l_iterator->second;
				}

			protected:

			private:
				bcInline void lock()
				{
					while (m_flag.test_and_set(std::memory_order::memory_order_acquire));
				}

				bcInline void lock_shared()
				{
					lock();
				}

				bcInline void unlock()
				{
					m_flag.clear(std::memory_order::memory_order_release);
				}

				bcInline void unlock_shared()
				{
					unlock();
				}

				std::atomic_flag m_flag; // TODO
				map_type m_threads;
			};

			static _static_data s_static_data;
			provider_type m_provider;

		};

		template< typename TProvider >
		typename bc_thread_proxy< TProvider >::_static_data bc_thread_proxy< TProvider >::s_static_data;

		using bc_thread = bc_thread_proxy< bc_thread_provider< g_current_platform > >;
	}
}