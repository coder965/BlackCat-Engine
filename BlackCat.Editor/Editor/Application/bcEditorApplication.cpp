// [11/12/2016 MRB]

#include "Editor/EditorPCH.h"
#include "Editor/Application/bcEditorApplication.h"
#include "Editor/Application/bcEditorRenderApplication.h"

namespace black_cat
{
	namespace editor
	{
		void bc_render_app_thread::run()
		{
			core_platform::bc_basic_hardware_info l_hardware_info;
			core_platform::bc_hardware_info::get_basic_info(&l_hardware_info);

			platform::bc_application_parameter l_app_parameters
			(
				m_instance,
				bcL("blackcat_editor_app"),
				m_cmd_line
			);
			game::bc_render_application_parameter l_render_app_parameters
			(
				l_app_parameters,
				m_output_window
			);
			game::bc_engine_component_parameter l_engine_component_parameters
			(
				32,
				20,
				BC_MEMORY_MIN_ALIGN,
				64,
				static_cast< bcUINT32 >(core::bc_mem_size::mb) * 25,
				static_cast< bcUINT32 >(core::bc_mem_size::mb) * 25,
				static_cast< bcUINT32 >(core::bc_mem_size::mb) * 15,
				static_cast< bcUINT32 >(core::bc_mem_size::mb) * 100,
				l_hardware_info.proccessor_count / 4,
				l_hardware_info.proccessor_count / 2
			);
			game::bc_engine_application_parameter l_engine_app_parameters
			(
				l_engine_component_parameters,
				l_render_app_parameters
			);

			bc_editor_render_app::start_up(l_engine_app_parameters);
			bc_editor_render_app::get().set_fps(60);

			m_initialized.store(1);

			bcINT32 l_code = bc_editor_render_app::get().run();
			bc_editor_render_app::close();

			m_result_code.store(l_code);
		}

		bc_editor_app::bc_editor_app(HINSTANCE p_instance, QWidget *parent)
			: QMainWindow(parent),
			m_render_app_thread()
		{
			ui.setupUi(this);
			_load_style();

			m_d3d_widget = std::make_unique< bc_d3d_output_widget >(centralWidget()->findChild<QFrame*>("leftRenderFrame"));
			m_d3d_output_window = std::make_unique<bc_render_application_d3dwidget_output_window>(m_d3d_widget.get());
			
			m_render_app_thread.m_instance = p_instance;
			m_render_app_thread.m_cmd_line = nullptr;
			m_render_app_thread.m_output_window = m_d3d_output_window.get();
			m_render_app_thread.start();

			m_awesome = std::make_unique< QtAwesome >(this);
			m_awesome->initFontAwesome();
			m_console_widget = std::make_unique< bc_console_widget >(m_awesome.get(), centralWidget()->findChild<QWidget*>("consoleTab"));

			while (m_render_app_thread.m_initialized.load() == 0);

			game::bc_game_console& l_bc_game_console = core::bc_get_service< game::bc_game_system >()->get_console();
			m_editor_game_console = std::make_unique< bc_editor_game_console >(l_bc_game_console, m_console_widget.get());

			l_bc_game_console.set_implementation(m_editor_game_console.get());

			m_editor_game_console->connect_widget(this);
			m_editor_game_console->connect_widget(m_console_widget.get());

			QObject::connect(m_editor_game_console.get(), SIGNAL(scriptExecuted(const QString&)), this, SLOT(onScriptExecution(const QString&)));
			
			core::bc_get_service<core::bc_logger>()->log_info(bcL("info from editor"));
			core::bc_get_service<core::bc_logger>()->log_debug(bcL("debug from editor"));
			core::bc_get_service<core::bc_logger>()->log_error(bcL("error from editor"));
		}

		bc_editor_app::~bc_editor_app()
		{
			m_render_app_thread.wait();
		}

		void bc_editor_app::focusInEvent(QFocusEvent* p_event)
		{
			m_d3d_widget->focus_event(p_event);
		}

		void bc_editor_app::focusOutEvent(QFocusEvent* p_event)
		{
			m_d3d_widget->focus_event(p_event);
		}

		void bc_editor_app::closeEvent(QCloseEvent* p_event)
		{
			m_d3d_widget->close_event(p_event);
		}

		void bc_editor_app::changeEvent(QEvent* p_event)
		{
			if (p_event->type() == QEvent::WindowStateChange)
			{
				QWindowStateChangeEvent* l_window_state_event = static_cast<QWindowStateChangeEvent*>(p_event);

				m_d3d_widget->window_state_change_event(l_window_state_event, windowState());
			}

			if (p_event->type() == QEvent::ActivationChange)
			{
				if (isActiveWindow())
				{
					QFocusEvent l_focus_event(QEvent::FocusIn);

					focusInEvent(&l_focus_event);
				}
				else
				{
					QFocusEvent l_focus_event(QEvent::FocusOut);

					focusInEvent(&l_focus_event);
				}
			}

			QWidget::changeEvent(p_event);
		}

		void bc_editor_app::_load_style() const
		{
			QFile f(":qdarkstyle/style.qss");

			if (f.exists())
			{
				f.open(QFile::ReadOnly | QFile::Text);
				QTextStream ts(&f);

				qApp->setStyleSheet(ts.readAll());
			}
		}

		void bc_editor_app::onScriptExecution(const QString& p_string)
		{
			statusBar()->showMessage("Script executed: " + p_string, 3000);
		}
	}
}
