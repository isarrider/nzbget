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

#include "nzbget.h"

#include <iostream>
#include "CheckResult.h"

namespace HealthCheck
{
	Json::JsonValue ToJson(const CheckResult& result)
	{
		Json::JsonObject json;
		json["Status"] = static_cast<int>(result.GetStatus());
		json["Message"] = result.GetMessage();

		return json;
	}

	std::string ToXml(const CheckResult& result)
	{
		xmlChar valueName[] = "value"; 
		xmlChar structName[] = "struct"; 
		xmlNodePtr rootNode = xmlNewNode(nullptr, valueName);
		xmlNodePtr structNode = xmlNewNode(nullptr, structName);

		Xml::AddNewNode(structNode, "Status", "i4", std::to_string(static_cast<int>(result.GetStatus())).c_str());
		Xml::AddNewNode(structNode, "Message", "string", result.GetMessage().c_str());

		std::string str = Xml::Serialize(rootNode);

		xmlFreeNode(rootNode);

		return str;
	}

	namespace File
	{
		CheckResult Exists(const fs::path& path)
		{
			boost::system::error_code ec;
			if (!fs::is_regular_file(path, ec))
			{
				return CheckResult::Error(ec.message());
			}

			return CheckResult::Ok();
		}

		CheckResult IsWritable(const fs::path& path)
		{
			std::ofstream file(path.c_str(), std::ios::app);
			if (!file.is_open())
			{
				return CheckResult::Error("Is not writeable");
			}

			return CheckResult::Ok();
		}

		CheckResult IsReadable(const fs::path& path)
		{
			std::ifstream file(path.c_str(), std::ios::app);
			if (!file.is_open())
			{
				return CheckResult::Error("Is not readable");
			}

			return CheckResult::Ok();
		}
	}

	namespace Directory
	{
		CheckResult Exists(const fs::path& path)
		{
			boost::system::error_code ec;
			if (!fs::is_directory(path, ec))
			{
				return CheckResult::Error(ec.message());
			}

			return CheckResult::Ok();
		}

		CheckResult IsWritable(const fs::path& path)
		{
			const auto filePath = path / "nzbget_write_test.tmp";
			std::ofstream file(filePath.c_str());
			if (!file.is_open())
				return CheckResult::Error("Is not writable");

			file << "This file was created to verify if this directory is writable. It should've been automatically deleted. Feel free to delete it.";
			file.close();

			fs::remove(filePath);

			return CheckResult::Ok();
		}
	}
}
