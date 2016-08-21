// [02/11/2016 MRB]

#include "GraphicImp/GraphicImpPCH.h"
#include "GraphicImp/bcExport.h"
#include "GraphicImp/Resource/bcResource.h"
#include "GraphicImp/Resource/View/bcDepthStencilViewConfig.h"
#include "GraphicImp/Resource/View/bcDepthStencilView.h"
#include "GraphicImp/Device/bcDevice.h"

namespace black_cat
{
	namespace graphic
	{
		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_depth_stencil_view< g_api_dx11 >::bc_platform_depth_stencil_view()
			: m_pack()
		{
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_depth_stencil_view< g_api_dx11 >::~bc_platform_depth_stencil_view()
		{
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_depth_stencil_view< g_api_dx11 >::bc_platform_depth_stencil_view(bc_platform_depth_stencil_view&& p_other)
			: bc_iresource_view(std::move(p_other)),
			m_pack(std::move(p_other.m_pack))
		{
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_depth_stencil_view< g_api_dx11 >& bc_platform_depth_stencil_view< g_api_dx11 >::operator=(bc_platform_depth_stencil_view&& p_other)
		{
			bc_iresource_view::operator=(std::move(p_other));
			m_pack = std::move(p_other.m_pack);

			return *this;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_resource_view_type bc_platform_depth_stencil_view< g_api_dx11 >::get_view_type() const
		{
			return bc_resource_view_type::depth_stencil;
		}
	}
}