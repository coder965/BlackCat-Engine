// [08/30/2016 MRB]

#include "BlackCat/BlackCatPCH.h"

#include "Core/Container/bcVector.h"
#include "Core/File/bcContentStreamManager.h"
#include "GraphicImp/bcRenderApiInfo.h"
#include "GraphicImp/Resource/Texture/bcTexture2d.h"
#include "GraphicImp/Resource/bcResourceConfig.h"
#include "Game/System/Render/bcVertexLayout.h"
#include "Game/Object/Mesh/bcHeightMap.h"
#include "Game/Object/Scene/bcScene.h"
#include "Game/Object/Scene/Component/bcHeightMapComponent.h"
#include "BlackCat/bcException.h"
#include "BlackCat/RenderPass/bcTerrainPassDx11.h"
#include "BlackCat/Loader/bcHeightMapLoaderDx11.h"

namespace black_cat
{
	core::bc_vector4f _plane_from_3_point(const core::bc_vector3f& p_first,
		const core::bc_vector3f& p_second,
		const core::bc_vector3f& p_third)
	{
		auto l_vector1 = p_second - p_first;
		auto l_vector2 = p_third - p_first;

		auto l_normal = l_vector1.cross(l_vector2);
		l_normal.normalize();
		auto l_distance = l_normal.dot(p_first);

		return core::bc_vector4f(l_normal, -l_distance);
	}

	struct _bc_parameter_buffer
	{
		BC_CBUFFER_ALIGN
		core::bc_vector4f m_frustum_planes[6];
	};

	void bc_terrain_pass_dx11::initialize_resources(game::bc_render_system& p_render_system)
	{
		graphic::bc_device& l_device = p_render_system.get_device();
		graphic::bc_texture2d l_back_buffer_texture = l_device.get_back_buffer_texture();

		m_command_list = l_device.create_command_list();
		m_pipeline_state = p_render_system.create_device_pipeline_state
		(
			"terrain_vs",
			"terrain_hs",
			"terrain_ds",
			nullptr,
			"terrain_ps",
			game::bc_vertex_type::pos_tex,
			game::bc_blend_type::opaque,
			game::bc_depth_stencil_type::depth_less_stencil_off,
			game::bc_rasterizer_type::fill_solid_cull_back,
			0x1,
			{ l_back_buffer_texture.get_format() },
			game::bc_surface_format_type::D32_FLOAT,
			game::bc_multi_sample_type::c1_q1
		);

		auto l_resource_configure = graphic::bc_graphic_resource_configure();

		auto l_parameter_cbuffer_config = l_resource_configure
			.as_resource()
			.as_buffer
			(
				1,
				sizeof(_bc_parameter_buffer),
				graphic::bc_resource_usage::gpu_rw
			)
			.as_constant_buffer();

		m_parameter_cbuffer = l_device.create_buffer(l_parameter_cbuffer_config, nullptr);

		graphic::bc_device_parameters l_old_parameters
		(
			0,
			0,
			graphic::bc_format::unknown,
			graphic::bc_texture_ms_config(1, 0)
		);
		graphic::bc_device_parameters l_new_parameters
		(
			l_back_buffer_texture.get_width(),
			l_back_buffer_texture.get_height(),
			l_back_buffer_texture.get_format(),
			l_back_buffer_texture.get_sample_count()
		);

		after_reset(p_render_system, l_device, l_old_parameters, l_new_parameters);

		m_device_compute_state = p_render_system.create_device_compute_state("terrain_chunk_info");
		m_run_chunk_info_shader = true;
	}

	void bc_terrain_pass_dx11::update(const game::bc_render_system_update_param& p_update_param)
	{
		m_camera_extends = p_update_param.m_camera_extends;
	}

	void bc_terrain_pass_dx11::initialize_frame(game::bc_render_system& p_render_system, game::bc_scene& p_scene, game::bc_render_thread& p_thread)
	{
		if (m_run_chunk_info_shader)
		{
			p_thread.start(m_command_list.get());

			auto l_heightmaps = p_scene.get_heightmaps();

			for (auto& l_actor : l_heightmaps)
			{
				const game::bc_height_map* l_heightmap = l_actor.get_component< game::bc_height_map_component >()->get_height_map();
				const bc_height_map_dx11* l_heightmap_dx11 = static_cast<const bc_height_map_dx11*>(l_heightmap);

				auto l_compute_state = p_render_system.create_compute_state
				(
					m_device_compute_state.get(),
					l_heightmap_dx11->get_width() / s_shader_thread_group_size,
					l_heightmap_dx11->get_height() / s_shader_thread_group_size,
					1,
					{},
					{ graphic::bc_resource_view_parameter(0, graphic::bc_shader_type::compute, l_heightmap_dx11->get_height_map_view()) },
					{ graphic::bc_resource_view_parameter(0, graphic::bc_shader_type::compute, l_heightmap_dx11->get_chunk_info_unordered_view()) },
					{ graphic::bc_constant_buffer_parameter(0, graphic::bc_shader_type::compute, l_heightmap_dx11->get_parameter_cbuffer()) }
				);

				p_thread.run_compute_shader(l_compute_state.get());
			}

			p_thread.finish();
			m_run_chunk_info_shader = false;
		}

		p_thread.start(m_command_list.get());

		_bc_parameter_buffer l_parameter;
		l_parameter.m_frustum_planes[0] = _plane_from_3_point(m_camera_extends[0], m_camera_extends[1], m_camera_extends[2]);
		l_parameter.m_frustum_planes[1] = _plane_from_3_point(m_camera_extends[6], m_camera_extends[5], m_camera_extends[4]);
		l_parameter.m_frustum_planes[2] = _plane_from_3_point(m_camera_extends[0], m_camera_extends[4], m_camera_extends[5]);
		l_parameter.m_frustum_planes[3] = _plane_from_3_point(m_camera_extends[1], m_camera_extends[5], m_camera_extends[6]);
		l_parameter.m_frustum_planes[4] = _plane_from_3_point(m_camera_extends[2], m_camera_extends[6], m_camera_extends[7]);
		l_parameter.m_frustum_planes[5] = _plane_from_3_point(m_camera_extends[7], m_camera_extends[4], m_camera_extends[0]);

		p_thread.update_subresource(m_parameter_cbuffer.get(), 0, &l_parameter, 0, 0);
		p_thread.bind_render_pass_state(m_render_pass_state.get());
		p_thread.clear_buffers(core::bc_vector4f(0, 0, 255, 0), 1, 0);
	}

	void bc_terrain_pass_dx11::execute(game::bc_render_system& p_render_system, game::bc_scene& p_scene, game::bc_render_thread& p_thread)
	{
		p_scene.render_heightmaps(p_render_system, p_thread);

		p_thread.unbind_render_pass_state(m_render_pass_state.get());
		p_thread.finish();

		m_command_list->reset();
	}

	void bc_terrain_pass_dx11::before_reset(game::bc_render_system& p_render_system, graphic::bc_device& p_device, graphic::bc_device_parameters& p_old_parameters, graphic::bc_device_parameters& p_new_parameters)
	{
		if
		(
			p_old_parameters.m_width != p_new_parameters.m_width ||
			p_old_parameters.m_height != p_new_parameters.m_height
		)
		{
			m_render_pass_state.reset();
		}
	}

	void bc_terrain_pass_dx11::after_reset(game::bc_render_system& p_render_system, graphic::bc_device& p_device, graphic::bc_device_parameters& p_old_parameters, graphic::bc_device_parameters& p_new_parameters)
	{
		if
		(
			p_old_parameters.m_width != p_new_parameters.m_width ||
			p_old_parameters.m_height != p_new_parameters.m_height
		)
		{
			graphic::bc_texture2d l_back_buffer_texture = p_device.get_back_buffer_texture();

			auto l_texturemap_sampler_config = game::bc_graphic_state_configs::bc_sampler_config(game::bc_sampler_type::filter_linear_linear_linear_address_clamp_clamp_clamp);
			auto l_texture_sampler_config = game::bc_graphic_state_configs::bc_sampler_config(game::bc_sampler_type::filter_linear_linear_linear_address_wrap_wrap_wrap);

			auto l_depth_stencil_view = *get_shared_resource< graphic::bc_depth_stencil_view >(game::bc_render_pass_resource_variable::depth_stencil_view);
			auto l_render_target_view = *get_shared_resource< graphic::bc_render_target_view >(game::bc_render_pass_resource_variable::render_target_view);
			auto l_viewport = graphic::bc_viewport::default_config(l_back_buffer_texture.get_width(), l_back_buffer_texture.get_height());
			auto l_texturemap_sampler = p_device.create_sampler_state(l_texturemap_sampler_config);
			auto l_texture_sampler = p_device.create_sampler_state(l_texture_sampler_config);

			m_render_pass_state = p_render_system.create_render_pass_state
			(
				m_pipeline_state.get(),
				l_viewport,
				{
					graphic::bc_render_target_view_ptr(l_render_target_view)
				},
				l_depth_stencil_view,
				{
					graphic::bc_sampler_parameter(0, core::bc_enum::or({ graphic::bc_shader_type::pixel }), l_texturemap_sampler),
					graphic::bc_sampler_parameter(0, core::bc_enum::or({ graphic::bc_shader_type::pixel }), l_texture_sampler)
				},
				{},
				{
					graphic::bc_constant_buffer_parameter(0, graphic::bc_shader_type::hull, m_parameter_cbuffer)
				}
			);
		}
	}

	void bc_terrain_pass_dx11::destroy(graphic::bc_device& p_device)
	{
		m_device_compute_state.reset();
		m_render_pass_state.reset();
		m_parameter_cbuffer.reset();
		m_pipeline_state.reset();
		m_command_list.reset();
	}

	void bc_terrain_pass_dx11::update_chunk_infoes()
	{
		m_run_chunk_info_shader = true;
	}
}