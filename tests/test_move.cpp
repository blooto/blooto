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

#include <string>
#include <boost/lexical_cast.hpp>

#include <blooto/move.hpp>
#include <blooto/piecetype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/queentype.hpp>
#include <blooto/pawntype.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_move
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_move) {
    using namespace blooto;
    Move m1{BishopType::instance, Square::B2, Square::D4};
    BOOST_CHECK_EQUAL(&m1.piecetype(), &BishopType::instance);
    BOOST_CHECK_EQUAL(m1.from(), Square::B2);
    BOOST_CHECK_EQUAL(m1.to(), Square::D4);
    BOOST_CHECK(!m1.attack());
    BOOST_CHECK(m1.promotion() == nullptr);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(m1), "Bb2-d4");
    Move m2{RookType::instance, Square::D3, Square::D7, true};
    BOOST_CHECK_EQUAL(&m2.piecetype(), &RookType::instance);
    BOOST_CHECK_EQUAL(m2.from(), Square::D3);
    BOOST_CHECK_EQUAL(m2.to(), Square::D7);
    BOOST_CHECK(m2.attack());
    BOOST_CHECK(m2.promotion() == nullptr);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(m2), "Rd3*d7");
    Move m3{
        PawnType::instance, Square::E7, Square::E8,
        false, &QueenType::instance
    };
    BOOST_CHECK_EQUAL(&m3.piecetype(), &PawnType::instance);
    BOOST_CHECK_EQUAL(m3.from(), Square::E7);
    BOOST_CHECK_EQUAL(m3.to(), Square::E8);
    BOOST_CHECK(!m3.attack());
    BOOST_CHECK_EQUAL(m3.promotion(), &QueenType::instance);
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(m3), "Pe7-e8=Q");
}
