// This file is part of Blooto.
//
// Blooto is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Blooto is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <blooto/bitscan.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_bitscan
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_bitscan) {
    using namespace blooto;
    std::uint64_t n1 = 1ULL;
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n1), 0);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n1), 0);
    std::uint64_t n2 = 2ULL;
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n2), 1);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n2), 1);
    std::uint64_t n3 = 10ULL;
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n3), 1);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n3), 1);
    std::uint64_t n4 = 0x1008020400802300ULL;
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 8);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 8);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 9);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 9);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 13);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4),13);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 23);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 23);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 34);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 34);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 41);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 41);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 51);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 51);
    n4 &= n4 ^ (0 - n4);
    BOOST_REQUIRE(n4 != 0ULL);
    BOOST_CHECK_EQUAL(BitScanGeneric::ls1b(n4), 60);
    BOOST_CHECK_EQUAL(BitScanOptimised::ls1b(n4), 60);
    n4 &= n4 ^ (0 - n4);
    BOOST_CHECK_EQUAL(n4, 0ULL);
}
