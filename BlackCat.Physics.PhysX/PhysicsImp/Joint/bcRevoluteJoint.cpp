// [12/15/2016 MRB]

#include "PhysicsImp/PhysicsImpPCH.h"
#include "PhysicsImp/bcExport.h"
#include "PhysicsImp/Joint/bcRevoluteJoint.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_revolute_joint<g_api_physx>::bc_platfrom_revolute_joint() noexcept
			: bc_platform_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_revolute_joint<g_api_physx>::bc_platfrom_revolute_joint(const bc_platfrom_revolute_joint& p_other) noexcept
			: bc_platform_joint(p_other)
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_revolute_joint<g_api_physx>::~bc_platfrom_revolute_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_revolute_joint<g_api_physx>& bc_platfrom_revolute_joint<g_api_physx>::operator=(const bc_platfrom_revolute_joint& p_other) noexcept
		{
			bc_platform_joint::operator=(p_other);

			return *this;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcFLOAT bc_platfrom_revolute_joint<g_api_physx>::get_angle() const noexcept
		{
			auto* l_px_joint = static_cast< physx::PxRevoluteJoint* >
				(
					static_cast< bc_platform_physics_reference& >(const_cast< bc_platfrom_revolute_joint& >(*this)).get_platform_pack().m_px_object
					);

			return l_px_joint->getAngle();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcFLOAT bc_platfrom_revolute_joint<g_api_physx>::get_velocity() const noexcept
		{
			auto* l_px_joint = static_cast< physx::PxRevoluteJoint* >
				(
					static_cast< bc_platform_physics_reference& >(const_cast< bc_platfrom_revolute_joint& >(*this)).get_platform_pack().m_px_object
					);

			return l_px_joint->getVelocity();
		}

		template<>
		BC_PHYSICSIMP_DLL
		void bc_platfrom_revolute_joint<g_api_physx>::enable_limit(bc_joint_angular_limit& p_limit) noexcept
		{
			auto* l_px_joint = static_cast< physx::PxRevoluteJoint* >
				(
					static_cast< bc_platform_physics_reference& >(const_cast< bc_platfrom_revolute_joint& >(*this)).get_platform_pack().m_px_object
					);

			l_px_joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
			l_px_joint->setLimit(physx::PxJointAngularLimitPair(p_limit.m_lower, p_limit.m_upper, physx::PxSpring(p_limit.p_stiffness, p_limit.p_damping)));
		}

		template<>
		BC_PHYSICSIMP_DLL
		void bc_platfrom_revolute_joint<g_api_physx>::disable_limit() noexcept
		{
			auto* l_px_joint = static_cast< physx::PxRevoluteJoint* >
				(
					static_cast< bc_platform_physics_reference& >(const_cast< bc_platfrom_revolute_joint& >(*this)).get_platform_pack().m_px_object
					);

			l_px_joint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, false);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_joint_angular_limit bc_platfrom_revolute_joint<g_api_physx>::get_limit() const noexcept
		{
			auto* l_px_joint = static_cast< physx::PxRevoluteJoint* >
				(
					static_cast< bc_platform_physics_reference& >(const_cast< bc_platfrom_revolute_joint& >(*this)).get_platform_pack().m_px_object
					);
			auto l_px_limit = l_px_joint->getLimit();

			return bc_joint_angular_limit(l_px_limit.upper, l_px_limit.lower, l_px_limit.stiffness, l_px_limit.damping);
		}
	}
}