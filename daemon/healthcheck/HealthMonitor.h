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

#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#include <vector>
#include <string_view>
#include <unordered_map>
#include "Specification.hpp"
#include "Check.h"

namespace HealthCheck
{
	using CheckFunc = std::function<Check(std::string_view)>;
	using SectionReport = std::pair<std::string_view, std::vector<Check>>;
	using NewsServersReport = std::vector<SectionReport>;



	struct HealthReport
	{
		SectionReport pathsReport;
		NewsServersReport newsServersReport;
	};

	class HealthMonitor final
	{
	public:
		HealthMonitor() = default;
		HealthMonitor(const HealthMonitor&) = delete;
		HealthMonitor& operator=(const HealthMonitor&) = delete;
		~HealthMonitor() = default;

		void RunChecks();
		const HealthReport& GetReport() const { return m_report; }

	private:
		HealthReport CheckUp() const;
		HealthReport m_report;
	};

	std::string ToJsonStr(const HealthReport& report);
	std::string ToXmlStr(const HealthReport& report);
}

extern HealthCheck::HealthMonitor* g_HealthMonitor;

#endif
