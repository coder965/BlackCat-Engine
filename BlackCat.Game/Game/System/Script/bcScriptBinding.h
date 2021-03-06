// [11/06/2016 MRB]

#pragma once

#include "Core/Container/bcVector.h"
#include "Game/System/Script/bcScriptSystem.h"

namespace black_cat
{
	namespace game
	{
		class bc_script_binding
		{
		private:
			using context_array_t = core::bc_vector
			<
				std::pair< std::reference_wrapper< platform::bc_script_context >, platform::bc_script_global_prototype_builder >
			>;
			friend class bc_script_system;

		public:
			bc_script_binding(bc_script_binding&& p_other) noexcept
				: m_script_system(p_other.m_script_system),
				m_contexts(std::move(p_other.m_contexts))
			{
			}

			~bc_script_binding() = default;

			bc_script_binding& operator=(bc_script_binding&& p_other) noexcept
			{
				m_contexts = std::move(p_other.m_contexts);

				return *this;
			}

			template< bc_script_context TContext, class TExtension >
			bc_script_binding& bind_type()
			{
				platform::bc_script_context& l_context = m_contexts[static_cast<bcSIZE>(TContext)].first;
				platform::bc_script_global_prototype_builder& l_global_builder = m_contexts[static_cast<bcSIZE>(TContext)].second;

				TExtension::bind(l_context, l_global_builder);

				return *this;
			}

			template< bc_script_context TContext, class TExtension >
			bc_script_binding& bind_instance(TExtension& p_instance)
			{
				platform::bc_script_context& l_context = m_contexts[static_cast<bcSIZE>(TContext)].first;
				platform::bc_script_global_prototype_builder& l_global_builder = m_contexts[static_cast<bcSIZE>(TContext)].second;

				TExtension::bind(l_context, l_global_builder, p_instance);

				return *this;
			}

		protected:

		private:
			explicit bc_script_binding(bc_script_system& p_script_system)
				: m_script_system(p_script_system),
				m_contexts()
			{
				auto& l_ui_context = p_script_system.get_ui_context().get_context();

				{
					platform::bc_script_context::scope l_ui_context_scope(l_ui_context);

					m_contexts.push_back(std::make_pair
					(
						std::ref(l_ui_context),
						l_ui_context.create_global_prototype_builder()
					));
				}
			}

			bc_script_system& m_script_system;
			context_array_t m_contexts;
		};
	}
}