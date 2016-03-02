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

#include <blooto/kingtype.hpp>
#include <blooto/bitboard.hpp>
#include <blooto/square.hpp>

constexpr static inline blooto::BitBoard M(blooto::Square sq) {
    using namespace blooto;
    return
        (rank(sq) > 0 ?
         BitBoard{square(file(sq), rank(sq) - 1)} :
         BitBoard{}) |
        (rank(sq) > 0 && file(sq) > 0 ?
         BitBoard{square(file(sq) - 1, rank(sq) - 1)} :
         BitBoard{}) |
        (file(sq) > 0 ?
         BitBoard{square(file(sq) - 1, rank(sq))} :
         BitBoard{}) |
        (rank(sq) < 7 && file(sq) > 0 ?
         BitBoard{square(file(sq) - 1, rank(sq) + 1)} :
         BitBoard{}) |
        (rank(sq) < 7 ?
         BitBoard{square(file(sq), rank(sq) + 1)} :
         BitBoard{}) |
        (rank(sq) < 7 && file(sq) < 7 ?
         BitBoard{square(file(sq) + 1, rank(sq) + 1)} :
         BitBoard{}) |
        (file(sq) < 7 ?
         BitBoard{square(file(sq) + 1, rank(sq))} :
         BitBoard{}) |
        (rank(sq) > 0 && file(sq) < 7 ?
         BitBoard{square(file(sq) + 1, rank(sq) - 1)} :
         BitBoard{});
}

blooto::BitBoard blooto::KingType::moves_for_square[64] = {
    M(Square::A1), M(Square::B1), M(Square::C1), M(Square::D1),
    M(Square::E1), M(Square::F1), M(Square::G1), M(Square::H1),
    M(Square::A2), M(Square::B2), M(Square::C2), M(Square::D2),
    M(Square::E2), M(Square::F2), M(Square::G2), M(Square::H2),
    M(Square::A3), M(Square::B3), M(Square::C3), M(Square::D3),
    M(Square::E3), M(Square::F3), M(Square::G3), M(Square::H3),
    M(Square::A4), M(Square::B4), M(Square::C4), M(Square::D4),
    M(Square::E4), M(Square::F4), M(Square::G4), M(Square::H4),
    M(Square::A5), M(Square::B5), M(Square::C5), M(Square::D5),
    M(Square::E5), M(Square::F5), M(Square::G5), M(Square::H5),
    M(Square::A6), M(Square::B6), M(Square::C6), M(Square::D6),
    M(Square::E6), M(Square::F6), M(Square::G6), M(Square::H6),
    M(Square::A7), M(Square::B7), M(Square::C7), M(Square::D7),
    M(Square::E7), M(Square::F7), M(Square::G7), M(Square::H7),
    M(Square::A8), M(Square::B8), M(Square::C8), M(Square::D8),
    M(Square::E8), M(Square::F8), M(Square::G8), M(Square::H8)
};
