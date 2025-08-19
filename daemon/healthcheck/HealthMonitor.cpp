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

#ifndef APP_HEALTH_H
#define APP_HEALTH_H

#include <vector>
#include <unordered_map>
#include "HealthMonitor.h"
#include "Options.h"
#include "Json.h"
#include "Xml.h"

namespace HealthCheck
{
	void HealthMonitor::RunChecks()
	{
		m_report.results = GetResults();
		m_report.hints = GetHints();
	}

	Results HealthMonitor::GetResults() const
	{
		return
		{
			{
				Options::DESTDIR,
				{
					Directory::Exists(g_Options->GetDestDir()),
					Directory::IsWritable(g_Options->GetDestDir())
				}
			},
			{
				Options::INTERDIR,
				{
					Directory::Exists(g_Options->GetDestDir()),
					Directory::IsWritable(g_Options->GetDestDir())
				}
			},
			{
				Options::NZBDIR,
				{
					Directory::Exists(g_Options->GetNzbDir()),
					Directory::IsWritable(g_Options->GetNzbDir())
				}
			},
			{
				Options::QUEUEDIR,
				{
					Directory::Exists(g_Options->GetQueueDir()),
					Directory::IsWritable(g_Options->GetQueueDir())
				}
			},
			{
				Options::TEMPDIR,
				{
					Directory::Exists(g_Options->GetTempDir()),
					Directory::IsWritable(g_Options->GetTempDir())
				}
			},
						{
				Options::WEBDIR,
				{
					Directory::Exists(g_Options->GetWebDir()),
					Directory::IsWritable(g_Options->GetWebDir())
				}
			},
			{
				Options::SCRIPTDIR,
				{
					Directory::Exists(g_Options->GetScriptDir()),
					Directory::IsWritable(g_Options->GetScriptDir())
				}
			},
#ifndef WIN32
			{
				Options::LOCKFILE,
				{
					File::Exists(g_Options->GetLockFile()),
					File::IsReadable(g_Options->GetLockFile())
				}
			},
#endif
			{
				Options::LOGFILE,
				{
					File::Exists(g_Options->GetLogFile()),
					File::IsWritable(g_Options->GetLogFile())
				}
			},
			{
				Options::CERTSTORE,
				{
					File::Exists(g_Options->GetCertStore()),
					File::IsReadable(g_Options->GetCertStore())
				}
			}
		};
	}

	Hints HealthMonitor::GetHints() const
	{
		return {{}};
	}

	Json::JsonValue ToJson(const Results& results)
	{
		Json::JsonObject json;

		for (const auto& [name, checks] : results)
		{
			Json::JsonArray jsonArr;

			for (const auto& check : checks)
			{
				jsonArr.push_back(ToJson(check));
			}
			
			json[name] = jsonArr;
		}

		return json;
	}

	Json::JsonValue ToJson(const Hints& hints)
	{
		Json::JsonObject json;

		for (const auto& [name, hintArr] : hints)
		{
			Json::JsonArray jsonArr;

			for (const auto& hint : hintArr)
			{
				jsonArr.push_back(Json::JsonValue(hint));
			}
			
			json[name] = jsonArr;
		}

		return json;
	}

	std::string ToXml(const Hints& hints)
	{

	}

	std::string ToJson(const HealthReport& report)
	{
		Json::JsonObject json;

		json["Results"] = ToJson(report.results);
		json["Hints"] = ToJson(report.hints);

		return Json::Serialize(json);
	}

	std::string ToXml(const HealthReport& report)
	{
		return "";
	}
}

#endif
