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

#include <fstream>
#include "PathSpecs.h"

namespace fs = boost::filesystem;
using namespace boost::system;

namespace HealthCheck::Specs
{
	namespace File
	{
		bool ExistsSpec::IsSatisfiedBy(std::string_view option) const
		{
			error_code ec;
			return fs::is_regular_file(option, ec) && !ec;
		}

		bool ReadableSpec::IsSatisfiedBy(std::string_view option) const
		{
#ifdef _WIN32
			std::ifstream file(option);
			return file.is_open();
#else
			return access(option.data(), R_OK) == 0;
#endif
		}

		bool WritableSpec::IsSatisfiedBy(std::string_view option) const
		{
#ifdef _WIN32
			std::ofstream file(option, std::ios::app);
			return file.is_open();
#else
			return access(option.data(), W_OK) == 0;
#endif
		}

		bool ExecutableSpec::IsSatisfiedBy(std::string_view option) const
		{
#ifdef _WIN32
			const fs::path path = fs::path(option);
			if (!path.has_extension()) return false;
			const std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return ext == ".exe" || ext == ".bat" || ext == ".cmd" || ext == ".com";
#else
			return access(option.data(), X_OK) == 0;
#endif
		}
	}

	namespace Directory
	{
		bool ExistsSpec::IsSatisfiedBy(std::string_view option) const
		{
			error_code ec;
			return fs::is_directory(option, ec);
		}

		bool WritableSpec::IsSatisfiedBy(std::string_view option) const
		{
			error_code ec;
			const auto path = fs::path(option) / fs::unique_path("%%%%-%%%%-%%%%-%%%%.tmp", ec);
			if (ec)
			{
				return false;
			}

			{
				std::ofstream testFile(path.string().c_str());
				if (!testFile.is_open())
				{
					return false;
				}
				testFile << "This file was created to verify if this directory is writable.\nFeel free to delete it.";

				if (testFile.fail())
				{
					return false;
				}
			}

			fs::remove(path, ec);
			if (ec)
			{
				// If we can't delete our own temp file, the directory is problematic.
				return false;
			}

			return true;
		}

		bool ReadableSpec::IsSatisfiedBy(std::string_view option) const
		{
			error_code ec;
			fs::directory_iterator(option, ec);
			return !ec;
		}
	}
}
