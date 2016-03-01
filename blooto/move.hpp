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

#ifndef _BLOOTO_MOVE_HPP
#define _BLOOTO_MOVE_HPP

#include <blooto/square.hpp>
#include <blooto/piecetype.hpp>

namespace blooto {

    //! Move of one piece
    class Move {
        const PieceType &piecetype_;
        Square from_;
        Square to_;
        bool attack_;

    public:

        //! Construct move
        //! @param from square where the move originates from
        //! @param to destination of the move
        //! @param piecetype type of piece that is moving
        //! @param attack true if this move is attack
        constexpr Move(const PieceType &piecetype,
                       Square from,
                       Square to,
                       bool attack = false) noexcept
        : piecetype_(piecetype), from_(from), to_(to), attack_(attack) {}

        //! Default copy constructor
        //! @param other other object used to construct this one
        constexpr Move(const Move &other) noexcept = default;

        //! Default move constructor
        //! @param other other object used to construct this one
        constexpr Move(Move &&other) noexcept = default;

        //! Default copy assignment operator
        //! @param other move to assign
        //! @return reference to this oblect
        Move &operator=(const Move &other) = default;

        //! Default move assignment operator
        //! @param other move to assign
        //! @return reference to this oblect
        Move &operator=(Move &&other) = default;

        //! Type of the piece
        //! @return piece type
        constexpr const PieceType &piecetype() const {return piecetype_;}

        //! Square where the move originates from
        //! @return source square
        constexpr Square from() const {return from_;}

        //! Destination of the move
        //! @return destination square
        constexpr Square to() const {return to_;}

        //! Whether the move is attack
        //! @return true is the move is attack
        constexpr bool attack() const {return attack_;}

        //! Compare with other move for equality
        //! @param rhs other move
        //! @return true if both moves are equal
        bool operator==(const Move &rhs) const {
            return
                &piecetype_ == &rhs.piecetype_ &&
                from_ == rhs.from_ &&
                to_ == rhs.to_ &&
                attack_ == rhs.attack_;
        }

        //! Compare with other move for inequality
        //! @param rhs other move
        //! @return true if both moves are not equal
        bool operator!=(const Move &rhs) const {
            return
                &piecetype_ != &rhs.piecetype_ ||
                from_ != rhs.from_ ||
                to_ != rhs.to_ ||
                attack_ != rhs.attack_;
        }

        //! Output move to output stream
        //! @param out output stream
        //! @param move move to output
        //! @return reference to output stream for operator chaining
        template<typename CharT, typename CharTraits>
        friend std::basic_ostream<CharT, CharTraits> &
        operator<<(std::basic_ostream<CharT, CharTraits> &out, const Move &move)
        {
            return out << move.piecetype().code() << move.from()
                       << (move.attack() ? '*' : '-') << move.to();
        }

    };

}

#endif
