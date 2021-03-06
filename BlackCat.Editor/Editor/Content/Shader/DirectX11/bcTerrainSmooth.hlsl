// [03/03/2017 MRB]

#include "bcRegister.hlsli"

#define THREAD_GROUP_SIZE 32
#define SMOOTH_RADIUS 1
#define SMOOTH_MAX 5

// == Resource ===================================================================================

RWTexture2D<int> g_heightmap : register(BC_COMPUTE_STATE_U0);

cbuffer g_cb_parameter : register(BC_COMPUTE_STATE_CB0)
{
    uint g_width : packoffset(c0.x);
    uint g_height : packoffset(c0.y);
    uint g_chunk_size : packoffset(c0.z);
    uint g_xz_multiplier : packoffset(c0.w);
    float g_y_multiplier : packoffset(c1.x);
    uint g_distance_detail : packoffset(c1.y); // Distance from camera that render will happen with full detail
    uint g_height_detail : packoffset(c1.z); // Lower values result in higher details
};

cbuffer g_cb_parameter1 : register(BC_COMPUTE_STATE_CB1)
{
    uint g_tool_center_x : packoffset(c0.x);
    uint g_tool_center_z : packoffset(c0.y);
    uint g_tool_radius : packoffset(c0.z);
    uint g_tool_smooth : packoffset(c0.w);
};

// == Helper ======================================================================================

float get_height(uint2 p_texcoord)
{
    return g_heightmap[p_texcoord] * g_y_multiplier;
}

void set_height(uint2 p_texcoord, float p_height)
{
    g_heightmap[p_texcoord] = p_height / g_y_multiplier;
}

// == Shader =====================================================================================

[numthreads(THREAD_GROUP_SIZE, THREAD_GROUP_SIZE, 1)]
void main(uint3 p_group_id : SV_GroupID,
    uint p_group_index : SV_GroupIndex,
    uint3 p_group_thread_id : SV_GroupThreadID,
    uint3 p_dispatch_thread_id : SV_DispatchThreadID)
{
    int2 l_tool_center = int2(g_tool_center_x, g_tool_center_z);
    int2 l_circle_coord = int2(p_dispatch_thread_id.x - g_tool_radius, p_dispatch_thread_id.y - g_tool_radius);
    int2 l_global_coord = l_tool_center + l_circle_coord;
    float l_center_distance = length(l_tool_center - l_global_coord);

    if (l_center_distance > g_tool_radius)
    {
        return;
    }

    float l_heights = 0;

    uint l_min_x = max(0, l_global_coord.x - SMOOTH_RADIUS);
    uint l_min_z = max(0, l_global_coord.y - SMOOTH_RADIUS);
    uint l_max_x = min(g_width, l_global_coord.x + SMOOTH_RADIUS);
    uint l_max_z = min(g_height, l_global_coord.y + SMOOTH_RADIUS);
    uint l_num_sample = 0;

    for (int x = l_min_x; x <= l_max_x; ++x)
    {
        for (int z = l_min_z; z <= l_max_z; ++z)
        {
            uint2 l_texcoord = uint2(x, z);

            l_heights += get_height(l_texcoord);

            ++l_num_sample;
        }
    }

    l_heights /= l_num_sample;
    float l_source_height = get_height(l_global_coord);
    float l_smooth_ratio = (SMOOTH_MAX - g_tool_smooth * 1.0f) / SMOOTH_MAX;
    float l_final_height = (l_heights * (1 - l_smooth_ratio)) + (l_source_height * l_smooth_ratio);

    set_height(l_global_coord, l_final_height);
}