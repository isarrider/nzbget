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
#include "Check.h"

namespace HealthCheck
{
	namespace File
	{
		Check Exists(const fs::path& path)
		{
			boost::system::error_code ec;
			if (!fs::is_regular_file(path, ec))
			{
				return Check::Error(path.string() + ": " + ec.message());
			}

			return Check::Ok();
		}

		Check IsWritable(const fs::path& path)
		{
			std::ofstream file(path.c_str(), std::ios::app);
			if (!file.is_open())
			{
				return Check::Error(path.string() + " is not writeable.");
			}

			return Check::Ok();
		}

		Check IsReadable(const fs::path& path)
		{
			std::ifstream file(path.c_str(), std::ios::app);
			if (!file.is_open())
			{
				return Check::Error(path.string() + " is not readable.");
			}

			return Check::Ok();
		}
	}

	namespace Directory
	{
		Check Exists(const fs::path& path)
		{
			boost::system::error_code ec;
			if (!fs::is_directory(path, ec))
			{
				return Check::Error(path.string() + ": " + ec.message());
			}

			return Check::Ok();
		}

		Check IsWritable(const fs::path& path)
		{
			const auto filePath = path / "nzbget_write_test.tmp";
			std::ofstream file(filePath.c_str());
			if (!file.is_open())
				return Check::Error(path.string() + " directory is not writable.");

			file << "This file was created to verify if this directory is writable. It should've been automatically deleted. Feel free to delete it.";
			file.close();

			fs::remove(filePath);

			return Check::Ok();
		}
	}
}
