// [01/18/2017 MRB]

#pragma once

#include "Core/Memory/bcPtr.h"
#include "Core/Container/bcVector.h"
#include "Core/Math/bcMatrix4f.h"
#include "PhysicsImp/Fundation/bcTransform.h"
#include "PhysicsImp/Shape/bcShapeGeometry.h"
#include "PhysicsImp/Shape/bcShapeBox.h"
#include "PhysicsImp/Shape/bcShapeSphere.h"
#include "PhysicsImp/Shape/bcShapeCapsule.h"
#include "PhysicsImp/Shape/bcShapeConvexMesh.h"
#include "PhysicsImp/Shape/bcShapeTriangleMesh.h"
#include "Game/bcExport.h"

namespace black_cat
{
	namespace game
	{
		struct bc_mesh_part_collider_entry
		{
			bc_mesh_part_collider_entry(core::bc_unique_ptr<physics::bc_shape_geometry> p_px_shape, const physics::bc_transform& p_transformation)
				: m_px_shape(std::move(p_px_shape)),
				m_transformation(p_transformation)
			{
			}

			bc_mesh_part_collider_entry(bc_mesh_part_collider_entry&&) = default;

			~bc_mesh_part_collider_entry() = default;

			bc_mesh_part_collider_entry& operator=(bc_mesh_part_collider_entry&&) = default;

			core::bc_unique_ptr<physics::bc_shape_geometry> m_px_shape;
			physics::bc_transform m_transformation;
		};

		class BC_GAME_DLL bc_mesh_part_collider
		{
		private:
			using colliders_container_t = core::bc_vector<bc_mesh_part_collider_entry>;

		public:
			using entry = bc_mesh_part_collider_entry;
			using const_iterator = colliders_container_t::const_iterator;
			using size_t = colliders_container_t::size_type;

		public:
			bc_mesh_part_collider();

			bc_mesh_part_collider(bc_mesh_part_collider&&) noexcept;

			~bc_mesh_part_collider();

			bc_mesh_part_collider& operator=(bc_mesh_part_collider&&) noexcept;

			void add_px_shape(const physics::bc_shape_box& p_box, const physics::bc_transform& p_transformation);

			void add_px_shape(const physics::bc_shape_sphere& p_sphere, const physics::bc_transform& p_transformation);

			void add_px_shape(const physics::bc_shape_capsule& p_capsule, const physics::bc_transform& p_transformation);

			void add_px_shape(physics::bc_convex_mesh_ref&& p_convex, const physics::bc_transform& p_transformation);

			void add_px_shape(physics::bc_triangle_mesh_ref&& p_mesh, const physics::bc_transform& p_transformation);

			const_iterator cbegin() const;

			const_iterator cend() const;

			size_t size() const;

			void shrink_to_fit();

		protected:
			
		private:
			core::bc_vector<bc_mesh_part_collider_entry> m_shapes;
			core::bc_vector<physics::bc_convex_mesh_ref> m_convexs;
			core::bc_vector<physics::bc_triangle_mesh_ref> m_triangles;
		};
	}
}