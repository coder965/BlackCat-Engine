// [01/07/2017 MRB]

#pragma once

#include "PhysicsImp/Body/bcRigidDynamic.h"
#include "Game/Object/Mesh/bcSubMesh.h"
#include "Game/Object/Scene/Component/bcRigidBodyComponent.h"

namespace black_cat
{
	namespace game
	{
		class BC_GAME_DLL bc_rigid_dynamic_component : public bc_rigid_body_component
		{
			BC_COMPONENT(rigid_dynamic)

		public:
			bc_rigid_dynamic_component(bc_actor_component_index p_index) noexcept;

			bc_rigid_dynamic_component(bc_rigid_dynamic_component&&) noexcept;

			~bc_rigid_dynamic_component();

			bc_rigid_dynamic_component& operator=(bc_rigid_dynamic_component&&) noexcept;

			physics::bc_rigid_body get_body() noexcept override;

			physics::bc_rigid_dynamic get_dynamic_body() const noexcept;

			bc_actor get_actor() const noexcept override;

			void initialize(bc_actor& p_actor, const core::bc_data_driven_parameter& p_parameters) override;

			void update(const bc_actor& p_actor, core_platform::bc_clock::update_param p_clock_update_param) override;

		protected:

		private:
			physics::bc_rigid_dynamic_ref m_px_actor_ref;
		};
	}
}