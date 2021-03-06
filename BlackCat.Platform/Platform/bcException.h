// [2/27/2015 MRB]

#pragma once

#include "Core/bcException.h"
#include "Core/Container/bcString.h"

namespace black_cat
{
	class bc_script_interrupted_exception : public std::exception
	{
	public:
		bc_script_interrupted_exception() = default;

		bc_script_interrupted_exception(const bc_script_interrupted_exception&) = default;

		~bc_script_interrupted_exception() = default;

		bc_script_interrupted_exception& operator =(const bc_script_interrupted_exception&) = default;

	protected:

	private:
	};

	class bc_script_execute_exception : public bc_runtime_exception
	{
	public:
		explicit bc_script_execute_exception(const bcCHAR* p_message);

		bc_script_execute_exception(const bc_script_execute_exception&) = default;

		~bc_script_execute_exception() = default;

		bc_script_execute_exception& operator=(const bc_script_execute_exception&) = default;

	protected:

	private:
	};

	class bc_script_compile_exception : public bc_runtime_exception
	{
	public:
		bc_script_compile_exception(const bcCHAR* p_source, bcINT32 p_line, bcINT32 p_column, const bcCHAR* p_message);

		bc_script_compile_exception(const bc_script_compile_exception&) = default;

		~bc_script_compile_exception() = default;

		bc_script_compile_exception& operator=(const bc_script_compile_exception&) = default;

		const core::bc_string& source() const noexcept;

		bcINT32 line() const noexcept;

		bcINT32 column() const noexcept;

	protected:

	private:
		core::bc_string m_source;
		bcINT32 m_line;
		bcINT32 m_column;
	};

	inline bc_script_execute_exception::bc_script_execute_exception(const bcCHAR* p_message)
		: bc_runtime_exception(p_message)
	{
	}

	inline bc_script_compile_exception::bc_script_compile_exception(const bcCHAR* p_source, bcINT32 p_line, bcINT32 p_column, const bcCHAR* p_message)
		: bc_runtime_exception(p_message),
		m_source(p_source),
		m_line(p_line),
		m_column(p_column)
	{
	}

	inline const core::bc_string& bc_script_compile_exception::source() const noexcept
	{
		return m_source;
	}

	inline bcINT32 bc_script_compile_exception::line() const noexcept
	{
		return m_line;
	}

	inline bcINT32 bc_script_compile_exception::column() const noexcept
	{
		return m_column;
	}
}