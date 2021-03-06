// [12/15/2016 MRB]

#include "PhysicsImp/PhysicsImpPCH.h"
#include "PhysicsImp/bcExport.h"
#include "PhysicsImp/Joint/bcSphericalJoint.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_spherical_joint< g_api_physx >::bc_platfrom_spherical_joint() noexcept
			: bc_platform_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_spherical_joint< g_api_physx >::bc_platfrom_spherical_joint(const bc_platfrom_spherical_joint& p_other) noexcept
			: bc_platform_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_spherical_joint< g_api_physx >::~bc_platfrom_spherical_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_spherical_joint< g_api_physx >& bc_platfrom_spherical_joint< g_api_physx >::operator=(const bc_platfrom_spherical_joint& p_other) noexcept
		{
			bc_platform_joint::operator=(p_other);

			return *this;
		}

		template<>
		BC_PHYSICSIMP_DLL
		void bc_platfrom_spherical_joint< g_api_physx >::enable_limit(bc_joint_cone_limit& p_limit) noexcept
		{
			auto* l_px_joint = static_cast< physx::PxSphericalJoint* >
			(
				static_cast< bc_platform_physics_reference& >(*this).get_platform_pack().m_px_object
			);

			l_px_joint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
			l_px_joint->setLimitCone(physx::PxJointLimitCone(p_limit.m_y_angle, p_limit.m_z_angle, physx::PxSpring(p_limit.p_stiffness, p_limit.p_damping)));
		}

		template<>
		BC_PHYSICSIMP_DLL
		void bc_platfrom_spherical_joint< g_api_physx >::disable_limit() noexcept
		{
			auto* l_px_joint = static_cast< physx::PxSphericalJoint* >
			(
				static_cast< bc_platform_physics_reference& >(*this).get_platform_pack().m_px_object
			);

			l_px_joint->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, false);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_joint_cone_limit bc_platfrom_spherical_joint< g_api_physx >::get_limit() const noexcept
		{
			auto* l_px_joint = static_cast< physx::PxSphericalJoint* >
			(
				static_cast< bc_platform_physics_reference& >(const_cast< bc_platfrom_spherical_joint& >(*this)).get_platform_pack().m_px_object
			);
			auto l_px_limit = l_px_joint->getLimitCone();

			return bc_joint_cone_limit(l_px_limit.yAngle, l_px_limit.zAngle, l_px_limit.stiffness, l_px_limit.damping);
		}
	}
}