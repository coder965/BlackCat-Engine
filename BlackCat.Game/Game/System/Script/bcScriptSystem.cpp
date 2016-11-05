 // [10/21/2016 MRB]

#include "Game/GamePCH.h"
#include "Core/File/bcFileStream.h"
#include "Game/bcException.h"
#include "Game/System/Script/bcScriptSystem.h"
#include "Game/System/bcGameSystem.h"

namespace black_cat
{
	namespace game
	{
		bc_script_system::bc_script_system()
			: m_script_runtime(),
			m_ui_context(),
			m_stringify_function()
		{
			m_script_runtime.initialize();
			m_ui_context.initialize(m_script_runtime);
			
			{
				platform::bc_script_context::scope l_context_scope(_get_context(bc_script_context::ui));

				m_stringify_function = l_context_scope.get_context()
					.get_global()
					.get_property(L"JSON")
					.as_object()
					.get_property(L"stringify")
					.as_function< platform::bc_script_string, platform::bc_script_variable >();
			}
		}

		bc_script_system::bc_script_system(bc_script_system&& p_other) noexcept
			: m_script_runtime(std::move(p_other.m_script_runtime)),
			m_ui_context(std::move(p_other.m_ui_context)),
			m_stringify_function(std::move(p_other.m_stringify_function))
		{

		}

		bc_script_system::~bc_script_system()
		{
			{
				platform::bc_script_context::scope l_context_scope(_get_context(bc_script_context::ui));

				m_stringify_function.reset();
			}

			m_ui_context.destroy();
			m_script_runtime.destroy();
		}

		bc_script_system& bc_script_system::operator=(bc_script_system&& p_other) noexcept
		{
			m_script_runtime = std::move(p_other.m_script_runtime);
			m_ui_context = std::move(p_other.m_ui_context);
			m_stringify_function = std::move(p_other.m_stringify_function);

			return *this;
		}

		platform::bc_script_bytecode bc_script_system::load_script(bc_script_context p_context, const bcECHAR* p_script_file)
		{
			auto* l_game_system = core::bc_get_service<bc_game_system>();
			core::bc_file_stream l_script_file;

			auto l_script_file_path = l_game_system->get_file_system().get_content_script_path(p_script_file);

			l_script_file.open_read(l_script_file_path.c_str());

			core::bc_wstring_frame l_script_file_content;
			core::bc_wstring_frame l_line;
			
			while (core::bc_get_wline(l_script_file, &l_line))
			{
				l_script_file_content.append(l_line);
			}

			platform::bc_script_context::scope l_context_scope(_get_context(p_context));
			platform::bc_script_bytecode l_bytecode = m_script_runtime.compile_script(l_script_file_content.c_str());

			if (l_context_scope.get_context().has_exception())
			{
				platform::bc_script_error l_error = l_context_scope.get_context().get_clear_exception();
				auto l_error_message = core::bc_to_string_frame(l_error.error_message());
				auto l_error_file = core::bc_to_exclusive_string(p_script_file);

				throw bc_script_compile_exception(l_error_file.c_str(), 0, 0, l_error_message.c_str());
			}

			return l_bytecode;
		}

		platform::bc_script_variable bc_script_system::run_script(bc_script_context p_context, const bcWCHAR* p_script)
		{
			platform::bc_script_context::scope l_context_scope(_get_context(p_context));
			
			platform::bc_script_bytecode l_bytecode = m_script_runtime.compile_script(p_script);

			if (l_context_scope.get_context().has_exception())
			{
				platform::bc_script_error l_error = l_context_scope.get_context().get_clear_exception();
				auto l_error_message = core::bc_to_string_frame(l_error.error_message());
				auto l_error_script = core::bc_to_exclusive_string(p_script);

				throw bc_script_compile_exception(l_error_script.c_str(), 0, 0, l_error_message.c_str());
			}

			platform::bc_script_variable l_result = m_script_runtime.run_script(l_bytecode);

			if (l_context_scope.get_context().has_exception())
			{
				platform::bc_script_error l_error = l_context_scope.get_context().get_clear_exception();
				core::bc_string_frame l_error_message = core::bc_to_string_frame(l_error.error_message());

				throw bc_script_execute_exception(l_error_message.c_str());
			}

			return l_result;
		}

		platform::bc_script_variable bc_script_system::run_script(bc_script_context p_context, platform::bc_script_bytecode& p_script)
		{
			platform::bc_script_context::scope l_context_scope(_get_context(p_context));

			platform::bc_script_variable l_result = m_script_runtime.run_script(p_script);

			if (l_context_scope.get_context().has_exception())
			{
				platform::bc_script_error l_error = l_context_scope.get_context().get_clear_exception();
				core::bc_string_frame l_error_message = core::bc_to_string_frame(l_error.error_message());

				throw bc_script_execute_exception(l_error_message.c_str());
			}

			return l_result;
		}

		core::bc_wstring bc_script_system::stringify(platform::bc_script_variable& p_value)
		{
			platform::bc_script_context::scope l_context_scope(_get_context(bc_script_context::ui));

			platform::bc_script_string l_json = (*m_stringify_function)
			(
				l_context_scope.get_context().create_variable(l_context_scope.get_context().get_global()),
				p_value
			);

			return l_json.data();
		}

		void bc_script_system::interupt_script_execuation()
		{
			m_script_runtime.interupt_script_execuation();
		}

		void bc_script_system::collect_garbage()
		{
			m_script_runtime.collect_garbage();
		}

		bcSIZE bc_script_system::memory_usage() const
		{
			return m_script_runtime.memory_usage();
		}

		void bc_script_system::update(core_platform::bc_clock::update_param p_clock_update_param)
		{
		}
	}
}