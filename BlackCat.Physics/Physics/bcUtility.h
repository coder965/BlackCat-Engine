// [11/30/2016 MRB]

#pragma once

#include "CorePlatform/bcType.h"

namespace black_cat
{
	namespace physics
	{
		struct bc_strided_data
		{
		public:
			bc_strided_data(const void* p_data, bcUINT32 p_stride)
				: m_data(p_data),
				m_stride(p_stride)
			{
			}

			template< typename T >
			const T& at(bcUINT32 p_index) const
			{
				return *reinterpret_cast< const T* >(reinterpret_cast<const bcBYTE*>(m_data) + p_index * m_stride);
			}

			const void* m_data;
			const bcUINT32 m_stride;
		};

		struct bc_bounded_strided_data : public bc_strided_data
		{
		public:
			bc_bounded_strided_data(const void* p_data, bcUINT32 p_stride, bcUINT32 p_count)
				: bc_strided_data(p_data, p_stride),
				m_count(p_count)
			{
			}

			template< typename T >
			const T& at(bcUINT32 p_index)
			{
				bcAssert(p_index <= m_count);

				return bc_strided_data::at<T>(p_index);
			}

			const bcUINT32 m_count;
		};

		struct bc_geometry_scale
		{
		public:
			bc_geometry_scale()
				: bc_geometry_scale(1)
			{
			}

			explicit bc_geometry_scale(bcFLOAT p_scale)
				: m_scale(p_scale)
			{
			}

			const bcFLOAT m_scale;
		};
	}
}