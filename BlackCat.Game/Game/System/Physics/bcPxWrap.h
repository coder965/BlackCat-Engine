// [02/21/2017 MRB]

#pragma once

#include "Core/Utility/bcEnumOperand.h"
#include "Physics/Collision/bcQueryGroup.h"
#include "PhysicsImp/Collision/bcShapeQuery.h"
#include "PhysicsImp/Body/bcRigidActor.h"
#include "Game/System/bcGameSystem.h"
#include "Game/Object/Scene/bcActor.hpp"

namespace black_cat
{
	namespace game
	{
		enum class bc_query_group : core::bc_enum::enum_t< physics::bc_query_group >
		{
			terrain = static_cast< core::bc_enum::enum_t< physics::bc_query_group > >(physics::bc_query_group::group1)
		};

		class bc_ray_hit : public physics::bc_ray_hit
		{
		public:
			bc_ray_hit(const physics::bc_ray_hit& p_hit)
				: physics::bc_ray_hit(p_hit)
			{	
			}

			bc_ray_hit(const bc_ray_hit&) = default;

			~bc_ray_hit() = default;

			bc_ray_hit& operator=(const bc_ray_hit&) = default;

			physics::bc_rigid_actor get_px_actor() const noexcept
			{
				return physics::bc_ray_hit::get_actor();
			}

			bc_actor get_actor() const noexcept
			{
				auto l_px_actor = get_px_actor();
				return core::bc_get_service<bc_game_system>()->get_physics_system().get_game_actor(l_px_actor);
			}

		protected:

		private:
		};

		class bc_overlap_hit : public physics::bc_overlap_hit
		{
		public:
			bc_overlap_hit(const physics::bc_overlap_hit& p_hit)
				: physics::bc_overlap_hit(p_hit)
			{
			}

			bc_overlap_hit(const bc_overlap_hit&) = default;

			~bc_overlap_hit() = default;

			bc_overlap_hit& operator=(const bc_overlap_hit&) = default;

			physics::bc_rigid_actor get_px_actor() const noexcept
			{
				return physics::bc_overlap_hit::get_actor();
			}

			bc_actor get_actor() const noexcept
			{
				auto l_px_actor = get_px_actor();
				return core::bc_get_service<bc_game_system>()->get_physics_system().get_game_actor(l_px_actor);
			}

		protected:

		private:
		};

		class bc_sweep_hit : public physics::bc_sweep_hit
		{
		public:
			bc_sweep_hit(const physics::bc_sweep_hit& p_hit)
				: physics::bc_sweep_hit(p_hit)
			{
			}

			bc_sweep_hit(const bc_sweep_hit&) = default;

			~bc_sweep_hit() = default;

			bc_sweep_hit& operator=(const bc_sweep_hit&) = default;

			physics::bc_rigid_actor get_px_actor() const noexcept
			{
				return physics::bc_sweep_hit::get_actor();
			}

			bc_actor get_actor() const noexcept
			{
				auto l_px_actor = get_px_actor();
				return core::bc_get_service<bc_game_system>()->get_physics_system().get_game_actor(l_px_actor);
			}

		protected:

		private:
		};
	}
}