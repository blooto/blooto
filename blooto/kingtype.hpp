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

#ifndef _BLOOTO_KINGTYPE_HPP
#define _BLOOTO_KINGTYPE_HPP

#include <blooto/piecetype.hpp>
#include <blooto/bitboard.hpp>
#include <blooto/square.hpp>

namespace blooto {

    //! Class containing king-specific operations.
    class KingType: public PieceTypeRegistered<KingType> {
        static BitBoard moves_for_square[64];

    public:
        //! String piece code
        constexpr static const char *codestring = "K";

        //! All possible moves from given square for given colour and occupancy.
        //! @param colour move colour
        //! @param square square to originate moves from
        //! @param occupancy BitBoard containing occipoed squares
        //! @return BitBoard containing squares this piece can move to
        BitBoard moves(MoveColour colour,
                       Square square,
                       BitBoard occupancy) const override
        {
            return moves_for_square[blooto::code(square)];
        }

        //! Check whether this piece type can be a candidate to promote to
        //! @return true if other pieces can be promoted to this one
        bool can_be_promotion() const override {return false;}

    };

}

#endif
