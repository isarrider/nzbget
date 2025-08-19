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
#include "Json.h"
#include "Xml.h"

namespace HealthCheck
{
	namespace fs = boost::filesystem;

	enum class Status { Ok, Warning, Error };

	class CheckResult final
	{
	public:
		static CheckResult Ok()
		{
			return CheckResult(Status::Ok, "");
		}

		static CheckResult Warning(std::string message)
		{
			return CheckResult(Status::Warning, std::move(message));
		}

		static CheckResult Error(std::string message)
		{
			return CheckResult(Status::Error, std::move(message));
		}

		bool IsError() const { return m_status == Status::Error; }
		Status GetStatus() const { return m_status; }
		const std::string& GetMessage() const { return m_message; }

	private:
		CheckResult(Status status, std::string message)
			: m_status{ status }
			, m_message{ std::move(message) }
		{
		}

		Status m_status;
		std::string m_message;
	};

	Json::JsonValue ToJson(const CheckResult& result);
	std::string ToXml(const CheckResult& result);

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
