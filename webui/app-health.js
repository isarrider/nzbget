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

const AppHealth = (new function($)
{
	'use strict';

	let $appHealthErrorsBadge;
	let $appHealthWarningsBadge;
	let appHealth;

	const SEVERITY = {
		WARNING: "warning",
		ERROR: "important",
	}

	const statusHandler = 
	{
		update: function(status) 
		{
			appHealth = status["Health"];
			redrawBadge(appHealth);
		}
	}

	this.init = function()
	{
		$appHealthErrorsBadge = $("#AppHealthErrorsBadge");
		$appHealthWarningsBadge = $("#AppHealthWarningsBadge");
		Status.subscribe(statusHandler);
	}

	function redrawBadge(appHealth)
	{
		const results = appHealth["Results"];
		const warnings = [];
		const errors = [];

		for (const key in results) {
			if (Object.prototype.hasOwnProperty.call(results, key)) {
				const checks = results[key];

				checks.forEach(function(check) {
					if (check.Status == 1)
					{
						warnings.push(check);
					}

					if (check.Status == 2)
					{
						errors.push(check);
					}
				});
			}
		}

		toggleBadgeVisibility($appHealthWarningsBadge, warnings, SEVERITY.WARNING);
		toggleBadgeVisibility($appHealthErrorsBadge, errors, SEVERITY.ERROR);
	}

	function toggleBadgeVisibility($badge, checks, severity)
	{
		if (checks.length > 0)
		{
			$badge.show();
			$badge.addClass("badge-" + severity)
			$badge.text(checks.length);
		}
		else
		{
			$badge.hide();
		}
	}
	
}(jQuery));
