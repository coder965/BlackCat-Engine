// [06/30/2016 MRB]

#include "Game/GamePCH.h"
#include "Platform/bcEvent.h"
#include "GraphicImp/bcRenderApiInfo.h"
#include "Game/System/Input/bcFreeCamera.h"

namespace black_cat
{
	namespace game
	{
		bc_free_camera::bc_free_camera(bcFLOAT p_aspect_ratio,
			bcFLOAT p_height_fov,
			bcFLOAT p_near_clip,
			bcFLOAT p_far_clip,
			bcFLOAT p_move_speed,
			bcFLOAT p_rotate_speed)
			: bc_perspective_camera(p_aspect_ratio, p_height_fov, p_near_clip, p_far_clip),
			m_move_speed(p_move_speed),
			m_rotate_speed(p_rotate_speed),
			m_w_pressed(false),
			m_s_pressed(false),
			m_a_pressed(false),
			m_d_pressed(false),
			m_e_pressed(false),
			m_q_pressed(false),
			m_shift_pressed(false),
			m_ctrl_pressed(false),
			m_rmb_pressed(false),
			m_dx(0),
			m_dy(0)
		{
		}

		void bc_free_camera::update(core_platform::bc_clock::update_param p_clock_update_param,
			const platform::bc_pointing_device& p_pointing_device,
			const platform::bc_key_device& p_key_device)
		{
			core::bc_vector3f l_position = get_position();
			bcFLOAT l_move_speed = m_shift_pressed ? m_move_speed * 6 : m_ctrl_pressed ? m_move_speed * 0.25 : m_move_speed;
			bcFLOAT l_rotate_speed = m_rotate_speed;

			l_move_speed *= p_clock_update_param.m_elapsed_second;
			l_rotate_speed *= p_clock_update_param.m_elapsed_second;

			if (m_w_pressed)
			{
				l_position += get_forward() * l_move_speed;
			}
			if (m_s_pressed)
			{
				l_position += get_back() * l_move_speed;
			}
			if (m_a_pressed)
			{
				l_position += get_left() * l_move_speed;
			}
			if (m_d_pressed)
			{
				l_position += get_right() * l_move_speed;
			}
			if (m_e_pressed)
			{
				l_position += get_down() * l_move_speed;
			}
			if (m_q_pressed)
			{
				l_position += get_up() * l_move_speed;
			}

			core::bc_vector3f l_direction = get_direction();

			if (m_rmb_pressed)
			{
				bcFLOAT l_pi = 3.14159265358979323846 / 2;
				auto l_dx = -m_dx * l_rotate_speed;
				auto l_dy = m_dy * l_rotate_speed;

				if (m_dy > l_pi)
				{
					m_dy = l_pi;
				}
				if (m_dy < -l_pi)
				{
					m_dy = -l_pi;
				}

				core::bc_matrix3f l_rotation_y;
				core::bc_matrix3f l_rotation_x;
				core::bc_matrix3f l_rotation;

				core::bc_vector3f l_right = get_right();
				core::bc_vector3f l_up = get_up();

				if(graphic::bc_render_api_info::is_left_handed())
				{
					l_rotation_y.rotation_euler_lh(l_up, -l_dx);
					l_rotation_x.rotation_euler_lh(l_right, l_dy);
				}
				else
				{
					l_rotation_y.rotation_euler_rh(l_up, -l_dx);
					l_rotation_x.rotation_euler_rh(l_right, l_dy);
				}
				l_rotation = l_rotation_y * l_rotation_x;

				l_direction = l_rotation * l_direction;
				l_direction.normalize();
			}

			auto l_lookat = l_position + l_direction;

			look_at(l_position, l_lookat, core::bc_vector3f(0, 1, 0));

			m_dx = 0;
			m_dy = 0;
		}

		bool bc_free_camera::on_key(core::bc_ievent& p_key_event)
		{
			bcAssert(core::bc_ievent::event_is<platform::bc_app_event_key>(p_key_event));

			auto& l_key_event = static_cast< platform::bc_app_event_key& >(p_key_event);

			if (l_key_event.get_key() == platform::bc_key::kb_W)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_w_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_w_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_S)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_s_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_s_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_A)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_a_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_a_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_D)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_d_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_d_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_E)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_e_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_e_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_Q)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_q_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_q_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_shift)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_shift_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_shift_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::kb_ctrl)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_ctrl_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_ctrl_pressed = false;
				}
			}
			else if (l_key_event.get_key() == platform::bc_key::ms_right_button)
			{
				if (l_key_event.get_key_state() == platform::bc_key_state::pressing)
				{
					m_rmb_pressed = true;
				}
				else if (l_key_event.get_key_state() == platform::bc_key_state::releasing)
				{
					m_rmb_pressed = false;
				}
			}

			return true;
		}

		bool bc_free_camera::on_pointing(core::bc_ievent& p_pointing_event)
		{
			bcAssert(core::bc_ievent::event_is<platform::bc_app_event_pointing>(p_pointing_event));

			auto& l_pointing_event = static_cast<platform::bc_app_event_pointing&>(p_pointing_event);

			m_dx = l_pointing_event.get_state().m_dx;
			m_dy = l_pointing_event.get_state().m_dy;

			return true;
		}
	}
}