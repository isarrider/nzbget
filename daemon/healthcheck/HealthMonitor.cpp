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
		m_report = CheckUp();
	}

	HealthReport HealthMonitor::CheckUp() const
	{
		HealthReport report;
		NewsServersReport newsServersReport;

		// pathsReport[Options::DESTDIR] = CheckDestDir(g_Options->GetDestDir());
		// pathsReport[Options::INTERDIR] = CheckInterDir(g_Options->GetInterDir());
		// pathsReport[Options::NZBDIR] = CheckDestDir(g_Options->GetNzbDir());
		// pathsReport[Options::CERTSTORE] = CheckCertStore(g_Options->GetCertStore());

		report.newsServersReport = std::move(newsServersReport);


// 			{
// 				Options::NZBDIR,
// 				{
// 					Directory::Exists(g_Options->GetNzbDir()),
// 					Directory::IsWritable(g_Options->GetNzbDir())
// 				}
// 			},
// 			{
// 				Options::QUEUEDIR,
// 				{
// 					Directory::Exists(g_Options->GetQueueDir()),
// 					Directory::IsWritable(g_Options->GetQueueDir())
// 				}
// 			},
// 			{
// 				Options::TEMPDIR,
// 				{
// 					Directory::Exists(g_Options->GetTempDir()),
// 					Directory::IsWritable(g_Options->GetTempDir())
// 				}
// 			},
// 						{
// 				Options::WEBDIR,
// 				{
// 					Directory::Exists(g_Options->GetWebDir()),
// 					Directory::IsWritable(g_Options->GetWebDir())
// 				}
// 			},
// 			{
// 				Options::SCRIPTDIR,
// 				{
// 					Directory::Exists(g_Options->GetScriptDir()),
// 					Directory::IsWritable(g_Options->GetScriptDir())
// 				}
// 			},
// #ifndef WIN32
// 			{
// 				Options::LOCKFILE,
// 				{
// 					File::Exists(g_Options->GetLockFile()),
// 					File::IsReadable(g_Options->GetLockFile())
// 				}
// 			},
// #endif
// 			{
// 				Options::LOGFILE,
// 				{
// 					File::Exists(g_Options->GetLogFile()),
// 					File::IsWritable(g_Options->GetLogFile())
// 				}
// 			},
// 			{
// 				Options::CERTSTORE,
// 				{
// 					File::Exists(g_Options->GetCertStore()),
// 					File::IsReadable(g_Options->GetCertStore())
// 				}
// 			}

		return report;
	}

	std::string ToJsonStr(const HealthReport& report)
	{
		Json::JsonObject reportJson;
		Json::JsonObject pathsJson;
		Json::JsonObject newsServersJson;

		for (const auto& [name, check] : report.newsServersReport)
		{
			pathsJson[name] = ToJson(check[0]);
		}

		reportJson["Paths"] = std::move(pathsJson);
		reportJson["NewsServers"] = std::move(newsServersJson);

		return Json::serialize(reportJson);
	}

	std::string ToXmlStr(const HealthReport& report)
	{
		return "";
	}
}

#endif
