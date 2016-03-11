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
#include <iostream>

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
            map_type::mapped_type king_code_;
            template <size_t N>
            PieceTypeCodes(const PieceType *const (&piecetypes)[N])
            {
                for (std::uint_fast8_t i = 1; i < N; ++i) {
                    const PieceType *pt = piecetypes[i];
                    map_.insert(map_type::value_type(pt, i));
                    if (pt == &KingType::instance)
                        king_code_ = i;
                }
            }
        public:
            static const PieceTypeCodes &instance() {
                static PieceTypeCodes inst{piecetypes};
                return inst;
            }
            static std::uint8_t get(const PieceType &pt) {
                return instance().map_.find(&pt)->second;
            }
            static std::uint8_t king_code() {
                return instance().king_code_;
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
        Board(): colour_{ColourWhite()} {pieces_.fill(0);}

        //! Construct empty board with specified move colour
        //! @param colour colour for the next move
        Board(MoveColour colour): colour_{colour} {pieces_.fill(0);}

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
            pieces_.fill(0);
            for (const auto &p: pieces)
                insert(p);
        }

        //! Construct board from move colour and list of pieces
        //! @param colour colour for the next move
        //! @param pieces list of pieces
        Board(MoveColour colour, const std::initializer_list<Piece> &pieces)
        : colour_{colour}
        {
            pieces_.fill(0);
            for (const auto &p: pieces)
                insert(p);
        }

        //! Move colour access method
        //! @return current move colour
        MoveColour colour() const {return colour_;}

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

        //! Board's representation of piece at some square
        class PieceCode {
            std::uint8_t code_;
            bool friendly_;
            bool can_move_;

        public:
            //! Construct piececode from code and colour
            //! @param code board-internal piece code
            //! @param friendly true if this piece is friendly
            //! @param can_move true if this piece can move
            PieceCode(std::uint8_t code, bool friendly, bool can_move)
            : code_{code}, friendly_{friendly}, can_move_{can_move} {}

            //! Board-internal piece code
            //! @return board-internal piece code
            std::uint8_t code() const {return code_;}

            //! Check whether this piece is friendly
            //! @return true if this piece is friendly
            bool friendly() const {return friendly_;}

            //! Check whether this piece can move
            //! @return true if this piece can move
            bool can_move() const {return can_move_;}

            //! Piece type
            //! @return piecetype for this piececode (or nullptr if no piece)
            const PieceType *piecetype() const {return piecetypes[code_];}
        };

        //! Piece code at given square
        //! @param square square to look for piece
        //! @return piececode for that square
        PieceCode at(Square square) const {
            return {
                pieces_[code(square)],
                friendlies_[square],
                can_move_[square]
            };
        }

        //! Remove piece from given square and return its piececode
        //! @param square square to take piece from
        //! @return piececode for piece taken
        PieceCode take(Square square) {
            PieceCode result{at(square)};
            pieces_[code(square)] = 0;
            occupied_ &= ~square;
            can_move_ &= ~square;
            friendlies_ &= ~square;
            return result;
        }

        //! Put piece to given square
        //! @param square square to put piece to
        //! @param pc piececode for piece to put
        void put(Square square, PieceCode pc) {
            pieces_[code(square)] = pc.code();
            occupied_.set(square, pc.code());
            friendlies_.set(square, pc.friendly());
            can_move_.set(square, pc.can_move());
        }

        //! Check whether unfriendly king is at the square
        //! @param square square to check
        //! @return true if unfriendly king is at the square
        bool is_unfriendly_king(Square square) const {
            return
                !can_move()[square] &&
                pieces_[code(square)] == PieceTypeCodes::king_code();
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

        //! Generate Move with promotion
        //! @param from source square where the piece to be moved is located
        //! @param to destination square where the piece to be moved to
        //! @param promotion piece type to promote piece to
        //! @return Move created
        Move move(Square from, Square to, const PieceType &promotion) const {
            return Move{*piecetype(from), from, to, occupied()[to], &promotion};
        }

        //! Bitboard of moves that can make a piece at given square
        //! @param square square a piece located at
        //! @return bitboard of squares this piece can move to
        BitBoard moves_from(Square square) const {
            const PieceType &pt = *piecetype(square);
            return pt.moves(colour_, square, occupied_) & ~friendlies_;
        }

        //! Move a piece on this board
        //! @param move move to apply
        //! @return pointer to type of piece being attacked or nullptr
        const PieceType *make_move(const Move &move) {
            const PieceType *attacked = piecetype(move.to());
            make_move(move.from(), move.to());
            if (move.promotion())
                make_promotion(move.to(), *move.promotion());
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

            //! Iterator tag type
            using iterator_category = std::forward_iterator_tag;

            //! The type pointed to by this iterator
            using value_type = Piece;

            //! Difference between iterators
            using difference_type = std::ptrdiff_t;

            //! Pointer to value
            using pointer = value_type *;

            //! Reference to value
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

        //! Iterator over all possible promotions
        class promotions_iterator {
            const PieceType *const *iter_;

        public:
            //! Iterator tag type
            using iterator_category = std::forward_iterator_tag;

            //! The type pointed to by this iterator
            using value_type = const PieceType &;

            //! Difference between iterators
            using difference_type = std::ptrdiff_t;

            //! Pointer to value
            using pointer = const PieceType *;

            //! Reference to value
            using reference = value_type;

            //! Tag for constructing promotions_iterator pointing to begin
            struct begin {};

            //! Tag for constructing promotions_iterator pointing after end
            struct end {};

            //! Construct null promotions_iterator
            constexpr promotions_iterator() noexcept: iter_{nullptr} {}

            //! Construct promotions_iterator pointing to the first promotion
            promotions_iterator(begin): iter_{std::begin(piecetypes)} {
                ++*this;
            }

            //! Construct promotions_iterator pointing after the last promotion
            promotions_iterator(end): iter_{std::end(piecetypes)} {}

            //! Promotion this iterator points to
            //! @return promotion piecetype
            constexpr const PieceType &operator*() const {
                return **iter_;
            }

            //! Move iterator forward
            //! @return reference to self
            promotions_iterator &operator++() {
                ++iter_;
                while (iter_ != std::end(piecetypes) &&
                    !(*iter_)->can_be_promotion())
                    ++iter_;
                return *this;
            }

            //! Move iterator forward
            //! @return old value of self
            //! This is post-increment operator,
            //! moving iterator forward, but returning
            //! old iterator's value.
            promotions_iterator operator++(int) {
                promotions_iterator tmp{*this};
                ++*this;
                return tmp;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are equal
            constexpr bool operator==(promotions_iterator rhs) const {
                return iter_ == rhs.iter_;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are not equal
            constexpr bool operator!=(promotions_iterator rhs) const {
                return iter_ != rhs.iter_;
            }

            //! Piece type code
            //! @return code of piece type
            constexpr std::uint8_t code() const {return iter_ - piecetypes;}

        };

        //! Make iterator pointing to the first possible promotion
        //! @return new iterator
        static promotions_iterator promotions_begin() {
            return promotions_iterator(promotions_iterator::begin());
        }

        //! Make iterator pointing after the last possible promotion
        //! @return new iterator
        static promotions_iterator promotions_end() {
            return promotions_iterator(promotions_iterator::end());
        }

        //! Proxy class representing sequence of possible piece promotions.
        //! The main purpose of this class is to be returned by
        //! Board::promotions() method.
        //! Can be used like this:
        //! @code
        //! for (auto piecetype: Board::promotions()) {
        //!     ...
        //! }
        //! @endcode
        struct Promotions {

            //! Iterator type
            using iterator = Board::promotions_iterator;

            //! Make iterator pointing to the first possible promotion
            //! @return new iterator
            iterator begin() const {return promotions_begin();}

            //! Make iterator pointing after the last possible promotion
            //! @return new iterator
            iterator end() const {return promotions_end();}

            //! Check that there are no possible promotions
            //! @return always false
            bool empty() const {return false;}

        };

        //! Create proxy object representing sequence of possible promotions
        //! Can be used like this:
        //! @code
        //! for (auto piecetype: Board::promotions()) {
        //!     ...
        //! }
        //! @endcode
        static Promotions promotions() {return Promotions{};}

        //! Move a promotion on this board
        //! @param square square where the piece to be promoted is located
        //! @param p iterator pointing to promotion to be performed
        void make_promotion(Square square, promotions_iterator p) {
            pieces_[code(square)] = p.code();
        }

        //! Move a promotion on this board
        //! @param square square where the piece to be promoted is located
        //! @param promotion piece type to use as promotion
        void make_promotion(Square square, const PieceType &promotion) {
            pieces_[code(square)] = PieceTypeCodes::get(promotion);
        }

        //! Iterator over all possible (semi-legal) moves on the board
        class moves_iterator {
            const Board &board_;
            BitBoard::iterator from_iter_;
            BitBoard::iterator to_iter_;
            promotions_iterator promo_iter_;

        public:

            //! Iterator tag type
            using iterator_category = std::forward_iterator_tag;

            //! The type pointed to by this iterator
            using value_type = Move;

            //! Difference between iterators
            using difference_type = std::ptrdiff_t;

            //! Pointer to value
            using pointer = value_type *;

            //! Reference to value
            using reference = value_type &;

            //! Tag for constructing move_iterator pointing to begin of moves
            struct begin {};

            //! Tag for constructing move_iterator pointing after end of moves
            struct end {};

            //! Construct move_iterator pointing to the first move
            //! @param board board ths iterator iterates over
            moves_iterator(const Board &board, begin)
            : board_{board}, from_iter_{board.can_move().begin()}
            {
                while (from_iter_ != board.can_move().end()) {
                    BitBoard moves = board.moves_from(*from_iter_);
                    if (!moves.empty()) {
                        to_iter_ = moves.begin();
                        std::uint8_t pcidx = board_.pieces_[code(*from_iter_)];
                        const PieceType &pt = *piecetypes[pcidx];
                        if (pt.can_be_promoted(board_.colour(), *to_iter_))
                            promo_iter_ = promotions_begin();
                        else
                            promo_iter_ = promotions_iterator{};
                        break;
                    }
                    ++from_iter_;
                }
            }

            //! Construct move_iterator pointing after the last move
            //! @param board board ths iterator iterates over
            moves_iterator(const Board &board, end)
            : board_{board}, from_iter_{board.can_move().end()} {}

            //! Move this iterator points to
            //! @return move
            Move operator*() const {
                if (promo_iter_ == promotions_iterator{})
                    return board_.move(*from_iter_, *to_iter_);
                else
                    return board_.move(*from_iter_, *to_iter_, *promo_iter_);
            }

            //! Move iterator forward
            //! @return reference to self
            moves_iterator &operator++() {
                if (promo_iter_ != promotions_iterator{}) {
                    ++promo_iter_;
                    if (promo_iter_ == promotions_end())
                        promo_iter_ = promotions_iterator{};
                    else
                        return *this;
                }
                ++to_iter_;
                while (to_iter_ == BitBoard::iterator()) {
                    ++from_iter_;
                    if (from_iter_ == board_.can_move().end())
                        return *this;
                    to_iter_ = board_.moves_from(*from_iter_).begin();
                }
                std::uint8_t pcidx = board_.pieces_[code(*from_iter_)];
                const PieceType &pt = *piecetypes[pcidx];
                if (pt.can_be_promoted(board_.colour(), *to_iter_))
                    promo_iter_ = promotions_begin();
                else
                    promo_iter_ = promotions_iterator{};
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
                    from_iter_ == rhs.from_iter_ &&
                    to_iter_ == rhs.to_iter_ &&
                    promo_iter_ == rhs.promo_iter_;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are not equal
            bool operator!=(moves_iterator rhs) const {
                return
                    &board_ != &rhs.board_ ||
                    from_iter_ != rhs.from_iter_ ||
                    to_iter_ != rhs.to_iter_ ||
                    promo_iter_ != rhs.promo_iter_;
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

    //! Read board from input stream
    //! @param in input stream
    //! @param board board to read
    //! @return reference to the same input stream for operator chaining
    //! Please note that this input operator just inserts new pieces
    //! to the board, so the board is not cleared before input.
    std::istream &operator>>(std::istream &in, Board &board);

    //! Write board to output stream
    //! @param out output stream
    //! @param board board to write
    //! @return reference to the same input stream for operator chaining
    std::ostream &operator<<(std::ostream &out, const Board &board);
}

#endif
