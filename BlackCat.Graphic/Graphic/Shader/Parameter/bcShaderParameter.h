// [01/15/2016 MRB]

#pragma once

#include "CorePlatform/bcType.h"
#include "Graphic/Shader/bcShader.h"

namespace black_cat
{
	namespace graphic
	{
		enum class bc_shader_parameter_type
		{
			vector,
			matrix,
			matrix_array,
			shader_view,
			unordered_view,
			cbuffer,
			sampler
		};

		class bc_ishader_parameter
		{
		public:
			virtual ~bc_ishader_parameter() = 0;

			bcINT get_register_index() const noexcept;

			void set_register_index(bcINT p_index) noexcept;

			bc_shader_type get_shader_types() const noexcept;

			void set_shader_types(bc_shader_type p_shader_types) noexcept;

			virtual bc_shader_parameter_type get_parameter_type() const = 0;

			virtual void set_parameter_data(void* p_data) = 0;

			virtual bool is_valid() const = 0;

		protected:
			bc_ishader_parameter();

			bc_ishader_parameter(bcINT p_register_index, bc_shader_type p_shader_types);

			bc_ishader_parameter(const bc_ishader_parameter& p_other) = default;

			bc_ishader_parameter& operator=(const bc_ishader_parameter& p_other) = default;

			bcINT16 m_register_index;
			bc_shader_type m_shader_types;

		private:
		};

		inline bc_ishader_parameter::bc_ishader_parameter()
			: m_register_index(-1),
			m_shader_types()
		{
		}

		inline bc_ishader_parameter::bc_ishader_parameter(bcINT p_register_index, bc_shader_type p_shader_types) 
			: m_register_index(p_register_index),
			m_shader_types(p_shader_types)
		{
		}

		inline bc_ishader_parameter::~bc_ishader_parameter()
		{
		}

		inline bcINT bc_ishader_parameter::get_register_index() const noexcept
		{
			return m_register_index;
		}

		inline void bc_ishader_parameter::set_register_index(bcINT p_index) noexcept
		{
			m_register_index = p_index;
		}

		inline bc_shader_type bc_ishader_parameter::get_shader_types() const noexcept
		{
			return m_shader_types;
		}

		inline void bc_ishader_parameter::set_shader_types(bc_shader_type p_shader_types) noexcept
		{
			m_shader_types = p_shader_types;
		}
	}
}