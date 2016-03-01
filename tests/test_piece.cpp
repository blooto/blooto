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

#include <blooto/piece.hpp>
#include <blooto/square.hpp>
#include <blooto/colour.hpp>
#include <blooto/piecetype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/queentype.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_piece
#include <boost/test/unit_test.hpp>

class ExpectWhat {
    std::string expected_;
public:
    ExpectWhat(const std::string &expected): expected_(expected) {}
    bool operator()(const std::exception &e) const {
        return e.what() == expected_;
    }
};

BOOST_AUTO_TEST_CASE(test_piece) {
    using namespace blooto;
    Piece p1{Square::D3, BishopType::instance, ColourWhite()};
    BOOST_CHECK_EQUAL(p1.square(), Square::D3);
    BOOST_CHECK_EQUAL(&p1.piecetype(), &BishopType::instance);
    BOOST_CHECK_EQUAL(p1.colour(), ColourWhite());
    BOOST_CHECK_EQUAL(p1, Piece::from_code("Bd3", ColourWhite()));
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p1), "Bd3");
    BitBoard p1_moves1 = p1.moves({});
    BitBoard p1_moves1_expected{
        Square::B1 | Square::C2 | /* D3 */
        Square::E4 | Square::F5 | Square::G6 | Square::H7 |
        Square::A6 | Square::B5 | Square::C4 | /* D3 */
        Square::E2 | Square::F1
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(p1_moves1.begin(), p1_moves1.end(),
                                  p1_moves1_expected.begin(),
                                  p1_moves1_expected.end());
    BitBoard p1_moves2 = p1.moves({Square::F5 | Square::B5});
    BitBoard p1_moves2_expected{
        Square::B1 | Square::C2 | /* D3 */
        Square::E4 | Square::F5 | /* ... */
        /* ... */ Square::B5 | Square::C4 | /* D3 */
        Square::E2 | Square::F1
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(p1_moves2.begin(), p1_moves2.end(),
                                  p1_moves2_expected.begin(),
                                  p1_moves2_expected.end());
    Piece p2{Piece::from_code("Rb1", ColourNeutral())};
    BOOST_CHECK_EQUAL(p2.square(), Square::B1);
    BOOST_CHECK_EQUAL(&p2.piecetype(), &RookType::instance);
    BOOST_CHECK_EQUAL(p2.colour(), ColourNeutral());
    BOOST_CHECK_EQUAL(p2,
                      (Piece{Square::B1, RookType::instance, ColourNeutral()}));
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p2), "Rb1");
    BitBoard p2_moves1 = p2.moves({});
    BitBoard p2_moves1_expected{
        Square::A1 | Square::C1 | Square::D1 | Square::E1 | Square::F1 |
        Square::G1 | Square::H1 | Square::B2 | Square::B3 | Square::B4 |
        Square::B5 | Square::B6 | Square::B7 | Square::B8
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(p2_moves1.begin(), p2_moves1.end(),
                                  p2_moves1_expected.begin(),
                                  p2_moves1_expected.end());
    BitBoard p2_moves2 = p2.moves({Square::B3 | Square::E1});
    BitBoard p2_moves2_expected{
        Square::A1 | Square::C1 | Square::D1 | Square::E1 |
        Square::B2 | Square::B3
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(p2_moves2.begin(), p2_moves2.end(),
                                  p2_moves2_expected.begin(),
                                  p2_moves2_expected.end());
    Piece p3(Square::A1, QueenType::instance, ColourBlack());
    BOOST_CHECK_EQUAL(p3.square(), Square::A1);
    BOOST_CHECK_EQUAL(&p3.piecetype(), &QueenType::instance);
    BOOST_CHECK_EQUAL(p3.colour(), ColourBlack());
    BOOST_CHECK_EQUAL(p3, Piece::from_code("Qa1", ColourBlack()));
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(p3), "Qa1");
    BitBoard p3_moves1 = p3.moves({});
    BitBoard p3_moves1_expected{
        Square::B1 | Square::C1 | Square::D1 | Square::E1 | Square::F1 |
        Square::G1 | Square::H1 | Square::A2 | Square::A3 | Square::A4 |
        Square::A5 | Square::A6 | Square::A7 | Square::A8 | Square::B2 |
        Square::C3 | Square::D4 | Square::E5 | Square::F6 | Square::G7 |
        Square::H8
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(p3_moves1.begin(), p3_moves1.end(),
                                  p3_moves1_expected.begin(),
                                  p3_moves1_expected.end());
    BitBoard p3_moves2 = p3.moves({Square::A3 | Square::C3 | Square::C1});
    BitBoard p3_moves2_expected{
        Square::B1 | Square::C1 | Square::A2 | Square::A3 | Square::B2 |
        Square::C3
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(p3_moves2.begin(), p3_moves2.end(),
                                  p3_moves2_expected.begin(),
                                  p3_moves2_expected.end());
    BOOST_CHECK_EXCEPTION(Piece::from_code("", ColourNeutral()),
                          Piece::ParseError, ExpectWhat("Empty piece code: "));
    BOOST_CHECK_EXCEPTION(Piece::from_code("1", ColourBlack()),
                          Piece::ParseError, ExpectWhat("Empty piece code: 1"));
    BOOST_CHECK_EXCEPTION(Piece::from_code("R", ColourWhite()),
                          Piece::ParseError,
                          ExpectWhat("Empty piece position: R"));
    BOOST_CHECK_EXCEPTION(Piece::from_code("Xc2", ColourNeutral()),
                          Piece::ParseError,
                          ExpectWhat("Unknown piece code: Xc2"));
    BOOST_CHECK_EXCEPTION(Piece::from_code("Qc0", ColourBlack()),
                          Piece::ParseError,
                          ExpectWhat("Wrong piece position: Qc0"));
    BOOST_CHECK_EXCEPTION(Piece::from_code("Qq1", ColourWhite()),
                          Piece::ParseError,
                          ExpectWhat("Wrong piece position: Qq1"));
}