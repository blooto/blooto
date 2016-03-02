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
#include <blooto/pawntype.hpp>
#include <blooto/bishoptype.hpp>
#include <blooto/rooktype.hpp>
#include <blooto/knighttype.hpp>
#include <blooto/queentype.hpp>
#include <blooto/kingtype.hpp>
#include <blooto/piece.hpp>
#include <blooto/colour.hpp>
#include <blooto/move.hpp>

namespace blooto {

    //! Chess board containing pieces at its squares
    class Board {
        constexpr static const PieceType *piecetypes[] = {
            nullptr, // code 0 means no piece
            &PawnType::instance,
            &BishopType::instance,
            &RookType::instance,
            &KnightType::instance,
            &QueenType::instance,
            &KingType::instance,
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

        class Proxy {
            const Board &board_;
        public:
            constexpr Proxy(const Board &board): board_{board} {}
            constexpr const Board &board() const {return board_;}
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

        //! Type of pice at the square
        //! @param square square
        //! @return pointer to type of the piece at that square (or nullptr)
        const PieceType *piecetype(Square square) const {
            return piecetypes[pieces_[code(square)]];
        }

        //! Piece at the square
        //! @param square square
        //! @return piece at that square
        //! Behaviour is undefined is there is no piece at that square!
        Piece operator[](Square square) const {
            return Piece(square, *piecetype(square),
                         friendlies_[square] ? colour_.to_piece_colour() :
                         can_move_[square] ? PieceColour(ColourNeutral()) :
                         colour_.opposite().to_piece_colour());
        }

        //! Move a piece on this board
        //! @param from source square where the piece to be moved is located
        //! @param to destination square where the piece to be moved to
        void make_move(Square from, Square to) {
            pieces_[code(to)] = pieces_[code(from)];
            pieces_[code(from)] = 0;
            occupied_ |= to;
            occupied_ &= ~from;
            can_move_ |= to;
            can_move_ &= ~from;
            if (friendlies_[from]) {
                friendlies_ |= to;
                friendlies_ &= ~from;
            }
        }

        //! Generate Move at this board from source and destination squares
        //! @param from source square where the piece to be moved is located
        //! @param to destination square where the piece to be moved to
        //! @return Move created
        Move move(Square from, Square to) const {
            return Move{*piecetype(from), from, to, occupied()[to]};
        }

        //! Move a piece on this board
        //! @param move move to apply
        //! @return pointer to type of piece being attacked or nullptr
        const PieceType *make_move(const Move &move) {
            const PieceType *attacked = piecetype(move.to());
            make_move(move.from(), move.to());
            return attacked;
        }

        //! Flip board colour
        void flip_colour() {
            colour_ = colour_.opposite();
            const BitBoard old_friendlies = friendlies_;
            const BitBoard old_can_move = can_move_;
            friendlies_ = occupied_ & ~old_can_move;
            can_move_ = occupied_ & ~old_friendlies;
        }

        //! Iterator to traverse pieces on the board
        class iterator {
            const Board &board_;
            BitBoard::iterator iter_;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Piece;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

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

            //! Return reference to the board this iterator iterates over
            //! @return reference to the iterator's board
            constexpr const Board &board() const {return board_;}

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

        //! BitBoard containing occupied squares
        //! @return bitboard of all occupied squares
        BitBoard occupied() const {return occupied_;}

        //! BitBoard containing squares occupied only by friendly pieces
        //! @return bitboard of squares occupied by friendly pieces
        BitBoard friendlies() const {return friendlies_;}

        //! BitBoard containing pieces that can move.
        //! @return bitboard of pieces of this board that can move
        //! Can be used like this:
        //! @code
        //! Board b;
        //! ...
        //! for (auto square: b.can_move()) {
        //!     ...
        //! }
        //! @endcode
        BitBoard can_move() const {return can_move_;}

        //! Iterator over all possible (semi-legal) moves on the board
        class moves_iterator {
            const Board &board_;
            BitBoard::iterator piece_iter_;
            BitBoard::iterator move_iter_;

            BitBoard moves_from_source() const {
                Square sq = *piece_iter_;
                const PieceType &piecetype = *board_.piecetype(sq);
                return
                    piecetype.moves(board_.colour_, sq, board_.occupied_) &
                    ~board_.friendlies_;
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Move;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

            //! Tag for constructing move_iterator pointing to begin of moves
            struct begin {};

            //! Tag for constructing move_iterator pointing after end of moves
            struct end {};

            //! Construct move_iterator pointing to the first move
            //! @param board board ths iterator iterates over
            moves_iterator(const Board &board, begin)
            : board_{board}, piece_iter_{board.can_move().begin()}
            {
                while (piece_iter_ != board.can_move().end()) {
                    BitBoard moves = moves_from_source();
                    if (!moves.empty()) {
                        move_iter_ = moves.begin();
                        break;
                    }
                    ++piece_iter_;
                }
            }

            //! Construct move_iterator pointing after the last move
            //! @param board board ths iterator iterates over
            moves_iterator(const Board &board, end)
            : board_{board}, piece_iter_{board.can_move().end()} {}

            //! Move this iterator points to
            //! @return move
            Move operator*() const {
                return board_.move(*piece_iter_, *move_iter_);
            }

            //! Move iterator forward
            //! @return reference to self
            moves_iterator &operator++() {
                ++move_iter_;
                while (move_iter_ == BitBoard::iterator()) {
                    ++piece_iter_;
                    if (piece_iter_ == board_.can_move().end())
                        break;
                    move_iter_ = moves_from_source().begin();
                }
                return *this;
            }

            //! Move iterator forward
            //! @return old value of self
            //! This is post-increment operator,
            //! moving iterator forward, but returning
            //! old iterator's value.
            moves_iterator operator++(int) {
                moves_iterator tmp{*this};
                ++*this;
                return tmp;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are equal
            bool operator==(moves_iterator rhs) const {
                return
                    &board_ == &rhs.board_ &&
                    piece_iter_ == rhs.piece_iter_ &&
                    move_iter_ == rhs.move_iter_;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are not equal
            bool operator!=(moves_iterator rhs) const {
                return
                    &board_ != &rhs.board_ ||
                    piece_iter_ != rhs.piece_iter_ ||
                    move_iter_ != rhs.move_iter_;
            }

        };

        //! Make iterator pointing to the first possible pseudo-legal move
        //! @return new iterator
        moves_iterator moves_begin() const {
            return moves_iterator(*this, moves_iterator::begin());
        }

        //! Make iterator pointing after the last possible pseudo-legal move
        //! @return new iterator
        moves_iterator moves_end() const {
            return moves_iterator(*this, moves_iterator::end());
        }

        //! Check that there are no mossible pseudo-legal moves
        //! @return true if board has no possible pseudo-legal moves
        bool moves_empty() const {return moves_begin() == moves_end();}

        //! Proxy class representing sequence of possible pseudo-legal moves.
        //! The main purpose of this class is to be returned by
        //! Board::moves() method.
        //! Can be used like this:
        //! @code
        //! Board b;
        //! ...
        //! for (auto move: b.moves()) {
        //!     ...
        //! }
        //! @endcode
        struct Moves: Proxy {

            //! Iterator type
            using iterator = Board::moves_iterator;

            //! Construct proxy object from board
            using Proxy::Proxy;

            //! Make iterator pointing to the first possible pseudo-legal move
            //! @return new iterator
            iterator begin() const {return board().moves_begin();}

            //! Make iterator pointing after the last possible pseudo-legal move
            //! @return new iterator
            iterator end() const {return board().moves_end();}

            //! Check that there are no possible pseudo-legal moves
            //! @return true if board has no possible pseudo-legal moves
            bool empty() const {return board().moves_empty();}

        };

        //! Create proxy object representing sequence of pseudo-legal moves.
        //! Can be used like this:
        //! @code
        //! Board b;
        //! ...
        //! for (auto move: b.moves()) {
        //!     ...
        //! }
        //! @endcode
        Moves moves() const {return Moves{*this};}

    };

}

#endif
