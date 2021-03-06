// [11/29/2016 MRB]

#pragma once

#include "Core/Math/bcVector3f.h"
#include "Physics/bcPhysicsApi.h"

namespace black_cat
{
	namespace physics
	{
		template< bc_physics_api TApi >
		struct bc_platform_bound_box_pack
		{
		};

		template< bc_physics_api TApi >
		class bc_platform_bound_box
		{
		public:
			using platform_pack = bc_platform_bound_box_pack<TApi>;

		public:
			bc_platform_bound_box() noexcept;

			bc_platform_bound_box(const core::bc_vector3f& p_center, const core::bc_vector3f& p_half_extend) noexcept;

			bc_platform_bound_box(const bc_platform_bound_box&) noexcept;

			~bc_platform_bound_box();

			bc_platform_bound_box& operator=(const bc_platform_bound_box&) noexcept;

			core::bc_vector3f get_center() const noexcept;

			core::bc_vector3f get_extend() const noexcept;

			void expand(const core::bc_vector3f& p_point) noexcept;

			void expand(const bc_platform_bound_box& p_box) noexcept;

			bool intersect(const bc_platform_bound_box& p_box) noexcept;

			bool contains(const core::bc_vector3f& p_point) const noexcept;

			bool contain(const bc_platform_bound_box& p_box) noexcept;

			void scale(bcFLOAT p_scale) noexcept;

			platform_pack& get_platform_pack()
			{
				return m_pack;
			}
			
		protected:

		private:
			platform_pack m_pack;
		};

		using bc_bound_box = bc_platform_bound_box< g_current_physics_api >;
	}
}