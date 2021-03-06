// [05/31/2016 MRB]

#pragma once

#include "CorePlatform/bcType.h"
#include "Core/Container/bcString.h"

namespace black_cat
{
	namespace core
	{

#define BC_PARAMETER_RESOLVER(p_name, p_value)	constexpr const bcCHAR* g_param_rsv_##p_name = p_value

#define BC_PARAMETER(p_name, p_value)			constexpr const bcCHAR* g_param_##p_name = p_value

#define BC_SERVICE_NAME(p_name)					"srv_" ## #p_name

#define BC_CONTENT_NAME(p_name)					"cnt_" ## #p_name

#define BC_COMPONENT_NAME(p_name)				"cmp_" ## #p_name

#define BC_EVENT_NAME(p_name)					"evt_" ## #p_name

#define BC_RENDER_PASS_NAME(p_name)				"rpass_" ## #p_name

#define BC_SERVICE(p_name) \
	public: \
	static constexpr const bcCHAR* service_name() \
	{ \
		return BC_SERVICE_NAME(p_name); \
	} \
	static constexpr bcUINT32 service_hash() \
	{ \
		return bc_compile_time_string_hash(BC_SERVICE_NAME(p_name)); \
	} \

#define BC_CONTENT(p_name) \
	public: \
	static constexpr const bcCHAR* content_name() \
	{ \
		return BC_CONTENT_NAME(p_name); \
	} \
	static constexpr bcUINT32 content_hash() \
	{ \
		return bc_compile_time_string_hash(BC_CONTENT_NAME(p_name)); \
	} \

#define BC_COMPONENT(p_name) \
	public: \
	static constexpr bool component_is_abstract() \
	{ \
		return false; \
	} \
	static constexpr const bcCHAR* component_name() \
	{ \
		return BC_COMPONENT_NAME(p_name); \
	} \
	static constexpr bcUINT32 component_hash() \
	{ \
		return bc_compile_time_string_hash(BC_COMPONENT_NAME(p_name)); \
	} \

#define BC_ABSTRACT_COMPONENT(p_name) \
	public: \
	static constexpr bool component_is_abstract() \
	{ \
		return true; \
	} \
	static constexpr const bcCHAR* component_name() \
	{ \
		return BC_COMPONENT_NAME(p_name); \
	} \
	static constexpr bcUINT32 component_hash() \
	{ \
		return bc_compile_time_string_hash(BC_COMPONENT_NAME(p_name)); \
	} \

#define BC_EVENT(p_name) \
	public: \
	static constexpr const bcCHAR* event_name() \
	{ \
		return BC_EVENT_NAME(p_name); \
	} \
	static constexpr bcUINT32 event_hash() \
	{ \
		return bc_compile_time_string_hash(BC_EVENT_NAME(p_name)); \
	} \

#define BC_RENDER_PASS(p_name) \
	public: \
	static constexpr const bcCHAR* render_pass_name() \
	{ \
		return BC_RENDER_PASS_NAME(p_name); \
	} \
	static constexpr bcUINT32 render_pass_hash() \
	{ \
		return bc_compile_time_string_hash(BC_RENDER_PASS_NAME(p_name)); \
	} \

		BC_PARAMETER(device, "device");
		BC_PARAMETER(shader_function, "function");
		BC_PARAMETER(mesh, "mesh");
		BC_PARAMETER(sub_mesh, "sub_mesh");
		BC_PARAMETER(child_actor, "childs");
		BC_PARAMETER(heightmap, "heightmap");
		BC_PARAMETER(texture_config, "config");

		BC_PARAMETER_RESOLVER(mesh, "get_mesh");
		BC_PARAMETER_RESOLVER(heightmap, "get_heightmap");
	}
}