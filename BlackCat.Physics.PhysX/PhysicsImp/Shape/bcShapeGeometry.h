// [12/20/2016 MRB]

#pragma once

#include "Physics/Shape/bcShapeGeometry.h"
#include "PhysicsImp/PhysicsImpPCH.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		struct bc_platform_shape_geometry_pack< g_api_physx >
		{
			explicit bc_platform_shape_geometry_pack(physx::PxGeometry& p_geometry)
				: m_px_geometry(p_geometry)
			{
			}

			physx::PxGeometry& m_px_geometry;
		};
	}
}