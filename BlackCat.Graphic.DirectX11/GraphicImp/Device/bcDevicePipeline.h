// [01/26/2016 MRB]

#pragma once

// Because explicit specialization of template classes must seen first by compiler
// in every 'imp' file we include all 'imp' files that are used
#include "GraphicImp/Device/bcDevice.h"
#include "GraphicImp/Device/bcDevicePipelineState.h"
#include "GraphicImp/Device/Command/bcDeviceCommandList.h"
#include "GraphicImp/PipelineStage/bcInputAssemblerStage.h"
#include "GraphicImp/PipelineStage/bcStreamOutputStage.h"
#include "GraphicImp/PipelineStage/bcRasterizerStage.h"
#include "GraphicImp/PipelineStage/bcOutputMergerStage.h"
#include "GraphicImp/PipelineStage/bcVertexStage.h"
#include "GraphicImp/PipelineStage/bcHullStage.h"
#include "GraphicImp/PipelineStage/bcDomainStage.h"
#include "GraphicImp/PipelineStage/bcGeometryStage.h"
#include "GraphicImp/PipelineStage/bcPixelStage.h"
#include "GraphicImp/PipelineStage/bcComputeStage.h"
#include "GraphicImp/Resource/bcResource.h"

#include "Graphic/bcRenderApiInfo.h"
#include "Graphic/Device/bcDevicePipeline.h"
#include "GraphicImp/GraphicImpPCH.h"

namespace black_cat
{
	namespace graphic
	{
		template< >
		struct bc_platform_device_pipeline_pack<bc_platform_render_api::directx11> 
			: public bc_platform_device_pipeline_pack<bc_platform_render_api::unknown>
		{
			Microsoft::WRL::ComPtr< ID3D11DeviceContext > m_context;
			Microsoft::WRL::ComPtr< ID3D11Query > m_query;
		};
	}
}