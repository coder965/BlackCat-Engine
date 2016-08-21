// [02/26/2016 MRB]

#include "Game/GamePCH.h"
#include "Game/Graphic/Command/bcPipelineDrawIndexedInstancedCommand.h"

namespace black_cat
{
	namespace game
	{
		bc_pipeline_draw_indexed_instanced_command::bc_pipeline_draw_indexed_instanced_command(graphic::bc_primitive p_primitive,
			graphic::bc_format p_index_format,
			graphic::bc_buffer_ptr p_vertex_buffer,
			graphic::bc_buffer_ptr p_index_buffer,
			graphic::bc_buffer_ptr p_instance_buffer)
			: bc_pipeline_draw_indexed_command(p_primitive, p_index_format, p_vertex_buffer, p_index_buffer),
			m_instance_buffer(p_instance_buffer)
		{
		}

		void bc_pipeline_draw_indexed_instanced_command::execute(graphic::bc_device_pipeline* p_pipeline, core::bc_parameter_pack&& p_parameters)
		{
			bcAssert(p_parameters.has_value());

			bc_draw_indexed_instanced_command_parameters* l_parameters = p_parameters.as<bc_draw_indexed_instanced_command_parameters>();
			
			p_pipeline->draw_indexed_instanced
				(
					l_parameters->m_index_count_per_instance,
					l_parameters->m_instance_count,
					l_parameters->m_start_index_location,
					l_parameters->m_base_vertex_location,
					l_parameters->m_start_instance_location
				);
		}

		void bc_pipeline_draw_indexed_instanced_command::execute(graphic::bc_device_pipeline* p_pipeline,
			bcUINT p_index_count_per_instance,
			bcUINT p_instance_count,
			bcUINT p_start_index_location,
			bcINT p_base_vertex_location,
			bcUINT p_start_instance_location)
		{
			p_pipeline->pipeline_set_default_states(graphic::bc_pipeline_stage::input_assembler_stage);

			bcUINT32 l_strides = m_vertex_buffer->get_structure_byte_stride();
			bcUINT32 l_offsets = 0;
			bcUINT32 l_instance_strides = m_instance_buffer->get_structure_byte_stride();
			bcUINT32 l_instance_offsets = 0;

			p_pipeline->bind_ia_primitive_topology(m_primitive);
			p_pipeline->bind_ia_vertex_buffers(0, 1, m_vertex_buffer.get(), &l_strides, &l_offsets);
			p_pipeline->bind_ia_index_buffer(m_index_buffer.get(), m_index_format);
			p_pipeline->bind_ia_vertex_buffers(1, 1, m_instance_buffer.get(), &l_instance_strides, &l_instance_offsets);

			p_pipeline->pipeline_apply_states(graphic::bc_pipeline_stage::input_assembler_stage);

			p_pipeline->draw_indexed_instanced
				(
					p_index_count_per_instance,
					p_instance_count,
					p_start_index_location,
					p_base_vertex_location,
					p_start_instance_location
				);
		}
	}
}