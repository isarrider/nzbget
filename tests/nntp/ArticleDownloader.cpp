/*
 *  This file is part of nzbget. See <https://nzbget.com>.
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

#include <boost/test/unit_test.hpp>
#include "ArticleDownloader.h"

BOOST_AUTO_TEST_SUITE(NNTPTest)

BOOST_AUTO_TEST_CASE(ReadChunkSizeTest)
{
	const int minSize = ArticleDownloader::READ_CHUNK_MIN;
	const int maxSize = ArticleDownloader::READ_CHUNK_MAX;

	// Typical articles are larger than the cap.
	BOOST_CHECK_EQUAL(ArticleDownloader::CalcReadChunkSize(750000), maxSize);
	// Small articles get a buffer of their own size...
	BOOST_CHECK_EQUAL(ArticleDownloader::CalcReadChunkSize(100000), 100000);
	// ...but never less than one TLS record.
	BOOST_CHECK_EQUAL(ArticleDownloader::CalcReadChunkSize(1000), minSize);
	// Unknown article size.
	BOOST_CHECK_EQUAL(ArticleDownloader::CalcReadChunkSize(0), maxSize);
	BOOST_CHECK_EQUAL(ArticleDownloader::CalcReadChunkSize(-1), maxSize);
}

BOOST_AUTO_TEST_SUITE_END()
