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
#include <string>
#include <boost/lexical_cast.hpp>

#include <blooto/board.hpp>
#include <blooto/square.hpp>
#include <blooto/piecetype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/queentype.hpp>
#include <blooto/kingtype.hpp>
#include <blooto/move.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_board
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_board) {
    using namespace blooto;
    std::initializer_list<Piece> pieces{
        {Square::B1, BishopType::instance, ColourNeutral()},
        {Square::D3, BishopType::instance, ColourWhite()},
        {Square::F5, BishopType::instance, ColourBlack()},
        {Square::H7, KingType::instance, ColourWhite()},
    };
    std::initializer_list<Square> occupied{
        Square::B1, Square::D3, Square::F5, Square::H7
    };
    std::initializer_list<Square> friendlies{Square::D3, Square::H7};
    std::initializer_list<Square> pieces_can_move{
        Square::B1, Square::D3, Square::H7
    };
    Board board{pieces};
    BOOST_CHECK(!board.empty());
    BOOST_CHECK_EQUAL(board.colour(), ColourWhite());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  pieces.begin(), pieces.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.occupied().begin(),
                                  board.occupied().end(),
                                  occupied.begin(),
                                  occupied.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.friendlies().begin(),
                                  board.friendlies().end(),
                                  friendlies.begin(),
                                  friendlies.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move().begin(),
                                  board.can_move().end(),
                                  pieces_can_move.begin(),
                                  pieces_can_move.end());
    std::string board_str1{"Neutral Bb1 White Bd3 Kh7 Black Bf5"};
    Board board_expected1{boost::lexical_cast<Board>(board_str1)};
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  board_expected1.begin(),
                                  board_expected1.end());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(board), board_str1);
    std::initializer_list<Move> moves1{
        {BishopType::instance, Square::B1, Square::A2},
        {BishopType::instance, Square::B1, Square::C2},
        {BishopType::instance, Square::D3, Square::B1, true},
        {BishopType::instance, Square::D3, Square::F1},
        {BishopType::instance, Square::D3, Square::C2},
        {BishopType::instance, Square::D3, Square::E2},
        {BishopType::instance, Square::D3, Square::C4},
        {BishopType::instance, Square::D3, Square::E4},
        {BishopType::instance, Square::D3, Square::B5},
        {BishopType::instance, Square::D3, Square::F5, true},
        {BishopType::instance, Square::D3, Square::A6},
        {KingType::instance, Square::H7, Square::G6},
        {KingType::instance, Square::H7, Square::H6},
        {KingType::instance, Square::H7, Square::G7},
        {KingType::instance, Square::H7, Square::G8},
        {KingType::instance, Square::H7, Square::H8},
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves_begin(), board.moves_end(),
                                  moves1.begin(), moves1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves().begin(), board.moves().end(),
                                  moves1.begin(), moves1.end());
    board.insert(Piece{Square::E2, RookType::instance, ColourBlack()});
    BOOST_CHECK(!board.empty());
    std::initializer_list<Piece> pieces2{
        {Square::B1, BishopType::instance, ColourNeutral()},
        {Square::E2, RookType::instance, ColourBlack()},
        {Square::D3, BishopType::instance, ColourWhite()},
        {Square::F5, BishopType::instance, ColourBlack()},
        {Square::H7, KingType::instance, ColourWhite()},
    };
    std::initializer_list<Square> occupied2{
        Square::B1, Square::E2, Square::D3, Square::F5, Square::H7
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  pieces2.begin(), pieces2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.occupied().begin(),
                                  board.occupied().end(),
                                  occupied2.begin(),
                                  occupied2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.friendlies().begin(),
                                  board.friendlies().end(),
                                  friendlies.begin(),
                                  friendlies.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move().begin(),
                                  board.can_move().end(),
                                  pieces_can_move.begin(),
                                  pieces_can_move.end());
    std::string board_str2{"Neutral Bb1 White Bd3 Kh7 Black Re2 Bf5"};
    Board board_expected2{boost::lexical_cast<Board>(board_str2)};
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  board_expected2.begin(),
                                  board_expected2.end());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(board), board_str2);
    std::initializer_list<Move> moves2{
        {BishopType::instance, Square::B1, Square::A2},
        {BishopType::instance, Square::B1, Square::C2},
        {BishopType::instance, Square::D3, Square::B1, true},
        {BishopType::instance, Square::D3, Square::C2},
        {BishopType::instance, Square::D3, Square::E2, true},
        {BishopType::instance, Square::D3, Square::C4},
        {BishopType::instance, Square::D3, Square::E4},
        {BishopType::instance, Square::D3, Square::B5},
        {BishopType::instance, Square::D3, Square::F5, true},
        {BishopType::instance, Square::D3, Square::A6},
        {KingType::instance, Square::H7, Square::G6},
        {KingType::instance, Square::H7, Square::H6},
        {KingType::instance, Square::H7, Square::G7},
        {KingType::instance, Square::H7, Square::G8},
        {KingType::instance, Square::H7, Square::H8},
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves_begin(), board.moves_end(),
                                  moves2.begin(), moves2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves().begin(), board.moves().end(),
                                  moves2.begin(), moves2.end());
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
    BOOST_CHECK_EQUAL(board[Square::H7],
                      (Piece{Square::H7,
                          KingType::instance,
                       ColourWhite()}));
    BOOST_CHECK_EQUAL(board.make_move({BishopType::instance, Square::D3,
                                       Square::F5, true}),
                      &BishopType::instance);
    std::initializer_list<Piece> pieces3{
        {Square::B1, BishopType::instance, ColourNeutral()},
        {Square::E2, RookType::instance, ColourBlack()},
        {Square::F5, BishopType::instance, ColourWhite()},
        {Square::H7, KingType::instance, ColourWhite()},
    };
    std::initializer_list<Square> occupied3{
        Square::B1, Square::E2, Square::F5, Square::H7
    };
    std::initializer_list<Square> friendlies3{Square::F5, Square::H7};
    std::initializer_list<Square> can_move3{
        Square::B1, Square::F5, Square::H7
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  pieces3.begin(), pieces3.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.occupied().begin(),
                                  board.occupied().end(),
                                  occupied3.begin(),
                                  occupied3.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.friendlies().begin(),
                                  board.friendlies().end(),
                                  friendlies3.begin(),
                                  friendlies3.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move().begin(),
                                  board.can_move().end(),
                                  can_move3.begin(),
                                  can_move3.end());
    std::string board_str3{"Neutral Bb1 White Bf5 Kh7 Black Re2"};
    Board board_expected3{boost::lexical_cast<Board>(board_str3)};
    BOOST_CHECK_EQUAL_COLLECTIONS(board.begin(), board.end(),
                                  board_expected3.begin(),
                                  board_expected3.end());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(board), board_str3);
    std::initializer_list<Move> moves3{
        {BishopType::instance, Square::B1, Square::A2},
        {BishopType::instance, Square::B1, Square::C2},
        {BishopType::instance, Square::B1, Square::D3},
        {BishopType::instance, Square::B1, Square::E4},
        {BishopType::instance, Square::F5, Square::B1, true},
        {BishopType::instance, Square::F5, Square::C2},
        {BishopType::instance, Square::F5, Square::D3},
        {BishopType::instance, Square::F5, Square::H3},
        {BishopType::instance, Square::F5, Square::E4},
        {BishopType::instance, Square::F5, Square::G4},
        {BishopType::instance, Square::F5, Square::E6},
        {BishopType::instance, Square::F5, Square::G6},
        {BishopType::instance, Square::F5, Square::D7},
        {BishopType::instance, Square::F5, Square::C8},
        {KingType::instance, Square::H7, Square::G6},
        {KingType::instance, Square::H7, Square::H6},
        {KingType::instance, Square::H7, Square::G7},
        {KingType::instance, Square::H7, Square::G8},
        {KingType::instance, Square::H7, Square::H8},
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves_begin(), board.moves_end(),
                                  moves3.begin(), moves3.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves().begin(), board.moves().end(),
                                  moves3.begin(), moves3.end());
    BOOST_CHECK_EQUAL(board[Square::B1],
                      (Piece{Square::B1,
                          BishopType::instance,
                       ColourNeutral()}));
    BOOST_CHECK_EQUAL(board[Square::E2],
                      (Piece{Square::E2,
                          RookType::instance,
                       ColourBlack()}));
    BOOST_CHECK_EQUAL(board[Square::F5],
                      (Piece{Square::F5,
                          BishopType::instance,
                       ColourWhite()}));
    board.flip_colour();
    std::initializer_list<Square> friendlies4{Square::E2};
    std::initializer_list<Square> can_move4{Square::B1, Square::E2};
    BOOST_CHECK_EQUAL_COLLECTIONS(board.occupied().begin(),
                                  board.occupied().end(),
                                  occupied3.begin(),
                                  occupied3.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.friendlies().begin(),
                                  board.friendlies().end(),
                                  friendlies4.begin(),
                                  friendlies4.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.can_move().begin(),
                                  board.can_move().end(),
                                  can_move4.begin(),
                                  can_move4.end());
    std::initializer_list<Move> moves4{
        {BishopType::instance, Square::B1, Square::A2},
        {BishopType::instance, Square::B1, Square::C2},
        {BishopType::instance, Square::B1, Square::D3},
        {BishopType::instance, Square::B1, Square::E4},
        {BishopType::instance, Square::B1, Square::F5, true},
        {RookType::instance, Square::E2, Square::E1},
        {RookType::instance, Square::E2, Square::A2},
        {RookType::instance, Square::E2, Square::B2},
        {RookType::instance, Square::E2, Square::C2},
        {RookType::instance, Square::E2, Square::D2},
        {RookType::instance, Square::E2, Square::F2},
        {RookType::instance, Square::E2, Square::G2},
        {RookType::instance, Square::E2, Square::H2},
        {RookType::instance, Square::E2, Square::E3},
        {RookType::instance, Square::E2, Square::E4},
        {RookType::instance, Square::E2, Square::E5},
        {RookType::instance, Square::E2, Square::E6},
        {RookType::instance, Square::E2, Square::E7},
        {RookType::instance, Square::E2, Square::E8},
    };
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves_begin(), board.moves_end(),
                                  moves4.begin(), moves4.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(board.moves().begin(), board.moves().end(),
                                  moves4.begin(), moves4.end());
}
