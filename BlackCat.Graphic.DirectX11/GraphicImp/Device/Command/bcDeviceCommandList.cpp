// [01/27/2016 MRB]

#include "GraphicImp/GraphicImpPCH.h"
#include "GraphicImp/bcExport.h"
#include "GraphicImp/Device/Command/bcDeviceCommandList.h"

namespace black_cat
{
	namespace graphic
	{
		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_device_command_list< g_api_dx11 >::bc_platform_device_command_list()
		{
			m_pack.m_command_list_proxy = nullptr;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_device_command_list<g_api_dx11>::bc_platform_device_command_list(platform_pack& p_pack)
		{
			m_pack.m_command_list_proxy = p_pack.m_command_list_proxy;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_device_command_list< g_api_dx11 >::bc_platform_device_command_list(const bc_platform_device_command_list& p_other)
			: bc_platform_device_reference(p_other)
		{
			m_pack.m_command_list_proxy = p_other.m_pack.m_command_list_proxy;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_device_command_list< g_api_dx11 >::~bc_platform_device_command_list()
		{
		}

		template<>
		BC_GRAPHICIMP_DLL
		bc_platform_device_command_list< g_api_dx11 >& bc_platform_device_command_list< g_api_dx11 >::operator=(const bc_platform_device_command_list& p_other)
		{
			bc_platform_device_reference::operator=(p_other);
			m_pack.m_command_list_proxy = p_other.m_pack.m_command_list_proxy;

			return *this;
		}

		template<>
		BC_GRAPHICIMP_DLL
		void bc_platform_device_command_list< g_api_dx11 >::reset()
		{
			if(is_valid() && m_pack.m_command_list_proxy->m_command_list)
			{
				m_pack.m_command_list_proxy->m_command_list->Release();
				m_pack.m_command_list_proxy->m_command_list = nullptr;
			}
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_device_command_list< g_api_dx11 >::is_valid() const noexcept
		{
			return m_pack.m_command_list_proxy != nullptr;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_device_command_list<g_api_dx11>::operator==(const bc_platform_device_command_list& p_other) const noexcept
		{
			return m_pack.m_command_list_proxy == p_other.m_pack.m_command_list_proxy;
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_device_command_list<g_api_dx11>::operator!=(const bc_platform_device_command_list& p_other) const noexcept
		{
			return !operator==(p_other);
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_device_command_list<g_api_dx11>::operator==(std::nullptr_t) const noexcept
		{
			return !is_valid();
		}

		template<>
		BC_GRAPHICIMP_DLL
		bool bc_platform_device_command_list<g_api_dx11>::operator!=(std::nullptr_t) const noexcept
		{
			return is_valid();
		}
	}
}