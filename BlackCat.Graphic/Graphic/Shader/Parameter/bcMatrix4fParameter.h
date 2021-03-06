// [01/16/2016 MRB]

#pragma once

#include "Core/Math/bcMatrix4f.h"
#include "Graphic/Shader/Parameter/bcShaderParameter.h"

namespace black_cat
{
	namespace graphic
	{
		class bc_matrix4f_parameter : public bc_ishader_parameter
		{
		public:
			bc_matrix4f_parameter();

			bc_matrix4f_parameter(bcINT p_register, bc_shader_type p_shader_types, core::bc_matrix4f& p_matrix);

			bc_matrix4f_parameter(bc_matrix4f_parameter&) = default;

			~bc_matrix4f_parameter() = default;

			bc_matrix4f_parameter& operator=(bc_matrix4f_parameter&) = default;

			core::bc_matrix4f get_matrix() const;

			void set_matrix(core::bc_matrix4f& p_matrix);

			bc_shader_parameter_type get_parameter_type() const override;

			void set_parameter_data(void* p_data) override;

			bool is_valid() const override;
		protected:

		private:
			core::bc_matrix4f m_matrix;
		};

		inline bc_matrix4f_parameter::bc_matrix4f_parameter()
			: bc_ishader_parameter()
		{
		}

		inline bc_matrix4f_parameter::bc_matrix4f_parameter(bcINT p_register, bc_shader_type p_shader_types, core::bc_matrix4f& p_matrix)
			: bc_ishader_parameter(p_register, p_shader_types),
			m_matrix(p_matrix)
		{
		}

		inline core::bc_matrix4f bc_matrix4f_parameter::get_matrix() const
		{
			return m_matrix;
		}

		inline void bc_matrix4f_parameter::set_matrix(core::bc_matrix4f& p_matrix)
		{
			m_matrix = p_matrix;
		}

		inline bc_shader_parameter_type bc_matrix4f_parameter::get_parameter_type() const
		{
			return bc_shader_parameter_type::matrix;
		}

		inline void bc_matrix4f_parameter::set_parameter_data(void* p_data)
		{
			if (!p_data)
				set_matrix(core::bc_matrix4f::identity());
			else
				set_matrix(*reinterpret_cast< core::bc_matrix4f*>(p_data));
		}

		inline bool bc_matrix4f_parameter::is_valid() const
		{
			return m_register_index != -1;
		}
	}
}