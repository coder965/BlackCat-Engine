// [04/04/2016 MRB]

#include "Core/CorePCH.h"
#include "Core/File/bcContent.h"
#include "Core/File/bcContentManager.h"

namespace black_cat
{
	namespace core
	{
		_bc_content_ptr_deleter::_bc_content_ptr_deleter()
			: m_content_manager(nullptr) 
		{
		}

		_bc_content_ptr_deleter::_bc_content_ptr_deleter(bc_content_manager* p_content_manager)
			: m_content_manager(p_content_manager)
		{
		}

		void _bc_content_ptr_deleter::operator()(bc_icontent* p_content) const
		{
			bcAssert(m_content_manager != nullptr);

			m_content_manager->destroy_content(p_content);
		}
	}
}
