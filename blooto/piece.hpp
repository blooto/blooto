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

#ifndef _BLOOTO_PIECE_HPP
#define _BLOOTO_PIECE_HPP

#include <string>
#include <stdexcept>
#include <ostream>

#include <blooto/square.hpp>
#include <blooto/piecetype.hpp>
#include <blooto/colour.hpp>
#include <blooto/bitboard.hpp>

namespace blooto {

    //! Piece of specific colour places at some square.
    class Piece {
        Square square_;
        const PieceType &piecetype_;
        PieceColour colour_;

    public:

        //! Exception class to throw when parsing piece description
        class ParseError;

        //! Construct a piece.
        //! @param square square where the piece is placed
        //! @param piecetype type of the piece
        //! @param colour colour of the piece
        Piece(Square square,
              const PieceType &piecetype,
              PieceColour colour)
        : square_{square}, piecetype_{piecetype}, colour_{colour} {}

        //! Square of the piece
        //! @return square
        Square square() const {return square_;}

        //! Type of the puece
        //! @return piece type

        const PieceType &piecetype() const {return piecetype_;};

        //! Colour of the piece
        //! @return colour
        PieceColour colour() const {return colour_;}

        //! Parse piece code and return piece of specific colour
        //! @param code string containing piece code and square
        //! @param colour piece colour
        //! @return resulting piece
        static Piece from_code(const std::string &code, PieceColour colour);

        //! Compare with other piece for equality
        //! @param rhs other piece
        //! @return true if both pieces are equal
        bool operator==(const Piece &rhs) const {
            return
                square_ == rhs.square_ &&
                &piecetype_ == &rhs.piecetype_ &&
                colour_ == rhs.colour_;
        }

        //! Compare with other piece for inequality
        //! @param rhs other piece
        //! @return true if both pieces are not equal
        bool operator!=(const Piece &rhs) const {
            return
                square_ != rhs.square_ ||
                &piecetype_ != &rhs.piecetype_ ||
                colour_ != rhs.colour_;
        }

        //! All possible moves by this piece for given occupancy
        //! @param occupancy BitBoard containing occipoed squares
        //! @return BitBoard containing squares this piece can move to
        BitBoard moves(BitBoard occupancy) const {
            return piecetype_.moves(square_, occupancy);
        }

        //! Output piece to output stream
        //! @param out output stream
        //! @param p piece to output
        //! @return reference to output stream for operator chaining
        template<typename CharT, typename CharTraits>
        friend std::basic_ostream<CharT, CharTraits> &
        operator<<(std::basic_ostream<CharT, CharTraits> &out,
                   const Piece &p)
        {
            return out << p.piecetype().code() << p.square();
        }

    };

    //! Exception class to throw when parsing piece description
    class Piece::ParseError: public std::runtime_error {
    public:
        //! Construct exception oblect
        //! @param msg textual error description
        ParseError(const std::string &msg): std::runtime_error{msg} {}
    };
}

#endif
