/*
 *  This file is part of nzbget. See <https://nzbget.com>.
 *
 *  Copyright (C) 2025 Denis <denis@nzbget.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CHECK_RESULT_H
#define CHECK_RESULT_H

#include <string>
#include <boost/filesystem.hpp>

namespace HealthCheck
{
	namespace fs = boost::filesystem;

	enum class Result { Ok, Warning, Error };

	class CheckResult final
	{
	public:
		static CheckResult Ok()
		{
			return CheckResult(Result::Ok, "");
		}

		static CheckResult Warning(std::string message)
		{
			return CheckResult(Result::Warning, std::move(message));
		}

		static CheckResult Error(std::string message)
		{
			return CheckResult(Result::Error, std::move(message));
		}

		bool IsError() const { return m_result == Result::Error; }
		Result GetResult() const { return m_result; }
		const std::string& GetMessage() const { return m_message; }

	private:
		CheckResult(Result result, std::string message)
			: m_result{ result }
			, m_message{ std::move(message) }
		{
		}

		Result m_result;
		std::string m_message;
	};

	namespace File
	{
		CheckResult Exists(const fs::path& path);
		CheckResult IsWritable(const fs::path& path);
		CheckResult IsReadable(const fs::path& path);
	}

	namespace Directory
	{
		CheckResult Exists(const fs::path& path);
		CheckResult IsWritable(const fs::path& path);
	}
}

#endif
