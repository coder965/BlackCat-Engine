// [11/13/2016 MRB]

#pragma once

#include "PlatformImp/Application/bcBasicWindow.h"
#include "Graphic/bcPlatformRenderApi.h"

namespace black_cat
{
	namespace graphic
	{
		template< bc_platform_render_api TRenderApi >
		struct bc_platform_device_output_parameter
		{
		};

		template< bc_platform_render_api TRenderApi >
		struct bc_platform_device_output_pack
		{
		};

		template< bc_platform_render_api TRenderApi >
		class bc_platform_device_output
		{
		public:
			using parameter = bc_platform_device_output_parameter<TRenderApi>;
			using platform_pack = bc_platform_device_output_pack<TRenderApi>;

		public:
			bc_platform_device_output(parameter p_parameter);

			bc_platform_device_output(bc_platform_device_output&&) noexcept;

			~bc_platform_device_output();

			bc_platform_device_output& operator=(bc_platform_device_output&&) noexcept;

			static bc_platform_device_output from_window(platform::bc_basic_window& p_render_window);

			platform_pack& get_platform_pack()
			{
				return m_pack;
			}

		protected:

		private:
			platform_pack m_pack;
		};

		using bc_device_output = bc_platform_device_output< g_current_platform_render_api >;
	}
}
