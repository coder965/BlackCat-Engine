 // [10/24/2016 MRB]

#pragma once

#include "CorePlatformImp/Concurrency/bcMutex.h"
#include "Core/Container/bcString.h"
#include "Core/Concurrency/bcThreadManager.h"
#include "Core/Utility/bcNullable.h"
#include "PlatformImp/Application/bcConsoleWindow.h"
#include "Game/bcExport.h"
#include "Game/Application/bcRenderApplication.h"
#include "Game/System/Script/bcGameConsole.h"

namespace black_cat
{
	namespace game
	{
		class BC_GAME_DLL bc_default_game_console : public bc_igame_console_imp
		{
		public:
			bc_default_game_console(bc_render_application& p_application, bc_game_console& p_game_console) noexcept;

			bc_default_game_console(bc_default_game_console&&) noexcept;

			~bc_default_game_console();

			bc_default_game_console& operator=(bc_default_game_console&&) noexcept;

			void write_output(bc_console_output_type p_type, const core::bc_estring& p_msg) override;

			void clear_output() override;

			void show() override;

			void hide() override;

			bool is_visibile() override;

			void update(core_platform::bc_clock::update_param p_clock_update_param) override;

		protected:

		private:
			void _input_spin();

			bc_render_application& m_application;
			core::bc_nullable< platform::bc_console_window > m_console;
			core_platform::bc_mutex m_console_mutex;
			core::bc_task< void > m_input_spin_task;
			core::bc_string m_input_line;
			core_platform::bc_mutex m_input_mutex;
		};
	}
}