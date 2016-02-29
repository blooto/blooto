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

#ifndef _BLOOTO_BOARD_HPP
#define _BLOOTO_BOARD_HPP

#include <cstdint>
#include <array>
#include <map>
#include <initializer_list>
#include <iterator>

#include <blooto/square.hpp>
#include <blooto/bitboard.hpp>
#include <blooto/piecetype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/queentype.hpp>
#include <blooto/piece.hpp>
#include <blooto/colour.hpp>

namespace blooto {

    //! Chess board containing pieces at its squares
    class Board {
        constexpr static const PieceType *piecetypes[] = {
            nullptr, // code 0 means no piece
            &BishopType::instance,
            &RookType::instance,
            &QueenType::instance
        };

        class PieceTypeCodes {
            using map_type = std::map<const PieceType *, std::uint8_t>;
            map_type map_;
            template <size_t N>
            PieceTypeCodes(const PieceType *const (&piecetypes)[N])
            {
                for (std::uint_fast8_t i = 1; i < N; ++i)
                    map_.insert(map_type::value_type(piecetypes[i], i));
            }
        public:
            static const PieceTypeCodes &instance() {
                static PieceTypeCodes inst{piecetypes};
                return inst;
            }
            static std::uint8_t get(const PieceType &pt) {
                return instance().map_.find(&pt)->second;
            }
        };

        MoveColour colour_;
        std::array<std::uint8_t, 64> pieces_;
        BitBoard occupied_;
        BitBoard friendlies_;
        BitBoard can_move_;

    public:

        //! Construct empty board
        Board(): colour_{ColourWhite()} {}

        //! Construct empty board with specified move colour
        //! @param colour colour for the next move
        Board(MoveColour colour): colour_{colour} {}

        //! Default copy constructor
        //! @param other board to construct from
        Board(const Board &other) = default;

        //! Default move constructor
        //! @param other board to construct from
        Board(Board &&other) = default;

        //! Construct board from list of pieces
        //! @param pieces list of pieces
        Board(const std::initializer_list<Piece> &pieces)
        : colour_{ColourWhite()}
        {
            for (const auto &p: pieces)
                insert(p);
        }

        //! Add a piece to this board
        //! @param piece piece to add
        void insert(const Piece &piece) {
            auto sq = static_cast<std::uint8_t>(piece.square());
            pieces_[sq] = PieceTypeCodes::get(piece.piecetype());
            occupied_ |= piece.square();
            if (colour_.friendly(piece.colour()))
                friendlies_ |= piece.square();
            if (colour_.can_move(piece.colour()))
                can_move_ |= piece.square();
        }

        //! Piece at the square
        //! @param square square
        //! @return piece at that square
        //! Behaviour is undefined is there is no piece at that square!
        Piece operator[](Square square) const {
            auto sq = static_cast<std::uint8_t>(square);
            return Piece(square, *piecetypes[pieces_[sq]],
                         friendlies_[square] ? colour_.to_piece_colour() :
                         can_move_[square] ? PieceColour(ColourNeutral()) :
                         colour_.opposite().to_piece_colour());
        }

        //! Iterator to traverse pieces on the board
        class iterator: std::iterator<std::forward_iterator_tag, Piece> {
            const Board &board_;
            BitBoard::iterator iter_;

        public:

            //! Construct iterator
            //! @param board board this iterator iterates over
            //! @param iter initial position of this iterator
            constexpr iterator(const Board &board, BitBoard::iterator iter)
            : board_(board), iter_(iter) {}

            //! Piece pointed by this iterator
            //! @return piece at the square iterator points to
            Piece operator*() const {return board_[*iter_];}

            //! Move iterator forward
            //! @return reference to self
            iterator &operator++() {
                ++iter_;
                return *this;
            }

            //! Move iterator forward
            //! @return old value of self
            //! This is post-increment operator,
            //! moving iterator forward, but returning
            //! old iterator's value.
            iterator operator++(int) {
                iterator tmp = *this;
                ++iter_;
                return tmp;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are equal
            constexpr bool operator==(iterator rhs) const {
                return &board_ == &rhs.board_ && iter_ == rhs.iter_;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are not equal
            constexpr bool operator!=(iterator rhs) const {
                return &board_ != &rhs.board_ || iter_ != rhs.iter_;
            }

        };

        //! Make iterator pointing to a piece at the first occupied square
        //! @return new iterator
        iterator begin() const {
            return iterator(*this, occupied_.begin());
        }

        //! Make iterator pointing after the last occupoed square
        //! @return new iterator
        iterator end() const {
            return iterator(*this, occupied_.end());
        }

        //! Check board for emptiness
        //! @return true if board contains no pieces
        bool empty() const {return occupied_.empty();}

    };

}

#endif
