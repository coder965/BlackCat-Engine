// [01/14/2016 MRB]

#include "Graphic/GraphicPCH.h"
#include "Graphic/PipelineStage/bcProgrammableStage.h"
#include "Graphic/bcRenderApiInfo.h"

namespace black_cat
{
	namespace graphic
	{
		bc_programmable_stage_state::bc_programmable_stage_state()
			: //m_shader_program(nullptr),
			m_constant_buffers(nullptr),
			m_sampler_states(nullptr),
			m_shader_resource_views(nullptr),
			m_unordered_access_views(nullptr)
			/*m_uav_initial_counts(0, bc_render_api_info::number_of_ps_cs_uav_registers())*/
		{
		}

		bc_programmable_stage_state::~bc_programmable_stage_state()
		{
		}

		void bc_programmable_stage_state::set_to_initial_state()
		{
			//m_shader_program.set_to_initial_state();
			m_constant_buffers.set_to_initial_state();
			m_sampler_states.set_to_initial_state();
			m_shader_resource_views.set_to_initial_state();
			m_unordered_access_views.set_to_initial_state();
			/*m_uav_initial_counts.set_to_initial_state();*/
		}

		void bc_programmable_stage_state::reset_tracking()
		{
			//m_shader_program.reset_tracking();
			m_constant_buffers.reset_tracking();
			m_sampler_states.reset_tracking();
			m_shader_resource_views.reset_tracking();
			m_unordered_access_views.reset_tracking();
			/*m_uav_initial_counts.reset_tracking();*/
		}
	}
}