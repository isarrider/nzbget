/*
 * This file is part of nzbget. See <https://nzbget.com>.
 *
 * Copyright (C) 2025 Denis <denis@nzbget.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

function SectionHealth(options_, sectionName)
{
	let name = sectionName;
	let health = {
		info: [],
		warnings: [],
		errors: []
	}

	let options = {};

	init(options_);

	this.getHealth = function()
	{
		return health;
	}

	this.getChecks = function(name)
	{
		return options[name];
	}

	this.getName = function()
	{
		return name;
	}

	this.getInfo = function(name) 
	{
		return options[name].info;
	}

	this.getWarnings = function(name) 
	{
		return options[name].warnings;
	}

	this.getErrors = function(name) 
	{
		return options[name].errors;
	}

	function init(options_)
	{
		Object.entries(options_).forEach(function([name, checks]) {
			options[name] = {
				info: [],
				warnings: [],
				errors: []
			};
			for (const check of checks) {
				if (check.Status === "info")
				{
					health.info.push(check);
					options[name].info.push(check);
				}
				else if (check.Status === "warning") {
					health.warnings.push(check);
					options[name].warnings.push(check);
				}
				else if (check.Status === "error") {
					health.errors.push(check);
					options[name].errors.push(check);
				}	
			}
		});
	}
}

function HealthReport() {
	let sections = {};
	let infoCount = 0;
	let warningsCount = 0;
	let errorsCount = 0;

	this.getInfoCount = function()
	{
		return infoCount;
	}

	this.getWarningsCount = function()
	{
		return warningsCount;
	}

	this.getErrorsCount = function()
	{
		return errorsCount;
	}

	this.getSection = function(name)
	{
		return sections[name];
	}

	this.addSection = function(section) 
	{
		sections[section.getName()] = section;
	}

	this.computeCounters = function()
	{
		infoCount = 0;
		warningsCount = 0;
		errorsCount = 0;

		Object.values(sections).forEach(function(section) {
			const health = section.getHealth();
			infoCount += health.info.length;
			warningsCount += health.warnings.length;
			errorsCount += health.errors.length;
		});
	}
}

const AppHealth = (new function($)
{
	'use strict';

	let $appHealthErrorsBadge;
	let $appHealthWarningsBadge;
	let $appHealthInfoBadge;
	let healthReport = {};
	this.SEVERITY = {
		OK: 0,
		INFO: 1,
		WARNING: 2,
		ERROR: 3,
	}
	const SEVERITY_STYLE = {
		INFO: "success",
		WARNING: "warning",
		ERROR: "important",
	}

	const statusHandler = 
	{
		update: function(status) 
		{
			healthReport = makeHealthReport(status["Health"]);
			redrawGlobalBadges(healthReport);
		}
	}

	this.init = function()
	{
		$appHealthInfoBadge = $("#AppHealthInfoBadge");
		$appHealthWarningsBadge = $("#AppHealthWarningsBadge");
		$appHealthErrorsBadge = $("#AppHealthErrorsBadge");
		Status.subscribe(statusHandler);
	}

	this.makeBadges = function(section)
	{
		if (!section) return null;

		const health = section.getHealth();
		const wrapper = $('<span style="margin-left: 5px;">');
		const $infoBadge = $('<span class="badge" style="margin-left: 3px;"></span>');
		const $warningsBadge = $('<span class="badge" style="margin-left: 3px;"></span>');
		const $errorsBadge = $('<span class="badge" style="margin-left: 3px;"></span>');
		const infoCount = health.info.length;
		const warningsCount = health.warnings.length;
		const errorsCount = health.errors.length;
		toggleBadgeVisibility($infoBadge, infoCount, SEVERITY_STYLE.INFO);
		toggleBadgeVisibility($warningsBadge, warningsCount, SEVERITY_STYLE.WARNING);
		toggleBadgeVisibility($errorsBadge, errorsCount, SEVERITY_STYLE.ERROR);
		wrapper.append($errorsBadge, $warningsBadge, $infoBadge);

		return wrapper;
	}

	this.getSection = function(name) 
	{
		return healthReport.getSection(name);
	}

	this.getChecks = function(section, name)
	{
		if (!section)
			return null;

		return section.getChecks(name);
	}

	function redrawGlobalBadges(healthReport)
	{
		const infoCount = healthReport.getInfoCount();
		const warningsCount = healthReport.getWarningsCount();
		const errorsCount = healthReport.getErrorsCount();
		toggleBadgeVisibility($appHealthInfoBadge, infoCount, SEVERITY_STYLE.INFO);
		toggleBadgeVisibility($appHealthWarningsBadge, warningsCount, SEVERITY_STYLE.WARNING);
		toggleBadgeVisibility($appHealthErrorsBadge, errorsCount, SEVERITY_STYLE.ERROR);
	}

	function makeHealthReport(appHealth) {
		let healthReport = new HealthReport();

		Object.entries(appHealth).forEach(function(entry) {
			const name = entry[0];
			const section = entry[1];
			healthReport.addSection(new SectionHealth(section, name.toUpperCase()));
		});

		healthReport.computeCounters();

		return healthReport;
	}

	function toggleBadgeVisibility($badge, checks, severity)
	{
		if (checks > 0)
		{
			$badge.show();
			$badge.addClass("badge-" + severity)
			$badge.text(checks);
		}
		else
		{
			$badge.hide();
		}
	}
	
}(jQuery));
