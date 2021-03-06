// [02/26/2016 MRB]

#pragma once

#include "GraphicImp/Device/bcDevicePipelineState.h"
#include "GraphicImp/Device/Command/bcDeviceCommandExecuter.h"
#include "GraphicImp/Resource/Texture/bcTexture2d.h"
#include "GraphicImp/Resource/View/bcRenderTargetView.h"
#include "GraphicImp/Resource/View/bcDepthStencilView.h"
#include "Game/System/Render/bcRenderPass.h"
#include "Game/System/Render/bcRenderPassState.h"
#include "BlackCat/bcExport.h"

namespace black_cat
{
	class BC_BLACKCAT_DLL bc_back_buffer_output_pass : public game::bc_irender_pass
	{
		BC_RENDER_PASS(back_buffer_output_pass)

	public:
		bc_back_buffer_output_pass() = default;

		~bc_back_buffer_output_pass() = default;

		bc_back_buffer_output_pass(bc_back_buffer_output_pass&&) = default;

		bc_back_buffer_output_pass& operator=(const bc_back_buffer_output_pass&) = default;

		void initialize_resources(game::bc_render_system& p_render_system) override;

		void update(const game::bc_render_system_update_param& p_update_param) override;

		void initialize_frame(game::bc_render_system& p_render_system, game::bc_scene& p_scene, game::bc_render_thread& p_thread) override;

		void execute(game::bc_render_system& p_render_system, game::bc_scene& p_scene, game::bc_render_thread& p_thread) override;

		void destroy(graphic::bc_device& p_device) override;

		void before_reset(game::bc_render_system& p_render_system, graphic::bc_device& p_device, graphic::bc_device_parameters& p_old_parameters, graphic::bc_device_parameters& p_new_parameters) override;

		void after_reset(game::bc_render_system& p_render_system, graphic::bc_device& p_device, graphic::bc_device_parameters& p_old_parameters, graphic::bc_device_parameters& p_new_parameters) override;

	protected:

	private:
		graphic::bc_device_command_list_ptr m_command_list;
		graphic::bc_device_pipeline_state_ptr m_pipeline_state;
		game::bc_render_pass_state_ptr m_render_pass_state;
	};
}
