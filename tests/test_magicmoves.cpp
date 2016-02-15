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

#include <blooto/magicmoves.hpp>
#include <blooto/square.hpp>
#include <blooto/bitboard.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_magicmoves
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_magicmoves) {
    using namespace blooto;
    initmagicmoves();
    BitBoard bb1{Rmagic(Square::A1, {})};
    BitBoard bb1e{
        Square::A2 | Square::A3 | Square::A4 | Square::A5 |
        Square::A6 | Square::A7 | Square::A8 |
        Square::B1 | Square::C1 | Square::D1 | Square::E1 |
        Square::F1 | Square::G1 | Square::H1
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb1.begin(), bb1.end(),
                                  bb1e.begin(), bb1e.end());
    BitBoard bb2{Rmagic(Square::A1, {Square::A2 | Square::B2})};
    BitBoard bb2e{
        Square::A2 |
        Square::B1 | Square::C1 | Square::D1 | Square::E1 |
        Square::F1 | Square::G1 | Square::H1
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb2.begin(), bb2.end(),
                                  bb2e.begin(), bb2e.end());
    BitBoard bb3{Rmagic(Square::D3, {})};
    BitBoard bb3e{
        Square::D1 | Square::D2 | Square::D4 | Square::D5 |
        Square::D6 | Square::D7 | Square::D8 |
        Square::A3 | Square::B3 | Square::C3 | Square::E3 |
        Square::F3 | Square::G3 | Square::H3
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb3.begin(), bb3.end(),
                                  bb3e.begin(), bb3e.end());
    BitBoard bb4{Rmagic(Square::D3, {Square::D5 | Square::F3})};
    BitBoard bb4e{
        Square::D1 | Square::D2 | Square::D4 | Square::D5 |
        Square::A3 | Square::B3 | Square::C3 | Square::E3 |
        Square::F3
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb4.begin(), bb4.end(),
                                  bb4e.begin(), bb4e.end());
    BitBoard bb5{Bmagic(Square::A1, {})};
    BitBoard bb5e{
        Square::B2 | Square::C3 | Square::D4 | Square::E5 |
        Square::F6 | Square::G7 | Square::H8
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb5.begin(), bb5.end(),
                                  bb5e.begin(), bb5e.end());
    BitBoard bb6{Bmagic(Square::A1, BitBoard{Square::C3})};
    BitBoard bb6e{Square::B2 | Square::C3};
    BOOST_CHECK_EQUAL_COLLECTIONS(bb6.begin(), bb6.end(),
                                  bb6e.begin(), bb6e.end());
    BitBoard bb7{Bmagic(Square::D3, {})};
    BitBoard bb7e{
        Square::B1 | Square::C2 | /* D3 */
        Square::E4 | Square::F5 | Square::G6 | Square::H7 |
        Square::A6 | Square::B5 | Square::C4 | /* D3 */
        Square::E2 | Square::F1
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb7.begin(), bb7.end(),
                                  bb7e.begin(), bb7e.end());
    BitBoard bb8{Bmagic(Square::D3, {Square::F5 | Square::B5})};
    BitBoard bb8e{
        Square::B1 | Square::C2 | /* D3 */
        Square::E4 | Square::F5 | /* ... */
        /* ... */ Square::B5 | Square::C4 | /* D3 */
        Square::E2 | Square::F1
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(bb8.begin(), bb8.end(),
                                  bb8e.begin(), bb8e.end());
}
