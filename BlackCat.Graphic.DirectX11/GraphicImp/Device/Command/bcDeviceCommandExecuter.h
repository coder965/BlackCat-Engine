// [01/27/2016 MRB]

#pragma once

#include "GraphicImp/bcDeviceReference.h"
#include "GraphicImp/bcDeviceRef.h"
#include "GraphicImp/Device/bcDevice.h"
#include "GraphicImp/Device/Command/bcDeviceCommandList.h"

#include "Graphic/Device/Command/bcDeviceCommandExecuter.h"
#include "GraphicImp/GraphicImpPCH.h"

using namespace Microsoft::WRL;

namespace black_cat
{
	namespace graphic
	{
		template< >
		struct bc_platform_device_command_executer_pack<g_api_dx11>
		{
			bc_device* m_device;
		};
	}
}
