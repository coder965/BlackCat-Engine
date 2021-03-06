// [03/28/2016 MRB]

#pragma once

#include "CorePlatform/Utility/bcNoCopy.h"
#include "Core/bcExport.h"
#include "Core/bcConstant.h"
#include "Core/Utility/bcRefCountPtr.h"

namespace black_cat
{
	namespace core
	{
		class bc_icontent;
		class bc_content_manager;

		using bc_content_hash_t = std::hash< const bcECHAR* >;

		class BC_CORE_DLL _bc_content_ptr_deleter
		{
		public:
			_bc_content_ptr_deleter();

			_bc_content_ptr_deleter(bc_content_manager* p_content_manager);

			void operator ()(bc_icontent* p_content) const;

		protected:

		private:
			bc_content_manager* m_content_manager;
		};

		template< typename TContent >
		using bc_content_ptr = bc_ref_count_ptr< TContent, _bc_content_ptr_deleter >;

		template< class TContent >
		class bc_content_traits
		{
		public:
			static constexpr const bcCHAR* content_name()
			{
				return TContent::content_name();
			}

			static constexpr bcUINT32 content_hash()
			{
				return TContent::content_hash();
			}
		};

		class bc_icontent : 
			private bc_ref_count,
			public core_platform::bc_no_copy
		{
		public:
			template< class T, class TDeleter >
			friend class bc_ref_count_ptr;

		public:
			virtual ~bc_icontent() = default;

			bc_content_hash_t::result_type _get_hash() const
			{
				return m_hash;
			}

			void _set_hash(bc_content_hash_t::result_type p_hash)
			{
				m_hash = p_hash;
			}

		protected:
			bc_icontent() = default;

			bc_icontent(bc_icontent&&) = default;

			bc_icontent& operator=(bc_icontent&&) = default;

		private:
			bc_content_hash_t::result_type m_hash;
		};

		using bc_icontent_ptr = bc_content_ptr< bc_icontent >;
	}
}