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

#include <initializer_list>

#include <blooto/bitboard.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_bitboard
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_bitboard) {
    blooto::BitBoard empty_bb;
    BOOST_CHECK(!empty_bb[blooto::Square::A1]);
    BOOST_CHECK(empty_bb.data() == 0);
    std::initializer_list<blooto::Square> empty_il{};
    BOOST_CHECK_EQUAL_COLLECTIONS(empty_bb.begin(), empty_bb.end(),
                                  empty_il.begin(), empty_il.end());
    blooto::BitBoard all_bb(~empty_bb);
    BOOST_CHECK(all_bb[blooto::Square::A1]);
    blooto::BitBoard d4_bb(blooto::Square::D4);
    BOOST_CHECK(!d4_bb[blooto::Square::A1]);
    BOOST_CHECK(d4_bb[blooto::Square::D4]);
    std::initializer_list<blooto::Square> d4_il{blooto::Square::D4};
    BOOST_CHECK_EQUAL_COLLECTIONS(d4_bb.begin(), d4_bb.end(),
                                  d4_il.begin(), d4_il.end());
    blooto::BitBoard b2_f6_bb(blooto::Square::B2 | blooto::Square::F6);
    BOOST_CHECK(!b2_f6_bb[blooto::Square::A1]);
    BOOST_CHECK(!b2_f6_bb[blooto::Square::D4]);
    BOOST_CHECK(b2_f6_bb[blooto::Square::B2]);
    BOOST_CHECK(b2_f6_bb[blooto::Square::F6]);
    std::initializer_list<blooto::Square> b2_f6_il{
        blooto::Square::B2,
        blooto::Square::F6
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(b2_f6_bb.begin(), b2_f6_bb.end(),
                                  b2_f6_il.begin(), b2_f6_il.end());
}
