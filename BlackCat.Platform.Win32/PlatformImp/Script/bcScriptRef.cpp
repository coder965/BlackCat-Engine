// [11/10/2016 MRB]

#include "PlatformImp/PlatformImpPCH.h"
#include "PlatformImp/Script/bcScriptRef.h"
#include "PlatformImp/Script/bcScriptContext.h"
#include "PlatformImp/Script/bcScriptVariable.h"
#include "PlatformImp/Script/bcScriptByteCode.h"
#include "PlatformImp/Script/bcScriptObject.h"
#include "PlatformImp/Script/bcScriptString.h"
#include "PlatformImp/Script/bcScriptFunction.h"
#include "PlatformImp/Script/bcScriptArray.h"
#include "PlatformImp/Script/bcScriptError.h"
#include "PlatformImp/Script/bcScriptPrototype.h"

namespace black_cat
{
	namespace platform
	{
		void _inc_ref(bc_platform_script_context< core_platform::g_api_win32 >& p_context)
		{
			bc_chakra_call l_call = JsAddRef(p_context.get_platform_pack().m_js_context, nullptr);
		}

		void _dec_ref(bc_platform_script_context< core_platform::g_api_win32 >& p_context)
		{
			bc_chakra_call l_call = JsRelease(p_context.get_platform_pack().m_js_context, nullptr);
		}

		void _inc_ref(bc_platform_script_bytecode< core_platform::g_api_win32 >& p_byte_code)
		{
			bc_chakra_call l_call = JsAddRef(p_byte_code.get_platform_pack().m_js_parsed_script, nullptr);
		}

		void _dec_ref(bc_platform_script_bytecode< core_platform::g_api_win32 >& p_byte_code)
		{
			bc_chakra_call l_call = JsRelease(p_byte_code.get_platform_pack().m_js_parsed_script, nullptr);
		}

		void _inc_ref(bc_platform_script_variable< core_platform::g_api_win32 >& p_variable)
		{
			bc_chakra_call l_call = JsAddRef(p_variable.get_platform_pack().m_js_value, nullptr);
		}

		void _dec_ref(bc_platform_script_variable< core_platform::g_api_win32 >& p_variable)
		{
			bc_chakra_call l_call = JsRelease(p_variable.get_platform_pack().m_js_value, nullptr);
		}

		void _inc_ref(bc_platform_script_object< core_platform::g_api_win32 >& p_object)
		{
			bc_chakra_call l_call = JsAddRef(p_object.get_platform_pack().m_js_object, nullptr);
		}

		void _dec_ref(bc_platform_script_object< core_platform::g_api_win32 >& p_object)
		{
			bc_chakra_call l_call = JsRelease(p_object.get_platform_pack().m_js_object, nullptr);
		}

		void _inc_ref(bc_platform_script_string< core_platform::g_api_win32 >& p_string)
		{
			bc_chakra_call l_call = JsAddRef(p_string.get_platform_pack().m_js_string, nullptr);
		}

		void _dec_ref(bc_platform_script_string< core_platform::g_api_win32 >& p_string)
		{
			bc_chakra_call l_call = JsRelease(p_string.get_platform_pack().m_js_string, nullptr);
		}

		void _inc_ref(bc_platform_script_function_base< core_platform::g_api_win32 >& p_function)
		{
			bc_chakra_call l_call = JsAddRef(p_function.get_platform_pack().m_js_function, nullptr);
		}

		void _dec_ref(bc_platform_script_function_base< core_platform::g_api_win32 >& p_function)
		{
			bc_chakra_call l_call = JsRelease(p_function.get_platform_pack().m_js_function, nullptr);
		}

		void _inc_ref(bc_platform_script_array_base< core_platform::g_api_win32 >& p_array)
		{
			_inc_ref(static_cast< bc_script_object& >(p_array));
		}

		void _dec_ref(bc_platform_script_array_base< core_platform::g_api_win32 >& p_array)
		{
			_dec_ref(static_cast< bc_script_object& >(p_array));
		}

		void _inc_ref(bc_platform_script_error< core_platform::g_api_win32 >& p_error)
		{
			bc_chakra_call l_call = JsAddRef(p_error.get_platform_pack().m_js_error, nullptr);
		}

		void _dec_ref(bc_platform_script_error< core_platform::g_api_win32 >& p_error)
		{
			bc_chakra_call l_call = JsRelease(p_error.get_platform_pack().m_js_error, nullptr);
		}

		template< typename T >
		void _inc_ref(bc_platform_script_prototype< core_platform::g_api_win32, T >& p_error)
		{
			bc_chakra_call l_call;
			l_call = JsAddRef(p_error.get_platform_pack().m_js_ctor_function, nullptr);
			l_call = JsAddRef(p_error.get_platform_pack().m_js_prototype, nullptr);
		}

		template< typename T >
		void _dec_ref(bc_platform_script_prototype< core_platform::g_api_win32, T >& p_error)
		{
			bc_chakra_call l_call;
			l_call = JsRelease(p_error.get_platform_pack().m_js_ctor_function, nullptr);
			l_call = JsAddRef(p_error.get_platform_pack().m_js_prototype, nullptr);
		}
	}
}
