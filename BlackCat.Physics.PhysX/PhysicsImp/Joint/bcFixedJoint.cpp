// [12/15/2016 MRB]

#include "PhysicsImp/PhysicsImpPCH.h"
#include "PhysicsImp/bcExport.h"
#include "PhysicsImp/Joint/bcFixedJoint.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_fixed_joint<g_api_physx>::bc_platfrom_fixed_joint() noexcept
			: bc_platform_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_fixed_joint<g_api_physx>::bc_platfrom_fixed_joint(const bc_platfrom_fixed_joint& p_other) noexcept
			: bc_platform_joint(p_other)
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_fixed_joint<g_api_physx>::~bc_platfrom_fixed_joint()
		{
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platfrom_fixed_joint<g_api_physx>& bc_platfrom_fixed_joint<g_api_physx>::operator=(const bc_platfrom_fixed_joint& p_other) noexcept
		{
			bc_platform_joint::operator=(p_other);

			return *this;
		}
	}
}