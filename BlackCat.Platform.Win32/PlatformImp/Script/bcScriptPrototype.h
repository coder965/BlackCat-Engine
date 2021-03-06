 // [10/15/2016 MRB]

#pragma once

#include "PlatformImp/Script/bcScriptRef.h"
#include "PlatformImp/Script/bcScriptReference.h"
#include "PlatformImp/Script/bcScriptContext.h"

#include "Platform/Script/bcScriptPrototype.h"
#include "PlatformImp/PlatformImpPCH.h"

namespace black_cat
{
	namespace platform
	{
		template< typename T >
		struct bc_platform_script_prototype_pack< core_platform::g_api_win32, T >
		{
			JsValueRef m_js_ctor_function;
			JsValueRef m_js_prototype;
		};

		template< core_platform::bc_platform TPlatform, typename T >
		bc_platform_script_prototype<TPlatform, T>::bc_platform_script_prototype(bc_script_context& p_context)
			: bc_platform_script_reference()
		{
		}

		template< core_platform::bc_platform TPlatform, typename T >
		bc_platform_script_prototype<TPlatform, T>::bc_platform_script_prototype()
			: bc_platform_script_reference()
		{
			m_pack.m_js_ctor_function = JS_INVALID_REFERENCE;
			m_pack.m_js_prototype = JS_INVALID_REFERENCE;
		}

		template< core_platform::bc_platform TPlatform, typename T >
		bc_platform_script_prototype<TPlatform, T>::bc_platform_script_prototype(const bc_platform_script_prototype&) noexcept = default;

		template< core_platform::bc_platform TPlatform, typename T >
		bc_platform_script_prototype<TPlatform, T>::~bc_platform_script_prototype() = default;

		template< core_platform::bc_platform TPlatform, typename T >
		bc_platform_script_prototype<TPlatform, T>& bc_platform_script_prototype<TPlatform, T>::operator=(const bc_platform_script_prototype&) noexcept = default;

		template< core_platform::bc_platform TPlatform, typename T >
		bool bc_platform_script_prototype<TPlatform, T>::is_valid() const noexcept
		{
			return m_pack.m_js_prototype != JS_INVALID_REFERENCE;
		}
	}
}