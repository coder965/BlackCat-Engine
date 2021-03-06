// [01/03/2016 MRB]

#pragma once

#include "Graphic/GraphicPCH.h"
#include "Graphic/bcDeviceReference.h"
#include "Graphic/bcDeviceRef.h"
#include "Graphic/bcRenderApi.h"
#include "Graphic/bcGraphicDefinition.h"

namespace black_cat
{
	namespace graphic
	{
		class bc_subresource_data
		{
		public:
			explicit bc_subresource_data(void* p_data, bcUINT p_row_pitch, bcUINT p_depth_pitch)
				: m_data(p_data), 
				m_row_pitch(p_row_pitch),
				m_depth_pitch(p_depth_pitch)
			{
			}

			const void* m_data;
			bcUINT m_row_pitch;
			bcUINT m_depth_pitch;
		};

		struct bc_mapped_resource
		{
		public:
			void* m_data;
			bcUINT m_row_pitch;
			bcUINT m_depth_pitch;
		};

		template<bc_render_api TRenderApi>
		struct bc_platform_iresource_pack
		{
		};

		template<bc_render_api TRenderApi>
		class bc_platform_iresource : public bc_platform_device_reference<TRenderApi>
		{
		public:
			using platform_pack = bc_platform_iresource_pack<TRenderApi>;

		public:
			virtual ~bc_platform_iresource();

			virtual bc_resource_type get_type() const = 0;

			bc_resource_eviction_priority get_eviction_priority() const;

			void set_eviction_priority(bc_resource_eviction_priority p_eviction_priority);

			platform_pack& get_platform_pack()
			{
				return m_pack;
			}

		protected:
			bc_platform_iresource();

			explicit bc_platform_iresource(platform_pack& p_pack);

			bc_platform_iresource(const bc_platform_iresource& p_other);

			bc_platform_iresource& operator=(const bc_platform_iresource& p_other);

			platform_pack m_pack;

		private:
		};

		using bc_iresource = bc_platform_iresource<g_current_render_api>;
	}
}