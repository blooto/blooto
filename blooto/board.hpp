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
#include <utility>
#include <iostream>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>

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
    public:
        //! Type sequence containing all possible piece types on this board
        using piecetypes_t =
            boost::mpl::vector<PawnType, BishopType, RookType,
                               KnightType, QueenType, KingType>;

    private:
        using numpieces = boost::mpl::size<piecetypes_t>::type;

        template <typename PT> using piece_code =
            boost::mpl::distance<boost::mpl::begin<piecetypes_t>::type,
                                 typename boost::mpl::find<piecetypes_t,
                                                           PT>::type>;

        template <typename NB, typename N> struct find_nb:
        boost::mpl::if_<boost::mpl::bool_<((1 << NB::value) < N::value)>,
                        find_nb<typename NB::next, N>, NB>::type {};

        template <typename N> using numbits = find_nb<boost::mpl::int_<0>, N>;

        using bb_size = numbits<numpieces>::type;

        // Let's hope that we won't have more than 2^32 pieces on single board.
        using piececode_t =
        boost::mpl::if_<
            boost::mpl::less_equal<bb_size, boost::mpl::int_<8>>,
            std::uint8_t,
            boost::mpl::if_<
                boost::mpl::less_equal<bb_size, boost::mpl::int_<16>>,
                std::uint16_t,
                std::uint32_t
            >::type
        >::type;

        struct bb_storage_base {
            void set(piececode_t, BitBoard) {}
            piececode_t get(Square square) const {return 0;}
            void move(Square, Square) {}
        };

        template <typename Base, typename N>
        class bb_storage_unit: public Base {
            BitBoard value_;
            void set_internal(boost::mpl::true_, BitBoard bb) {value_ |= bb;}
            void set_internal(boost::mpl::false_, BitBoard bb) {value_ &= ~bb;}
            void set_internal(bool v, BitBoard bb) {
                if (v)
                    set_internal(boost::mpl::true_(), bb);
                else
                    set_internal(boost::mpl::false_(), bb);
            }
        public:
            bb_storage_unit() {}
            bb_storage_unit(const bb_storage_unit &other) = default;
            bb_storage_unit(bb_storage_unit &&other) = default;
            template <typename Transform>
            bb_storage_unit(const bb_storage_unit &other,
                            const Transform &transform)
            : Base{other, transform}
            , value_{transform(N(), other.value_)} {}
            void set(piececode_t code, BitBoard bb) {
                set_internal(code & (1 << N::value), bb);
                Base::set(code, bb);
            }
            piececode_t get(Square square) const {
                return
                    (value_[square] ? (1 << N::value) : 0) | Base::get(square);
            }
            void move(Square from, Square to) {
                set_internal(value_[from], BitBoard{to});
                Base::move(from, to);
            }
        };

        using bb_storage =
            boost::mpl::fold<boost::mpl::range_c<int, 0, bb_size::value>::type,
                             bb_storage_base,
                             bb_storage_unit<boost::mpl::_1,
                                             boost::mpl::_2>>::type;

        struct piece_registry_base {
            const PieceType *piecetypes[numpieces::value];
            using map_type = std::map<const PieceType *, piececode_t>;
            map_type map_;
        };

        template <typename Base, typename PT>
        struct piece_registry_unit: Base {
            piece_registry_unit() {
                using value_type = typename Base::map_type::value_type;
                using piece_type = PT;
                static const piece_type &instance = piece_type::instance;
                static const piececode_t code = piece_code<piece_type>::value;
                Base::map_.insert(value_type(&instance, code));
                Base::piecetypes[code] = &instance;
            }
        };

        using piece_registry =
            boost::mpl::fold<piecetypes_t,
                             piece_registry_base,
                             piece_registry_unit<boost::mpl::_1,
                                                 boost::mpl::_2>>::type;

        class PieceTypeCodes: piece_registry {
            PieceTypeCodes() {}
        public:
            using piecetypes_t = const PieceType *const(&)[numpieces::value];
            static const PieceTypeCodes &instance() {
                static PieceTypeCodes inst;
                return inst;
            }
            static piececode_t get(const PieceType &pt) {
                return instance().piece_registry::map_.find(&pt)->second;
            }
            static constexpr piececode_t king_code() {
                return piece_code<KingType>::value;
            }
            static piecetypes_t piecetypes() {
                return instance().piece_registry::piecetypes;
            }
        };

        class Proxy {
            const Board &board_;
        public:
            constexpr Proxy(const Board &board): board_{board} {}
            constexpr const Board &board() const {return board_;}
        };

        MoveColour colour_;
        bb_storage pieces_;
        BitBoard friendlies_or_neutral_;
        BitBoard unfriendlies_or_neutral_;

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

        //! Construct board from move colour and list of pieces
        //! @param colour colour for the next move
        //! @param pieces list of pieces
        Board(MoveColour colour, const std::initializer_list<Piece> &pieces)
        : colour_{colour}
        {
            for (const auto &p: pieces)
                insert(p);
        }

        //! Move colour access method
        //! @return current move colour
        MoveColour colour() const {return colour_;}

        //! Add a piece to this board
        //! @param piece piece to add
        void insert(const Piece &piece) {
            pieces_.set(PieceTypeCodes::get(piece.piecetype()),
                        BitBoard{piece.square()});
            if (colour_.friendly(piece.colour()))
                unfriendlies_or_neutral_ &= ~piece.square();
            else
                unfriendlies_or_neutral_ |= piece.square();
            if (colour_.can_move(piece.colour()))
                friendlies_or_neutral_ |= piece.square();
            else
                friendlies_or_neutral_ &= ~piece.square();
        }

        //! Type of pice at the square
        //! @param square square
        //! @return pointer to type of the piece at that square (or nullptr)
        const PieceType *piecetype(Square square) const {
            if (!occupied()[square])
                return nullptr;
            return PieceTypeCodes::piecetypes()[pieces_.get(square)];
        }

        //! Piece at the square
        //! @param square square
        //! @return piece at that square
        //! Behaviour is undefined is there is no piece at that square!
        Piece operator[](Square square) const {
            return Piece(square, *piecetype(square),
                         friendlies()[square] ? colour_.to_piece_colour() :
                         can_move()[square] ? PieceColour(ColourNeutral()) :
                         colour_.opposite().to_piece_colour());
        }

        //! Check whether unfriendly king is at the square
        //! @param square square to check
        //! @return true if unfriendly king is at the square
        bool is_unfriendly_king(Square square) const {
            return
                unfriendlies()[square] &&
                pieces_.get(square) == PieceTypeCodes::king_code();
        }

        //! Move a piece on this board
        //! @param from source square where the piece to be moved is located
        //! @param to destination square where the piece to be moved to
        void make_move(Square from, Square to) {
            pieces_.move(from, to);
            if (friendlies_or_neutral_[from]) {
                friendlies_or_neutral_ |= to;
                friendlies_or_neutral_ &= ~from;
            } else {
                friendlies_or_neutral_ &= ~to;
            }
            if (unfriendlies_or_neutral_[from]) {
                unfriendlies_or_neutral_ |= to;
                unfriendlies_or_neutral_ &= ~from;
            } else {
                unfriendlies_or_neutral_ &= ~to;
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
            return pt.moves(colour_, square, occupied()) & ~friendlies();
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
            std::swap(friendlies_or_neutral_, unfriendlies_or_neutral_);
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
            return iterator(*this, occupied().begin());
        }

        //! Make iterator pointing after the last occupoed square
        //! @return new iterator
        iterator end() const {
            return iterator(*this, occupied().end());
        }

        //! Check board for emptiness
        //! @return true if board contains no pieces
        bool empty() const {return occupied().empty();}

        //! BitBoard containing occupied squares
        //! @return bitboard of all occupied squares
        BitBoard occupied() const {
            return friendlies_or_neutral_ | unfriendlies_or_neutral_;
        }

        //! BitBoard containing squares occupied only by friendly pieces
        //! @return bitboard of squares occupied by friendly pieces
        BitBoard friendlies() const {
            return friendlies_or_neutral_ & ~unfriendlies_or_neutral_;
        }

        //! BitBoard containing squares occupied only by unfriendly pieces
        //! @return bitboard of squares occupied by unfriendly pieces
        BitBoard unfriendlies() const {
            return unfriendlies_or_neutral_ & ~friendlies_or_neutral_;
        }

        //! BitBoard containing squares occupied only by neutral pieces
        //! @return bitboard of squares occupied by neutral pieces
        BitBoard neutrals() const {
            return friendlies_or_neutral_ & unfriendlies_or_neutral_;
        }

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
        BitBoard can_move() const {return friendlies_or_neutral_;}

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
            promotions_iterator(begin)
            : iter_{std::begin(PieceTypeCodes::piecetypes())} {
                ++*this;
            }

            //! Construct promotions_iterator pointing after the last promotion
            promotions_iterator(end)
            : iter_{std::end(PieceTypeCodes::piecetypes())} {}

            //! Promotion this iterator points to
            //! @return promotion piecetype
            constexpr const PieceType &operator*() const {
                return **iter_;
            }

            //! Move iterator forward
            //! @return reference to self
            promotions_iterator &operator++() {
                ++iter_;
                while (iter_ != std::end(PieceTypeCodes::piecetypes()) &&
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
            piececode_t code() const {
                return iter_ - std::begin(PieceTypeCodes::piecetypes());
            }

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
            pieces_.set(p.code(), BitBoard{square});
        }

        //! Move a promotion on this board
        //! @param square square where the piece to be promoted is located
        //! @param promotion piece type to use as promotion
        void make_promotion(Square square, const PieceType &promotion) {
            pieces_.set(PieceTypeCodes::get(promotion), BitBoard{square});
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
                        piececode_t pcidx = board_.pieces_.get(*from_iter_);
                        const PieceType &pt =
                            *PieceTypeCodes::piecetypes()[pcidx];
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
                piececode_t pcidx = board_.pieces_.get(*from_iter_);
                const PieceType &pt =
                    *PieceTypeCodes::piecetypes()[pcidx];
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
