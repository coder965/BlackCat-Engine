// [12/15/2016 MRB]

#pragma once

#include "PhysicsImp/Joint/bcJoint.h"

#include "Physics/Joint/bcDistanceJoint.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		struct bc_platform_distance_joint_pack<g_api_physx>
		{
		};
	}
}