// [12/09/2016 MRB]

#pragma once

#include "Physics/bcPhysicsApi.h"
#include "Physics/bcPhysicsReference.h"

namespace black_cat
{
	namespace physics
	{
		template< bc_physics_api TApi, class TObject >
		struct bc_platform_physics_ref_pack
		{
		};

		/**
		 * \brief Store a strong reference to physics reference objects
		 * \tparam TApi 
		 * \tparam TObject 
		 */
		template< bc_physics_api TApi, class TObject >
		class bc_platform_physics_ref
		{
		public:
			using type = TObject;
			using platform_pack = bc_platform_physics_ref_pack<TApi, TObject>;

		public:
			bc_platform_physics_ref();

			template
			<
				typename = typename std::enable_if
				<
					std::is_base_of_v< bc_platform_physics_reference< TApi >, std::decay_t< type > >
				>::type
			>
			explicit bc_platform_physics_ref(const type& p_object);

			bc_platform_physics_ref(bc_platform_physics_ref&& p_other) noexcept;

			~bc_platform_physics_ref();

			bc_platform_physics_ref& operator=(bc_platform_physics_ref&& p_other) noexcept;

			type* operator->() noexcept;

			type& operator*();

			type& get();

			const type& get() const;

			void reset();

			void reset(const type& p_object);

			platform_pack& get_platform_pack()
			{
				return m_pack;
			}

		protected:

		private:
			platform_pack m_pack;
		};

		template< typename TObject >
		using bc_physics_ref = bc_platform_physics_ref< g_current_physics_api, TObject >;
	}
}