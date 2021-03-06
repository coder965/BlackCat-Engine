// [01/07/2017 MRB]

#pragma once

#include "PhysicsImp/Body/bcRigidBody.h"
#include "Game/Object/Mesh/bcMeshCollider.h"
#include "Game/Object/Scene/bcActorComponent.h"

namespace black_cat
{
	namespace game
	{
		class bc_physics_system;
		struct bc_mesh_collider_shape;
		class bc_mesh_node;
		class bc_sub_mesh;
		class bc_mesh_component;

		class bc_rigid_body_component : public bc_iactor_component
		{
			BC_ABSTRACT_COMPONENT(rigid_body)

		public:
			virtual ~bc_rigid_body_component();

			virtual physics::bc_rigid_body get_body() noexcept = 0;

		protected:
			explicit bc_rigid_body_component(bc_actor_component_index p_index) noexcept;

			bc_rigid_body_component(bc_rigid_body_component&&) noexcept;

			bc_rigid_body_component& operator=(bc_rigid_body_component&&) noexcept;

			void initialize_from_mesh(bc_physics_system& p_physics_system, 
				bc_actor& p_actor, 
				physics::bc_rigid_body& p_px_actor, 
				bc_mesh_component& p_mesh_component);

		private:
			void _get_mesh_colliders(const bc_sub_mesh& p_mesh, 
				const bc_mesh_node* p_mesh_node, 
				core::bc_vector_frame<const bc_mesh_part_collider::entry*>& p_result);
		};

		inline bc_rigid_body_component::~bc_rigid_body_component() = default;

		inline bc_rigid_body_component::bc_rigid_body_component(bc_actor_component_index p_index) noexcept
			: bc_iactor_component(p_index)
		{
		}

		inline bc_rigid_body_component::bc_rigid_body_component(bc_rigid_body_component&&) noexcept = default;

		inline bc_rigid_body_component& bc_rigid_body_component::operator=(bc_rigid_body_component&&) noexcept = default;
	}
}