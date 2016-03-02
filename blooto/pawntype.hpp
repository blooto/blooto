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

#ifndef _BLOOTO_PAWNTYPE_HPP
#define _BLOOTO_PAWNTYPE_HPP

#include <cstdint>

#include <blooto/piecetype.hpp>
#include <blooto/bitboard.hpp>
#include <blooto/square.hpp>

namespace blooto {

    //! Class containing pawn-specific operations.
    class PawnType: public PieceTypeRegistered<PawnType> {

        class moves_visitor: public boost::static_visitor<BitBoard> {
            std::uint8_t file_;
            std::uint8_t rank_;
            BitBoard occupancy_;

        public:

            constexpr moves_visitor(Square square, BitBoard occupancy)
            : file_{file(square)}, rank_{rank(square)}, occupancy_{occupancy} {}

            BitBoard operator()(ColourWhite) const {
                BitBoard result;
                if (rank_ < 7) {
                    if (file_ > 0) {
                        Square sq = square(file_ - 1, rank_ + 1);
                        if (occupancy_[sq])
                            result |= sq;
                    }
                    {
                        Square sq = square(file_, rank_ + 1);
                        if (!occupancy_[sq])
                            result |= sq;
                    }
                    if (file_ < 7) {
                        Square sq = square(file_ + 1, rank_ + 1);
                        if (occupancy_[sq])
                            result |= sq;
                    }
                }
                if (rank_ == 1) {
                    Square sq1 = square(file_, rank_ + 1);
                    Square sq2 = square(file_, rank_ + 2);
                    if (!occupancy_[sq1] && !occupancy_[sq2])
                        result |= sq2;
                }
                return result;
            }

            BitBoard operator()(ColourBlack) const {
                BitBoard result;
                if (rank_ > 0) {
                    if (file_ > 0) {
                        Square sq = square(file_ - 1, rank_ - 1);
                        if (occupancy_[sq])
                            result |= sq;
                    }
                    {
                        Square sq = square(file_, rank_ - 1);
                        if (!occupancy_[sq])
                            result |= sq;
                    }
                    if (file_ < 7) {
                        Square sq = square(file_ + 1, rank_ - 1);
                        if (occupancy_[sq])
                            result |= sq;
                    }
                }
                if (rank_ == 6) {
                    Square sq1 = square(file_, rank_ - 1);
                    Square sq2 = square(file_, rank_ - 2);
                    if (!occupancy_[sq1] && !occupancy_[sq2])
                        result |= sq2;
                }
                return result;
            }

        };

    public:
        //! String piece code
        constexpr static const char *codestring = "P";

        //! All possible moves from given square for given colour and occupancy.
        //! @param colour move colour
        //! @param square square to originate moves from
        //! @param occupancy BitBoard containing occipoed squares
        //! @return BitBoard containing squares this piece can move to
        BitBoard moves(MoveColour colour,
                       Square square,
                       BitBoard occupancy) const override
        {
            return boost::apply_visitor(moves_visitor(square, occupancy),
                                        colour);
        }

    };

}

#endif
