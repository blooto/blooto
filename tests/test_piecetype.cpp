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

#include <blooto/piecetype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/queentype.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_piecetype
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_piecetype) {
    using namespace blooto;
    const PieceType *b = PieceType::get("B");
    BOOST_CHECK_EQUAL(b, &BishopType::instance);
    BOOST_CHECK_EQUAL(b->code(), "B");
    const PieceType *r = PieceType::get("R");
    BOOST_CHECK_EQUAL(r, &RookType::instance);
    BOOST_CHECK_EQUAL(r->code(), "R");
    const PieceType *q = PieceType::get("Q");
    BOOST_CHECK_EQUAL(q, &QueenType::instance);
    BOOST_CHECK_EQUAL(q->code(), "Q");
}
