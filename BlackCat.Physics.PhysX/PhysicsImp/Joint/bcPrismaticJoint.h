// [12/15/2016 MRB]

#pragma once

#include "PhysicsImp/Joint/bcJoint.h"

#include "Physics/Joint/bcPrismaticJoint.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		struct bc_platform_prismatic_joint_pack<g_api_physx>
		{
		};
	}
}