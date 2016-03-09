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

#ifndef _BLOOTO_SOLUTION_HPP
#define _BLOOTO_SOLUTION_HPP

#include <list>

#include <blooto/move.hpp>

namespace blooto {

    //! Chess composition solution tree
    class Solution {
    public:

        //! List of solutions containing next moves
        using list = std::list<Solution>;

        //! Construct solution containing just one move
        //! @param from square where the move originates from
        //! @param to destination of the move
        //! @param piecetype type of piece that is moving
        //! @param attack true if this move is attack
        Solution(const PieceType &piecetype,
                 Square from,
                 Square to,
                 bool attack = false) noexcept
        : move_{piecetype, from, to, attack} {}

        //! Construct solution containing just one move
        //! @param move move the solution will contain
        Solution(const Move &move) noexcept: move_{move} {}

        //! Construct solution containing a move and list of subsequent moves
        //! @param move move the solution will contain
        //! @param next next moves
        Solution(const Move &move, const list &next)
        : move_{move}, next_{next} {}

        //! Construct solution containing a move and list of subsequent moves
        //! @param move move the solution will contain
        //! @param next next moves
        Solution(const Move &move, list &&next) : move_{move}, next_{next} {}

        //! First move
        const Move &move() const noexcept {return move_;}

        //! List of solutions containing next moves
        const list &next() const noexcept {return next_;}

    private:

        Move move_;
        list next_;

    };

}

#endif
