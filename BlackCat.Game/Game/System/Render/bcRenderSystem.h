// [04/28/2016 MRB]

#pragma once

#include "CorePlatformImp/Concurrency/bcMutex.h"
#include "Core/Utility/bcInitializable.h"
#include "Core/Utility/bcNullable.h"
#include "Core/Container/bcString.h"
#include "Core/Container/bcVector.h"
#include "Core/Container/bcArray.h"
#include "Core/Event/bcEvent.h"
#include "Graphic/Math/bcMatrix4f.h"
#include "GraphicImp/Device/bcDevice.h"
#include "GraphicImp/Device/bcDevicePipeline.h"
#include "GraphicImp/Device/Command/bcDeviceCommandExecuter.h"
#include "GraphicImp/Resource/Buffer/bcBuffer.h"
#include "Game/bcExport.h"
#include "Game/System/Render/bcRenderPassManager.h"
#include "Game/System/Render/bcStateConfigs.h"
#include "Game/System/Render/bcRenderState.h"
#include "Game/System/Render/bcRenderPassState.h"
#include "Game/System/Render/bcRenderInstance.h"
#include "Game/System/Render/bcRenderSystemParameter.h"
#include "Game/System/Render/Scence/bcScenceGraph.h"

namespace black_cat
{
	namespace game
	{
		struct bc_render_system_parameter
		{
			bc_render_system_parameter(bcUINT32 p_device_backbuffer_width, 
				bcUINT32 p_device_backbuffer_height, 
				graphic::bc_format p_device_backbuffer_format, 
				platform::bc_render_window& p_render_window)
				: m_device_backbuffer_width(p_device_backbuffer_width),
				m_device_backbuffer_height(p_device_backbuffer_height),
				m_device_backbuffer_format(p_device_backbuffer_format),
				m_render_window(p_render_window)
			{
			}

			bcUINT32 m_device_backbuffer_width;
			bcUINT32 m_device_backbuffer_height;
			graphic::bc_format m_device_backbuffer_format;
			platform::bc_render_window& m_render_window;
		};

		struct bc_render_system_update_param : public core_platform::bc_clock::update_param
		{
			bc_render_system_update_param(const core_platform::bc_clock::update_param& p_clock_update, const graphic::bc_matrix4f& p_view, const graphic::bc_matrix4f& p_projecttion)
				: core_platform::bc_clock::update_param(p_clock_update),
				m_view_matrix(p_view),
				m_projection_matrix(p_projecttion)
			{
			}

			graphic::bc_matrix4f m_view_matrix;
			graphic::bc_matrix4f m_projection_matrix;
		};

		class BC_GAME_DLL bc_render_system : public core::bc_initializable<bc_render_system_parameter>
		{
		private:
			friend class _bc_render_state_handle_deleter;
			friend class _bc_render_pass_state_handle_deleter;
			using render_state_entry = std::pair< core::bc_nullable< bc_render_state >, core::bc_vector_frame< bc_render_instance > >;

		public:
			using update_param = bc_render_system_update_param;

		public:
			bc_render_system() = default;

			bc_render_system(bc_render_system&&) = default;

			~bc_render_system();

			bc_render_system& operator=(bc_render_system&&) = default;

			graphic::bc_device& get_device()
			{
				return m_device;
			}

			bc_scence_graph& get_scence_graph()
			{
				return m_scence_graph;
			}

			// Thread safe function
			graphic::bc_device_pipeline_state_ptr create_device_pipeline_state(const bcCHAR* p_vertex_shader_name,
				const bcCHAR* p_hull_shader_name,
				const bcCHAR* p_domain_shader_name,
				const bcCHAR* p_geometry_shader_name,
				const bcCHAR* p_pixel_shader_name,
				bc_vertex_type p_vertex_layout,
				bc_blend_type p_blend,
				bc_depth_stencil_type p_depth_stencil,
				bc_rasterizer_type p_rasterizer,
				bcUINT p_sample_mask,
				std::initializer_list< bc_surface_format_type > p_render_target_formats,
				bc_surface_format_type p_depth_stencil_format,
				bc_multi_sample_type p_ms_config);

			// Shader parameter register indices will be reindexed based on their ordering in the array
			// Thread safe function
			bc_render_pass_state_ptr create_render_pass_state(graphic::bc_device_pipeline_state_ptr& p_pipeline_state,
				graphic::bc_viewport p_viewport,
				bc_render_pass_state_render_target_view_array&& p_shader_targets,
				graphic::bc_depth_stencil_view_ptr p_shader_depth,
				bc_render_pass_state_sampler_array&& p_shader_samplers,
				bc_render_pass_state_shader_view_array&& p_shader_views,
				bc_render_pass_state_constant_buffer_array&& p_shader_buffers);

			// Shader parameter register indices will be reindexed based on their ordering in the array
			// Thread safe function
			bc_render_state_ptr create_render_state(graphic::bc_primitive p_primitive,
				graphic::bc_buffer_ptr& p_vertex_buffer,
				bcUINT32 p_verext_buffer_offset,
				graphic::bc_buffer_ptr& p_index_buffer,
				bc_index_type p_index_type,
				bcUINT32 p_index_count,
				bcUINT32 p_index_buffer_offset,
				bc_render_state_shader_view_array&& p_shader_views,
				bc_render_state_constant_buffer_array&& p_shader_buffers);

			// Thread safe function
			void destroy_render_pass_state(bc_render_pass_state* p_render_pass_state);

			// Thread safe function
			void destroy_render_state(bc_render_state* p_render_state);

			void add_render_pass(bcUINT p_location, core::bc_unique_ptr< bc_irender_pass >&& p_pass);

			bool remove_render_pass(bcUINT p_location);

			bool remove_render_pass(core::bc_string p_name);

			// Bind render pass states to given device pipeline and apply changes
			// Apply RasterizerStage for ViewPort
			// Apply OutputputMergerStage for RenderTargets and DepthStencil
			// Apply ProgrammableStates for parameters
			void bind_render_pass_state(graphic::bc_device_pipeline* p_pipeline, bc_render_pass_state_ptr& p_render_pass_state);

			void unbind_render_pass_state(graphic::bc_device_pipeline* p_pipeline, bc_render_pass_state_ptr& p_render_pass_state);

			// Bind render states to given device pipeline and apply changes
			// Apply InputAssemberStage for primitive topology, vertex buffer and index buffer
			// Apply ProgrammableStates for parameters
			void bind_render_state(graphic::bc_device_pipeline* p_pipeline, bc_render_state_ptr& p_render_state);

			void unbind_render_state(graphic::bc_device_pipeline* p_pipeline, bc_render_state_ptr& p_render_state);

			// Add a render instance to render queue
			void add_render_instance(const bc_render_state_ptr& p_state, const bc_render_instance& p_instance);

			// Render all instances in render queue
			void render_all_instances();

			// Clear render queue. After rendering instances this function must be called
			void clear_render_instances();

			void update(update_param p_update_params);

			void initialize_frame();

			void start_frame();

			void finish_frame();

		protected:
			void _initialize(bc_render_system_parameter p_parameter) override;

			void _destroy() override;

		private:
			bool _event_handler(core::bc_ievent& p_event);

			graphic::bc_device m_device;
			graphic::bc_device_command_executer_ptr m_device_executer;
			graphic::bc_device_pipeline_ptr m_device_pipeline;

			core::bc_event_listener_handle m_window_resize_handle;
			core::bc_event_listener_handle m_device_listener_handle;
			bc_render_pass_manager m_render_pass_manager;

			core::bc_vector_movale< core::bc_nullable< bc_render_pass_state > > m_render_pass_states;
			core::bc_vector_movale< render_state_entry > m_render_states;
			core_platform::bc_mutex m_render_states_mutex;

			graphic::bc_constant_buffer_parameter m_global_cbuffer_parameter;
			graphic::bc_constant_buffer_parameter m_perobject_cbuffer_parameter;

			bc_scence_graph m_scence_graph;
		};
	}
}