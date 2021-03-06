// [06/30/2016 MRB]

#pragma once

#include "CorePlatformImp/Utility/bcClock.h"
#include "PlatformImp/Application/bcPointingDevice.h"
#include "PlatformImp/Application/bcKeyDevice.h"
#include "Game/System/Input/bcCamera.h"

namespace black_cat
{
	namespace game
	{
		class BC_GAME_DLL bc_free_camera : public bc_perspective_camera
		{
		public:
			bc_free_camera(bcFLOAT p_aspect_ratio,
				bcFLOAT p_height_fov,
				bcFLOAT p_near_clip,
				bcFLOAT p_far_clip,
				bcFLOAT p_move_speed = 25,
				bcFLOAT p_rotate_speed = 1);

			bc_free_camera(bc_free_camera&&) = default;

			~bc_free_camera() = default;

			bc_free_camera& operator=(bc_free_camera&&) = default;

			void update(core_platform::bc_clock::update_param p_clock_update_param,
				const platform::bc_pointing_device& p_pointing_device,
				const platform::bc_key_device& p_key_device) override;

		protected:
			bool on_key(core::bc_ievent& p_key_event) override;

			bool on_pointing(core::bc_ievent& p_pointing_event) override;

		private:
			bcFLOAT m_move_speed;
			bcFLOAT m_rotate_speed;

			bool m_w_pressed;
			bool m_s_pressed;
			bool m_a_pressed;
			bool m_d_pressed;
			bool m_e_pressed;
			bool m_q_pressed;
			bool m_shift_pressed;
			bool m_ctrl_pressed;
			bool m_rmb_pressed;
			bcINT16 m_dx;
			bcINT16 m_dy;
		};
	}
}
