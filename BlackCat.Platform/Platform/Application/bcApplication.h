// [2/23/2015 MRB]

#pragma once

#include "Core/Utility/bcSingleton.h"
#include "Core/Concurrency/bcThreadManager.h"
#include "Core/Event/bcEventManager.h"
#include "Core/File/bcContentStreamManager.h"
#include "Platform/PlatformPCH.h"
#include "Platform/bcEvent.h"
#include "Platform/Application/bcBasicWindow.h"
#include "Platform/Application/bcConsoleWindow.h"

namespace black_cat
{
	namespace platform
	{
		template< core_platform::bc_platform >
		struct bc_platform_application_parameter_pack
		{
		public:
			bc_platform_application_parameter_pack(const bcECHAR* p_app_name,
				const bcCHAR* p_commandline)
				: m_app_name(p_app_name),
				m_commandline(p_commandline)
			{
			}

			const bcECHAR* m_app_name;
			const bcCHAR* m_commandline;
		};

		using bc_application_parameter = bc_platform_application_parameter_pack< core_platform::g_current_platform >;

		template< core_platform::bc_platform >
		struct bc_platform_application_pack
		{

		};

		template< core_platform::bc_platform TPlatform >
		class bc_platform_application
		{
		public:
			using platform_pack = bc_platform_application_pack< TPlatform >;

		public:
			bc_platform_application(bc_application_parameter& p_parameter);

			bc_platform_application(bc_platform_application&&) noexcept;

			~bc_platform_application();

			bc_platform_application operator=(bc_platform_application&&) noexcept;

			/**
			 * \brief Create a basic window.
			 * \param p_caption 
			 * \param p_width 
			 * \param p_height 
			 * \return 
			 */
			bc_basic_window create_basic_window(core::bc_estring p_caption, bcUINT32 p_width, bcUINT32 p_height);

			/**
			 * \brief Create a console window. Support for creating console window along with render window is platform specific.
			 * \param p_caption 
			 * \return 
			 */
			bc_console_window create_console_window(core::bc_estring p_caption);

			/**
			 * \brief Update method for things like message dispatching.
			 * Implementation must propagate application events in response to OS events.
			 */
			void update();

			void request_termination();

		protected:

		private:
			platform_pack m_pack;
		};

		using bc_application = bc_platform_application< core_platform::g_api_win32 >;
	}
}
