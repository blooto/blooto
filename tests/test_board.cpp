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

#include <blooto/board.hpp>
#include <blooto/square.hpp>
#include <blooto/piecetype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/queentype.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_board
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_board) {
    using namespace blooto;
    std::initializer_list<Piece> pieces{
        {Square::B1, BishopType::instance, ColourNeutral()},
        {Square::D3, BishopType::instance, ColourWhite()},
        {Square::F5, BishopType::instance, ColourBlack()},
    };
    std::initializer_list<Piece> pieces_can_move{
        {Square::B1, BishopType::instance, ColourNeutral()},
        {Square::D3, BishopType::instance, ColourWhite()},
    };
    Board board{pieces};
    BOOST_CHECK(!board.empty());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  pieces.begin(), pieces.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move_begin(), board.can_move_end(),
                                  pieces_can_move.begin(),
                                  pieces_can_move.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move().begin(),
                                  board.can_move().end(),
                                  pieces_can_move.begin(),
                                  pieces_can_move.end());
    board.insert(Piece{Square::E2, RookType::instance, ColourBlack()});
    BOOST_CHECK(!board.empty());
    std::initializer_list<Piece> pieces2{
        {Square::B1, BishopType::instance, ColourNeutral()},
        {Square::E2, RookType::instance, ColourBlack()},
        {Square::D3, BishopType::instance, ColourWhite()},
        {Square::F5, BishopType::instance, ColourBlack()},
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  pieces2.begin(), pieces2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move_begin(), board.can_move_end(),
                                  pieces_can_move.begin(),
                                  pieces_can_move.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move().begin(),
                                  board.can_move().end(),
                                  pieces_can_move.begin(),
                                  pieces_can_move.end());
    BOOST_CHECK_EQUAL(board[Square::B1],
                      (Piece{Square::B1,
                             BishopType::instance, 
                             ColourNeutral()}));
    BOOST_CHECK_EQUAL(board[Square::E2],
                      (Piece{Square::E2,
                             RookType::instance,
                             ColourBlack()}));
    BOOST_CHECK_EQUAL(board[Square::D3],
                      (Piece{Square::D3,
                             BishopType::instance,
                             ColourWhite()}));
    BOOST_CHECK_EQUAL(board[Square::F5],
                      (Piece{Square::F5,
                             BishopType::instance,
                             ColourBlack()}));
}
