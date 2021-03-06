// [01/29/2016 MRB]

#include "GraphicImp/GraphicImpPCH.h"
#include "GraphicImp/bcExport.h"
#include "GraphicImp/Resource/bcResource.h"
#include "GraphicImp/Resource/Buffer/bcBufferConfig.h"
#include "GraphicImp/Resource/Buffer/bcBuffer.h"
#include "GraphicImp/Device/bcDevice.h"

namespace black_cat
{
	namespace graphic
	{
		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_buffer< g_api_dx11 >::bc_platform_buffer()
			: bc_platform_iresource()
		{
			m_pack.m_buffer = nullptr;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_buffer<g_api_dx11>::bc_platform_buffer(platform_pack& p_pack)
			: bc_platform_iresource(bc_platform_iresource::platform_pack(p_pack.m_buffer))
		{
			m_pack.m_buffer = p_pack.m_buffer;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_buffer< g_api_dx11 >::~bc_platform_buffer()
		{
		};

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_buffer< g_api_dx11 >::bc_platform_buffer(const bc_platform_buffer& p_other)
			: bc_platform_iresource(p_other)
		{
			m_pack.m_buffer = p_other.m_pack.m_buffer;
		};

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_buffer< g_api_dx11 >& bc_platform_buffer< g_api_dx11 >::operator=(const bc_platform_buffer& p_other)
		{
			bc_platform_iresource::operator=(p_other);
			m_pack.m_buffer = p_other.m_pack.m_buffer;

			return *this;
		};

		template<>
		BC_GRAPHICIMP_DLL
		bcUINT bc_platform_buffer< g_api_dx11 >::get_byte_width() const
		{
			D3D11_BUFFER_DESC l_desc;

			const_cast< bc_platform_buffer* >(this)->m_pack.m_buffer->GetDesc(&l_desc);

			return l_desc.ByteWidth;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_resource_usage bc_platform_buffer< g_api_dx11 >::get_usage() const
		{
			D3D11_BUFFER_DESC l_desc;

			const_cast< bc_platform_buffer* >(this)->m_pack.m_buffer->GetDesc(&l_desc);

			return bc_graphic_cast(l_desc.Usage);
		}

		template<>
		BC_GRAPHICIMP_DLL
		bcUINT bc_platform_buffer< g_api_dx11 >::get_structure_byte_stride() const
		{
			D3D11_BUFFER_DESC l_desc;

			const_cast< bc_platform_buffer* >(this)->m_pack.m_buffer->GetDesc(&l_desc);

			return l_desc.StructureByteStride;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_buffer< g_api_dx11 >::is_valid() const noexcept
		{
			return m_pack.m_buffer != nullptr;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_buffer<g_api_dx11>::operator==(const bc_platform_buffer& p_other) const noexcept
		{
			return m_pack.m_buffer == p_other.m_pack.m_buffer;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_buffer<g_api_dx11>::operator!=(const bc_platform_buffer& p_other) const noexcept
		{
			return !operator==(p_other);
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_buffer<g_api_dx11>::operator==(std::nullptr_t) const noexcept
		{
			return !is_valid();
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_buffer<g_api_dx11>::operator!=(std::nullptr_t) const noexcept
		{
			return is_valid();
		}
	}
}