// [12/13/2016 MRB]

#include "PhysicsImp/PhysicsImpPCH.h"
#include "PhysicsImp/bcExport.h"
#include "PhysicsImp/Fundation/bcTransform.h"
#include "PhysicsImp/bcUtility.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>::bc_platform_transform() noexcept
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>::bc_platform_transform(const core::bc_vector3f& p_position) noexcept
		{
			m_pack.m_px_transform = physx::PxTransform(bc_to_right_hand(p_position));
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform< g_api_physx >::bc_platform_transform(const core::bc_matrix4f& p_rotation) noexcept
		{
			m_pack.m_px_transform = physx::PxTransform(bc_to_right_hand(p_rotation));
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>::bc_platform_transform(const core::bc_vector3f& p_position, const core::bc_matrix3f& p_rotation) noexcept
		{
			m_pack.m_px_transform = physx::PxTransform
			(
				bc_to_right_hand(p_position),
				physx::PxQuat(bc_to_right_hand(p_rotation))
			);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>::bc_platform_transform(const bc_platform_transform& p_other) noexcept
		{
			m_pack.m_px_transform = p_other.m_pack.m_px_transform;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>::~bc_platform_transform()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>& bc_platform_transform<g_api_physx>::operator=(const bc_platform_transform& p_other) noexcept
		{
			m_pack.m_px_transform = p_other.m_pack.m_px_transform;

			return *this;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx> bc_platform_transform<g_api_physx>::operator*(const bc_platform_transform& p_other) noexcept
		{
			bc_platform_transform l_result;

			l_result.m_pack.m_px_transform = m_pack.m_px_transform * p_other.m_pack.m_px_transform;

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx>& bc_platform_transform<g_api_physx>::operator*=(const bc_platform_transform& p_other) noexcept
		{
			m_pack.m_px_transform * p_other.m_pack.m_px_transform;

			return *this;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx> bc_platform_transform<g_api_physx>::get_inverse() const noexcept
		{
			bc_platform_transform l_result;

			l_result.m_pack.m_px_transform = m_pack.m_px_transform.getInverse();

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx> bc_platform_transform<g_api_physx>::get_normalized() const noexcept
		{
			bc_platform_transform l_result;

			l_result.m_pack.m_px_transform = m_pack.m_px_transform.getNormalized();

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_vector3f bc_platform_transform<g_api_physx>::transform(const core::bc_vector3f& p_vector) const noexcept
		{
			auto l_px_vec3 = m_pack.m_px_transform.transform(bc_to_right_hand(p_vector));

			return bc_to_game_hand(l_px_vec3);
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_vector3f bc_platform_transform<g_api_physx>::transform_inverse(const core::bc_vector3f& p_vector) const noexcept
		{
			auto l_px_vec3 = m_pack.m_px_transform.transformInv(bc_to_right_hand(p_vector));

			return bc_to_game_hand(l_px_vec3);
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_vector3f bc_platform_transform<g_api_physx>::rotate(const core::bc_vector3f& p_vector) const noexcept
		{
			auto l_px_vec3 = m_pack.m_px_transform.rotate(bc_to_right_hand(p_vector));

			return bc_to_game_hand(l_px_vec3);
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_vector3f bc_platform_transform<g_api_physx>::rotate_inverse(const core::bc_vector3f& p_vector) const noexcept
		{
			auto l_px_vec3 = m_pack.m_px_transform.rotateInv(bc_to_right_hand(p_vector));

			return bc_to_game_hand(l_px_vec3);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bool bc_platform_transform<g_api_physx>::is_finite() const noexcept
		{
			return m_pack.m_px_transform.isFinite();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bool bc_platform_transform<g_api_physx>::is_valid() const noexcept
		{
			return m_pack.m_px_transform.isValid();
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_vector3f bc_platform_transform< g_api_physx >::get_position() const noexcept
		{
			return bc_to_game_hand(m_pack.m_px_transform.p);
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_matrix3f bc_platform_transform< g_api_physx >::get_matrix3() const noexcept
		{
			physx::PxMat33 l_mat(m_pack.m_px_transform.q);

			return bc_to_game_hand(l_mat);
		}

		template<>
		BC_PHYSICSIMP_DLL
		core::bc_matrix4f bc_platform_transform< g_api_physx >::get_matrix4() const noexcept
		{
			physx::PxMat44 l_mat(m_pack.m_px_transform);

			return bc_to_game_hand(l_mat);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_transform<g_api_physx> bc_platform_transform<g_api_physx>::identity() noexcept
		{
			bc_platform_transform l_result;

			l_result.m_pack.m_px_transform = physx::PxTransform::createIdentity();

			return l_result;
		}
	}
}