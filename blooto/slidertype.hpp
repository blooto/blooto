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

#ifndef _BLOOTO_SLIDERTYPE_HPP
#define _BLOOTO_SLIDERTYPE_HPP

#include <blooto/piecetype.hpp>
#include <blooto/bitboard.hpp>

namespace blooto {
    //! Base class for singletons containing slider piece specific operations.
    template <typename PieceMagic, typename Type>
    class SliderType: public PieceTypeRegistered<Type> {
    public:
        //! All possible moves from given square for given colour and occupancy.
        //! @param colour move colour
        //! @param square square to originate moves from
        //! @param occupancy BitBoard containing occipoed squares
        //! @return BitBoard containing squares this piece can move to
        BitBoard moves(MoveColour colour,
                       Square square,
                       BitBoard occupancy) const override
        {
            return PieceMagic::moves(square, occupancy);
        }
    };
}

#endif
