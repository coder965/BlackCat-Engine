// [02/24/2016 MRB]

#pragma once

#include "Core/CorePCH.h"
#include "Core/bcException.h"
#include "Core/Memory/bcAlloc.h"
#include "Core/Container/bcAllocator.h"

namespace black_cat
{
	namespace core
	{
		class _bc_parameter_pack_base
		{
		public:
			_bc_parameter_pack_base() = default;

			virtual ~_bc_parameter_pack_base() = default;

			virtual bcSIZE size() const = 0;

			virtual _bc_parameter_pack_base* copy(void* p_memory) = 0;

			virtual _bc_parameter_pack_base* move(void* p_memory) = 0;
		};

		template< typename T >
		class _bc_parameter_pack_imp : public _bc_parameter_pack_base
		{
		public:
			template< typename = typename std::enable_if< std::is_copy_constructible< T >::value >::type >
			explicit _bc_parameter_pack_imp(const T& p_data)
				: _bc_parameter_pack_base(),
				m_value(p_data)
			{
			}

			template< typename = typename std::enable_if< !std::is_copy_constructible< T >::value >::type >
			explicit _bc_parameter_pack_imp(T& p_data)
			{
				throw bc_logic_exception("You are trying to copy a non copiable object");
			}

			explicit _bc_parameter_pack_imp(T&& p_data)
				: _bc_parameter_pack_base(),
				m_value(std::move(p_data))
			{
			}

			~_bc_parameter_pack_imp() = default;

			bcSIZE size() const override
			{
				return sizeof(_bc_parameter_pack_imp<T>);
			}

			_bc_parameter_pack_base* copy(void* p_memory) override
			{
				_bc_parameter_pack_imp* l_pointer = reinterpret_cast<_bc_parameter_pack_imp*>(p_memory);
				new (l_pointer) _bc_parameter_pack_imp(m_value);

				return l_pointer;
			}

			_bc_parameter_pack_base* move(void* p_memory) override
			{
				_bc_parameter_pack_imp* l_pointer = reinterpret_cast<_bc_parameter_pack_imp*>(p_memory);
				new (l_pointer) _bc_parameter_pack_imp(std::move(m_value));

				return l_pointer;
			}

			T m_value;
		};

		/**
		 * \brief This class can hold any data type, (for small data types it use an internall buffer and for large data,
		 * memory will be allocated from frame allocator)
		 * You can use both copy and movale objects but if you use movale only object and try to copy this object
		 * a logic exception will be thrown
		 */
		class bc_parameter_pack : public bc_object_allocator
		{
		public:
			bc_parameter_pack();

			bc_parameter_pack(const bc_parameter_pack& p_other);

			bc_parameter_pack(bc_parameter_pack&& p_other);

			template< typename T, typename = typename std::enable_if< !std::is_same< typename std::decay<T>::type, bc_parameter_pack >::value >::type >
			explicit bc_parameter_pack(T&& p_value);

			~bc_parameter_pack();

			bc_parameter_pack& operator=(const bc_parameter_pack& p_other);

			bc_parameter_pack& operator=(bc_parameter_pack&& p_other);

			template<typename T>
			void set_value(T&& p_data);

			template< typename T >
			bool is() const;

			/**
			 * \brief Cast underlying type to requested type.
			 * \tparam T 
			 * \return Return null if underlying type mismatch
			 */
			template<typename T>
			T* as();

			template<typename T>
			const T* as() const;

			/**
			* \brief Return pointer to underlying buffer
			* \return Return null if there is no state
			*/
			template<>
			const void* as< void >() const;

			/**
			 * \brief Cast underlying type to requested type. Throw bad_cast exception if underlying type mismatch
			 * \tparam T 
			 * \return 
			 */
			template < typename T >
			T* as_throw();

			template < typename T >
			const T* as_throw() const;

			/**
			 * \brief Return pointer to underlying buffer. Throw bad_cast exception if there is no state
			 * \return 
			 */
			template <>
			const void* as_throw< void >() const;

			template<typename T>
			T release_as();

			/**
			 * \brief Clear internal states if any exist
			 */
			void reset();

			bool has_value() const;

			bool used_internall_buffer() const;

		protected:

		private:
			static constexpr bcUINT s_buffer_size = 5 * sizeof(void*);
			
			bcUINT m_buffer[s_buffer_size];

			bool m_used_internal_buffer;
			_bc_parameter_pack_base* m_object;
		};

		// Same as bc_parameter_pack but doesn't use frame allocator.
		class bc_any : public bc_parameter_pack
		{
		public:
			bc_any();

			explicit bc_any(const bc_parameter_pack& p_parameter);

			bc_any(const bc_any& p_other);

			bc_any(bc_any&& p_other);

			template< typename T, typename = typename std::enable_if< !std::is_same< typename std::decay<T>::type, bc_parameter_pack >::value >::type >
			explicit bc_any(T&& p_value);

			~bc_any();

			bc_any& operator=(const bc_parameter_pack& p_other);

			bc_any& operator=(const bc_any& p_other);

			bc_any& operator=(bc_any&& p_other);

		protected:

		private:
		};

		inline bc_parameter_pack::bc_parameter_pack()
			: m_used_internal_buffer(false),
			m_object(nullptr)
		{
			set_allocator_alloc_type(bc_alloc_type::frame);
		}

		inline bc_parameter_pack::bc_parameter_pack(const bc_parameter_pack& p_other)
			: bc_parameter_pack()
		{
			operator=(p_other);
		}

		inline bc_parameter_pack::bc_parameter_pack(bc_parameter_pack&& p_other)
			: bc_parameter_pack()
		{
			operator=(std::move(p_other));
		}

		template< typename T, typename = typename std::enable_if< !std::is_same< typename std::decay<T>::type, bc_parameter_pack >::value >::type >
		bc_parameter_pack::bc_parameter_pack(T&& p_value)
			: bc_parameter_pack()
		{
			set_value(std::forward<T>(p_value));
		}

		inline bc_parameter_pack::~bc_parameter_pack()
		{
			reset();
		}

		inline bc_parameter_pack& bc_parameter_pack::operator=(const bc_parameter_pack& p_other)
		{
			reset();

			if (p_other.m_used_internal_buffer)
			{
				m_object = p_other.m_object->copy(&m_buffer);
				m_used_internal_buffer = true;
			}
			else
			{
				bcBYTE* l_buffer = allocate<bcBYTE>(p_other.m_object->size()).release();
				m_object = p_other.m_object->copy(l_buffer);
				m_used_internal_buffer = false;
			}

			return *this;
		}

		inline bc_parameter_pack& bc_parameter_pack::operator=(bc_parameter_pack&& p_other)
		{
			reset();

			if (p_other.m_used_internal_buffer)
			{
				m_object = p_other.m_object->move(&m_buffer);
				m_used_internal_buffer = true;
			}
			else
			{
				m_object = p_other.m_object;
				m_used_internal_buffer = false;
			}

			p_other.m_object = nullptr;
			p_other.m_used_internal_buffer = false;

			return *this;
		}

		template < typename T >
		void bc_parameter_pack::set_value(T&& p_data)
		{
			// Because function is perfect forwarded if parameter be l-value T will be deduced to T&
			using T1 = typename std::remove_reference< T >::type;

			reset();

			if (sizeof(_bc_parameter_pack_imp<T1>) <= s_buffer_size)
			{
				new (&m_buffer) _bc_parameter_pack_imp<T1>(std::forward<T>(p_data));
				m_object = reinterpret_cast<_bc_parameter_pack_base*>(&m_buffer);
				m_used_internal_buffer = true;
			}
			else
			{
				m_object = allocate<_bc_parameter_pack_imp<T1>>(std::forward<T>(p_data)).release();
				m_used_internal_buffer = false;
			}
		}

		template< typename T >
		bool bc_parameter_pack::is() const
		{
			if (!has_value())
				return false;

			return dynamic_cast<_bc_parameter_pack_imp<T>*>(m_object) != nullptr;
		}

		template < typename T >
		T* bc_parameter_pack::as()
		{
			if (!has_value())
			{
				return nullptr;
			}
			
			auto* l_imp = dynamic_cast< _bc_parameter_pack_imp<T>* >(m_object);

			if(l_imp)
			{
				return &l_imp->m_value;
			}

			return nullptr;
		}

		template< typename T >
		const T* bc_parameter_pack::as() const
		{
			return const_cast< bc_parameter_pack& >(*this).as< T >();
		}

		template<>
		const void* bc_parameter_pack::as<void>() const
		{
			if (!has_value())
			{
				return nullptr;
			}

			return m_object;
		}

		template < typename T >
		T* bc_parameter_pack::as_throw()
		{
			T* l_value = as<T>();

			if (l_value == nullptr)
			{
				throw bc_bad_cast_exception();
			}

			return &l_value->m_value;
		}

		template< typename T >
		const T* bc_parameter_pack::as_throw() const
		{
			return const_cast< bc_parameter_pack& >(*this).as_throw< T >();
		}

		template<>
		const void* bc_parameter_pack::as_throw<void>() const
		{
			if (!has_value())
			{
				throw bc_bad_cast_exception();
			}

			return m_object;
		}

		template< typename T >
		T bc_parameter_pack::release_as()
		{
			T l_data = std::move(*as<T>());

			reset();

			return l_data;
		}

		inline void bc_parameter_pack::reset()
		{
			// Free allocated memory
			if(m_object && !m_used_internal_buffer)
			{
				deallocate(m_object);
			}
			// Only destruct constructed object in internal buffer
			else if(m_object && m_used_internal_buffer)
			{
				m_object->~_bc_parameter_pack_base();
			}

			m_used_internal_buffer = false;
			m_object = nullptr;
		}

		inline bool bc_parameter_pack::has_value() const
		{
			return m_object != nullptr;
		}

		inline bool bc_parameter_pack::used_internall_buffer() const
		{
			return m_used_internal_buffer;
		}

		inline bc_any::bc_any()
		{
			set_allocator_alloc_type(bc_alloc_type::unknown);
		}

		inline bc_any::bc_any(const bc_parameter_pack& p_parameter)
			: bc_any()
		{
			operator=(p_parameter);
		}

		inline bc_any::bc_any(const bc_any& p_other)
			: bc_any()
		{
			operator=(p_other);
		}

		inline bc_any::bc_any(bc_any&& p_other)
			: bc_any()
		{
			operator=(std::move(p_other));
		}

		template< typename T, typename = typename std::enable_if< !std::is_same< typename std::decay<T>::type, bc_parameter_pack >::value >::type >
		bc_any::bc_any(T&& p_value)
			: bc_any()
		{
			set_value(std::forward<T>(p_value));
		}

		inline bc_any::~bc_any()
		{
		}

		inline bc_any& bc_any::operator=(const bc_parameter_pack& p_other)
		{
			bc_parameter_pack::operator=(p_other);

			return *this;
		}

		inline bc_any& bc_any::operator=(const bc_any& p_other)
		{
			bc_parameter_pack::operator=(p_other);

			return *this;
		}

		inline bc_any& bc_any::operator=(bc_any&& p_other)
		{
			bc_parameter_pack::operator=(std::move(p_other));

			return *this;
		}
	}
}
