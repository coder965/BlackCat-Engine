// [11/12/2016 MRB]

#pragma once

#include "Editor/Widget/bcD3DOutputWidget.h"
#include "Editor/Widget/bcConsoleWidget.h"
#include "Editor/Application/bcRenderApplicationD3DWidgetOutputWindow.h"
#include "Editor/Application/bcEditorGameConsole.h"
#include "Editor/QtAwesome/QtAwesome.h"
#include "Editor/UI/bcFormTools.h"
#include "Editor/UI/bcFormTerrain.h"
#include "ui_bcBlackCatEditor.h"

#include <QtWidgets/QMainWindow>
#include <QtConcurrent/QtConcurrent>

namespace black_cat
{
	namespace editor
	{
		class bc_render_app_thread : public QThread
		{
		public:
			HINSTANCE m_instance;
			const char* m_cmd_line;
			game::bc_irender_application_output_window* m_output_window;
			QAtomicInt m_initialized;
			QAtomicInt m_result_code;

		protected:
			void run() override;
		};

		class bc_editor_app : public QMainWindow
		{
			Q_OBJECT

		public:
			explicit bc_editor_app(HINSTANCE p_instance, QWidget* parent = Q_NULLPTR);

			~bc_editor_app();

		protected:
			void focusInEvent(QFocusEvent* p_event) override;

			void focusOutEvent(QFocusEvent* p_event) override;

			void closeEvent(QCloseEvent* p_event) override;

			void changeEvent(QEvent* p_event) override;

		private slots:
			void onScriptExecution(const QString& p_string);

		signals:
			void uiScripted(const QString& p_string);

		private:
			void _load_style() const;

			void _load_icon(QWidget* p_parent, QVariantMap& p_options) const;

			void _load_icon_button(QAbstractButton* p_bottom, QVariantMap& p_options) const;

			void _load_icon_toolbox(QToolBox* p_tool_box, QVariantMap& p_options) const;

			Ui::bcBlackCatEditorClass ui;
			std::unique_ptr< QtAwesome > m_awesome;

			std::unique_ptr< bc_d3d_output_widget > m_d3d_widget;
			std::unique_ptr< bc_render_application_d3dwidget_output_window > m_d3d_output_window;
			std::unique_ptr< bc_console_widget > m_console_widget;
			std::unique_ptr< bc_editor_game_console > m_editor_game_console;

			std::unique_ptr< bc_form_tools > m_form_tools;
			std::unique_ptr< bc_form_terrain > m_form_terrain;

			bc_render_app_thread m_render_app_thread;
		};
	}
}