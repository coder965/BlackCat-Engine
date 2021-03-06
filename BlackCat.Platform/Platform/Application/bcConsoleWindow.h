 // [10/23/2016 MRB]

#pragma once

#include "Core/Container/bcString.h"
#include "Platform/Application/bcWindow.h"

namespace black_cat
{
	namespace platform
	{
		template< core_platform::bc_platform TPlatform >
		class bc_platform_application;

		enum class bc_console_window_text_color
		{
			white,
			red,
			green,
			blue,
			aqua,
			purple,
			yellow
		};

		template< core_platform::bc_platform >
		struct bc_platform_console_window_parameter_pack
		{
			explicit bc_platform_console_window_parameter_pack(core::bc_estring p_caption)
				: m_caption(std::move(p_caption))
			{
			}

			core::bc_estring m_caption;
		};

		using bc_console_window_parameter = bc_platform_console_window_parameter_pack< core_platform::g_current_platform >;

		template< core_platform::bc_platform >
		struct bc_platform_console_window_pack
		{
		};

		template< core_platform::bc_platform TPlatform >
		class bc_platform_console_window : public bc_platform_window< TPlatform >
		{
		public:
			using platform_pack = bc_platform_console_window_pack< TPlatform >;
			using paramater = bc_console_window_parameter;
			using bc_platform_window< TPlatform >::id;
			friend class bc_platform_application< TPlatform >;

		public:
			bc_platform_console_window(bc_platform_console_window&& p_other) noexcept;

			~bc_platform_console_window();

			bc_platform_console_window& operator=(bc_platform_console_window&& p_other) noexcept;

			id get_id() const override;

			void set_text_color(bc_console_window_text_color p_color);

			void clear();

			const bcECHAR* get_caption() const override;

			void set_caption(const bcECHAR* p_caption) override;

			void update() override;

			void close() override;

			platform_pack& get_platform_pack()
			{
				return m_pack;
			}

		protected:

		private:
			bc_platform_console_window(bc_console_window_parameter& p_parameter);

			platform_pack m_pack;
		};

		using bc_console_window = bc_platform_console_window< core_platform::g_current_platform >;
	}
}
