// [09/13/2016 MRB]

#pragma once

#include "Core/bcConstant.h"
#include "Core/Math/bcVector3f.h"
#include "PhysicsImp/Shape/bcHeightField.h"
#include "Game/System/Render/bcRenderState.h"

namespace black_cat
{
	namespace game
	{
		class BC_GAME_DLL bc_height_map : public core::bc_icontent
		{
			BC_CONTENT(height_map)

		public:
			bc_height_map(const core::bc_vector3f& p_position,
				bcUINT16 p_width,
				bcUINT16 p_height,
				bcUINT16 p_xz_multiplier,
				bcFLOAT p_y_multiplier,
				bc_render_state_ptr& p_render_state,
				physics::bc_height_field_ref&& p_px_height_map,
				void* p_px_height_map_deserialize_buffer);

			bc_height_map(bc_height_map&& p_other) noexcept;

			virtual ~bc_height_map();

			bc_height_map& operator=(bc_height_map&& p_other) noexcept;

			const core::bc_vector3f& get_position() const
			{
				return m_position;
			}

			bcUINT16 get_width() const
			{
				return m_width;
			}

			bcUINT16 get_height() const
			{
				return m_height;
			}

			bcUINT16 get_xz_multiplier() const
			{
				return m_xz_multiplier;
			}

			bcFLOAT get_y_multiplier() const
			{
				return m_y_multiplier;
			}

			const bc_render_state* get_render_state() const
			{
				return m_render_state.get();
			}

			physics::bc_height_field get_px_height_field() const
			{
				return m_px_height_map.get();
			}

		protected:

		private:
			core::bc_vector3f m_position;
			bcUINT16 m_width;
			bcUINT16 m_height;
			bcUINT16 m_xz_multiplier;
			bcFLOAT m_y_multiplier;

			bc_render_state_ptr m_render_state;
			physics::bc_height_field_ref m_px_height_map;
			void* m_px_height_map_deserialize_buffer;
		};

		using bc_height_map_ptr = core::bc_content_ptr< bc_height_map >;
	}
}