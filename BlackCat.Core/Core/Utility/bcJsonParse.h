// [03/10/2017 MRB]

#pragma once

#include "Core/Math/bcVector2f.h"
#include "Core/Math/bcVector3f.h"
#include "Core/Math/bcVector4f.h"
#include "Core/File/bcJsonDocument.h"
#include "Core/bcException.h"

namespace black_cat
{
	namespace json_parse
	{
		inline void bc_parse(core::bc_vector2f& p_value, core::bc_json_parse_object& p_jvalue)
		{
			if(!p_jvalue.IsArray())
			{
				throw bc_io_exception("Bad json format");
			}

			auto l_jarray = p_jvalue.GetArray();

			p_value.x = l_jarray.Size() > 0 ? l_jarray[0].GetFloat() : 0;
			p_value.y = l_jarray.Size() > 1 ? l_jarray[1].GetFloat() : 0;
		}

		inline void bc_parse(core::bc_vector2i& p_value, core::bc_json_parse_object& p_jvalue)
		{
			if (!p_jvalue.IsArray())
			{
				throw bc_io_exception("Bad json format");
			}

			auto l_jarray = p_jvalue.GetArray();

			p_value.x = l_jarray.Size() > 0 ? l_jarray[0].GetInt() : 0;
			p_value.y = l_jarray.Size() > 1 ? l_jarray[1].GetInt() : 0;
		}

		inline void bc_parse(core::bc_vector3f& p_value, core::bc_json_parse_object& p_jvalue)
		{
			if (!p_jvalue.IsArray())
			{
				throw bc_io_exception("Bad json format");
			}

			auto l_jarray = p_jvalue.GetArray();

			p_value.x = l_jarray.Size() > 0 ? l_jarray[0].GetFloat() : 0;
			p_value.y = l_jarray.Size() > 1 ? l_jarray[1].GetFloat() : 0;
			p_value.z = l_jarray.Size() > 2 ? l_jarray[2].GetFloat() : 0;
		}

		inline void bc_parse(core::bc_vector3i& p_value, core::bc_json_parse_object& p_jvalue)
		{
			if (!p_jvalue.IsArray())
			{
				throw bc_io_exception("Bad json format");
			}

			auto l_jarray = p_jvalue.GetArray();

			p_value.x = l_jarray.Size() > 0 ? l_jarray[0].GetInt() : 0;
			p_value.y = l_jarray.Size() > 1 ? l_jarray[1].GetInt() : 0;
			p_value.z = l_jarray.Size() > 3 ? l_jarray[3].GetInt() : 0;
		}

		inline void bc_parse(core::bc_vector4f& p_value, core::bc_json_parse_object& p_jvalue)
		{
			if (!p_jvalue.IsArray())
			{
				throw bc_io_exception("Bad json format");
			}

			auto l_jarray = p_jvalue.GetArray();

			p_value.x = l_jarray.Size() > 0 ? l_jarray[0].GetFloat() : 0;
			p_value.y = l_jarray.Size() > 1 ? l_jarray[1].GetFloat() : 0;
			p_value.z = l_jarray.Size() > 2 ? l_jarray[2].GetFloat() : 0;
			p_value.w = l_jarray.Size() > 3 ? l_jarray[3].GetFloat() : 0;
		}

		inline void bc_parse(core::bc_vector4i& p_value, core::bc_json_parse_object& p_jvalue)
		{
			if (!p_jvalue.IsArray())
			{
				throw bc_io_exception("Bad json format");
			}

			auto l_jarray = p_jvalue.GetArray();

			p_value.x = l_jarray.Size() > 0 ? l_jarray[0].GetInt() : 0;
			p_value.y = l_jarray.Size() > 1 ? l_jarray[1].GetInt() : 0;
			p_value.z = l_jarray.Size() > 2 ? l_jarray[2].GetInt() : 0;
			p_value.w = l_jarray.Size() > 3 ? l_jarray[3].GetInt() : 0;
		}
	}
}