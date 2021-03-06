// [01/27/2016 MRB]

#pragma once

#include "GraphicImp/bcDeviceReference.h"
#include "GraphicImp/bcDeviceRef.h"
#include "GraphicImp/bcDeviceResourceContent.h"
#include "Graphicimp/Shader/bcShader.h"

#include "Graphic/Shader/bcComputeShader.h"
#include "GraphicImp/GraphicImpPCH.h"

namespace black_cat
{
	namespace graphic
	{
		template<>
		struct bc_platform_compute_shader_pack<bc_render_api::directx11>
		{
			ID3D11ComputeShader* m_shader;
		};
	}
}