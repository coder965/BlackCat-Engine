// [04/27/2016 MRB]

#pragma once

#include "CorePlatform/bcType.h"
#include "CorePlatformImp/Utility/bcClock.h"
#include "Core/bcConstant.h"
#include "Core/Utility/bcDataDrivenParameter.h"
#include "Game/bcExport.h"
#include "Game/Object/Scene/bcActor.h"

namespace black_cat
{
	namespace game
	{
		class bc_actor_component_manager;
		using bc_actor_component_hash = bcUINT32;
		using bc_actor_component_index = bcINT32;
		constexpr bc_actor_component_index g_actor_component_invalid_index = bc_actor_component_index(-1);

		class BC_GAME_DLL bc_iactor_component
		{
		private:
			friend class bc_actor_component_manager;

		public:
			virtual ~bc_iactor_component();

			virtual bc_actor get_actor() const noexcept = 0;

			/**
			 * \brief Initialize component data members that are shared between instances of this component.
			 * In other word data memebers that act like entity template datas.
			 * \param p_parameters 
			 */
			virtual void initialize(bc_actor& p_actor, const core::bc_data_driven_parameter& p_parameters) = 0;

			virtual void update(const bc_actor& p_actor, core_platform::bc_clock::update_param p_clock_update_param) = 0;

		protected:
			explicit bc_iactor_component(bc_actor_component_index p_index) noexcept;

			bc_iactor_component(bc_iactor_component&& p_other) noexcept;

			bc_iactor_component& operator=(bc_iactor_component&& p_other) noexcept;

			bc_actor_component_index get_index() const noexcept;

			static bc_actor_component_manager* _get_manager() noexcept;

		private:
			bc_actor_component_index m_index;
		};

		inline bc_iactor_component::~bc_iactor_component() = default;

		inline bc_iactor_component::bc_iactor_component(bc_actor_component_index p_index) noexcept
			: m_index(p_index)
		{
		}

		inline bc_iactor_component::bc_iactor_component(bc_iactor_component&& p_other) noexcept
			: m_index(p_other.m_index)
		{
		}

		inline bc_iactor_component& bc_iactor_component::operator=(bc_iactor_component&& p_other) noexcept
		{
			m_index = p_other.m_index;

			return *this;
		}

		inline bc_actor_component_index bc_iactor_component::get_index() const noexcept
		{
			return m_index;
		}
	}
}