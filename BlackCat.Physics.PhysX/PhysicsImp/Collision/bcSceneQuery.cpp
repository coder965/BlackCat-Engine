// [12/16/2016 MRB]

#include "PhysicsImp/PhysicsImpPCH.h"
#include "PhysicsImp/bcExport.h"
#include "PhysicsImp/Collision/bcSceneQuery.h"

namespace black_cat
{
	namespace physics
	{
		/*template<>
		BC_PHYSICSIMP_DLL
		bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::bc_platform_scene_query_buffer(bcUINT32 p_touching_hit_count)
		{
			m_pack.m_touches.reserve(p_touching_hit_count);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::bc_platform_scene_query_buffer(const bc_platform_scene_query_buffer& p_other) noexcept
		{
			m_pack.m_px_query = p_other.m_pack.m_px_query;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::~bc_platform_scene_query_buffer()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>& bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::operator=(const bc_platform_scene_query_buffer& p_other) noexcept
		{
			m_pack.m_px_query = p_other.m_pack.m_px_query;

			return *this;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bool bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::has_block() const noexcept
		{
			return m_pack.m_px_query.hasBlock;
		}

		template<>
		BC_PHYSICSIMP_DLL
		const bc_platform_ray_hit<g_api_physx>* bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::get_block() const noexcept
		{
			m_pack.m_block.get_platform_pack().m_px_hit = m_pack.m_px_query.block;

			return &m_pack.m_block;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::get_touch_count() const noexcept
		{
			return m_pack.m_px_query.nbTouches;
		}

		template<>
		BC_PHYSICSIMP_DLL
		const bc_platform_ray_hit<g_api_physx>* bc_platform_scene_query_buffer<g_api_physx, bc_platform_ray_hit<g_api_physx>>::get_touches() const noexcept
		{
			if(m_pack.m_touches.size() == 0)
			{
				m_pack.m_touches.resize(m_pack.m_touches.capacity());

				for (bcUINT32 i = 0; i < m_pack.m_touches.size(); ++i)
				{
					m_pack.m_touches[i].get_platform_pack().m_px_hit = m_pack.m_px_query.touches[i];
				}
			}

			return m_pack.m_touches.data();
		}*/
	}
}