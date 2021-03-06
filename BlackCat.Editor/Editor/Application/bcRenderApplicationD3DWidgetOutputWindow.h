// [11/13/2016 MRB]

#pragma once

#include "Game/Application/bcIRenderApplicationOutputWindow.h"
#include "Editor/Widget/bcD3DOutputWidget.h"

namespace black_cat
{
	namespace editor
	{
		class bc_render_application_d3dwidget_output_window : public game::bc_irender_application_output_window
		{
		public:
			bc_render_application_d3dwidget_output_window(bc_d3d_output_widget* p_d3d_widget);

			bc_render_application_d3dwidget_output_window(bc_render_application_d3dwidget_output_window&&) noexcept;

			~bc_render_application_d3dwidget_output_window();

			bc_render_application_d3dwidget_output_window& operator=(bc_render_application_d3dwidget_output_window&&) noexcept;

			bc_d3d_output_widget* get_widget() noexcept;

			platform::bc_window_id get_id() const noexcept override;

			bcUINT32 get_width() const noexcept override;

			bcUINT32 get_height() const noexcept override;

			graphic::bc_device_output get_device_output() const override;

			void update() override;

			void close() noexcept override;

		protected:

		private:
			bc_d3d_output_widget* m_d3d_widget;
		};

		inline bc_render_application_d3dwidget_output_window::bc_render_application_d3dwidget_output_window(bc_d3d_output_widget* p_d3d_widget)
			: m_d3d_widget(p_d3d_widget)
		{
		}

		inline bc_render_application_d3dwidget_output_window::bc_render_application_d3dwidget_output_window(bc_render_application_d3dwidget_output_window&&) noexcept = default;

		inline bc_render_application_d3dwidget_output_window::~bc_render_application_d3dwidget_output_window() = default;

		inline bc_render_application_d3dwidget_output_window& bc_render_application_d3dwidget_output_window::operator=(bc_render_application_d3dwidget_output_window&&) noexcept = default;

		inline bc_d3d_output_widget* bc_render_application_d3dwidget_output_window::get_widget() noexcept
		{
			return m_d3d_widget;
		}

		inline platform::bc_window_id bc_render_application_d3dwidget_output_window::get_id() const noexcept
		{
			return static_cast< platform::bc_window_id >(m_d3d_widget->winId());
		}

		inline bcUINT32 bc_render_application_d3dwidget_output_window::get_width() const noexcept
		{
			return m_d3d_widget->width();
		}

		inline bcUINT32 bc_render_application_d3dwidget_output_window::get_height() const noexcept
		{
			return  m_d3d_widget->height();
		}

		inline graphic::bc_device_output bc_render_application_d3dwidget_output_window::get_device_output() const
		{
			return m_d3d_widget->get_device_output();
		}

		inline void bc_render_application_d3dwidget_output_window::update()
		{
		}

		inline void bc_render_application_d3dwidget_output_window::close() noexcept
		{
			// Because we are running blackcat appliction in a separate thread and this function will be called 
			// by that thread we can't call qt widget close function due to concurrency problem with main gui thread
			//m_d3d_widget->close();
		}
	}
}