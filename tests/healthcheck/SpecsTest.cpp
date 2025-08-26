/*
 *  This file is part of nzbget. See <https://nzbget.com>.
 *
 *  Copyright (C) 2015-2016 Andrey Prygunkov <hugbug@users.sourceforge.net>
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
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "nzbget.h"

#include "boost/test/unit_test.hpp"

#include <fstream>
#include "HealthCheck.h"

namespace fs = boost::filesystem;
using namespace HealthCheck::Specs;

struct SpecsFixture 
{
	fs::path m_baseTempdir;
	fs::path m_writableDir;
	fs::path m_readonlyDir;
	fs::path m_nonExistentPath;

	fs::path m_writableFile;
	fs::path m_readonlyFile;
	fs::path m_executableFile;

	SpecsFixture()
	 {
		m_baseTempdir = fs::temp_directory_path() / fs::unique_path();
		fs::create_directory(m_baseTempdir);

		m_writableDir = m_baseTempdir / "writableDir";
		fs::create_directory(m_writableDir);

		m_readonlyDir = m_baseTempdir / "readonlyDir";
		fs::create_directory(m_readonlyDir);

		m_nonExistentPath = m_baseTempdir / "does_not_exist";

		m_writableFile = m_writableDir / "writable.txt";
		std::ofstream(m_writableFile.string()) << "content";

		m_readonlyFile = m_writableDir / "readonly.txt";
		std::ofstream(m_readonlyFile.string()) << "content";

		m_executableFile = m_writableDir / "executable.sh";
		std::ofstream(m_executableFile.string()) << "#!/bin/sh\necho hello";

		fs::permissions(m_readonlyDir, fs::perms::owner_read);
		fs::permissions(m_readonlyFile, fs::perms::owner_read);
		fs::permissions(m_executableFile, fs::perms::owner_exe);
	}
	~SpecsFixture() 
	{
		fs::permissions(m_readonlyDir, fs::perms::owner_all);
		fs::remove_all(m_baseTempdir);
	}
};

BOOST_AUTO_TEST_SUITE(FileSystemSpecs)

BOOST_FIXTURE_TEST_CASE(DirectoryExistsSpecTest, SpecsFixture)
{
	Directory::ExistsSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(m_writableDir.string()));
	BOOST_CHECK(spec.IsSatisfiedBy(m_readonlyDir.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_writableFile.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_nonExistentPath.string()));
}

BOOST_FIXTURE_TEST_CASE(DirectoryIsReadableSpecTest, SpecsFixture)
{
	Directory::ReadableSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(m_writableDir.string()));
	BOOST_CHECK(spec.IsSatisfiedBy(m_readonlyDir.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_nonExistentPath.string()));
}

BOOST_FIXTURE_TEST_CASE(DirectoryIsWritableSpecTest, SpecsFixture)
{
	Directory::WritableSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(m_writableDir.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_readonlyDir.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_nonExistentPath.string()));
}

BOOST_FIXTURE_TEST_CASE(FileIsRegularSpecTest, SpecsFixture)
{
	File::ExistsSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(m_writableFile.string()));
	BOOST_CHECK(spec.IsSatisfiedBy(m_readonlyFile.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_writableDir.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_nonExistentPath.string()));
}

BOOST_FIXTURE_TEST_CASE(FileIsWritableSpecTest, SpecsFixture)
{
	File::WritableSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(m_writableFile.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_readonlyFile.string()));
}

BOOST_FIXTURE_TEST_CASE(FileIsExecutableSpecTest, SpecsFixture)
{
	File::ExecutableSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(m_executableFile.string()));
	BOOST_CHECK(!spec.IsSatisfiedBy(m_writableFile.string()));
}

BOOST_FIXTURE_TEST_CASE(EmptyOptionSpecTest, SpecsFixture)
{
	EmptyOptionSpec spec;

	BOOST_CHECK(spec.IsSatisfiedBy(""));
	BOOST_CHECK(!spec.IsSatisfiedBy("not empty"));
}

BOOST_AUTO_TEST_SUITE_END()
