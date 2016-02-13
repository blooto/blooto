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

#ifndef _BLOOTO_BITBOARD_HPP
#define _BLOOTO_BITBOARD_HPP

#include <cstdint>
#include <iterator>

#include "square.hpp"

namespace blooto {

    //! BitBoard, representing set of chessboard squares.

    //! This class represents a set of chessboard squares.
    //! The internal representation is 64-bit unsigned integer,
    //! with each bit corresponding to a chessboard square.
    //! It's useful as a piece centric view of chessboard.
    //! See https://chessprogramming.wikispaces.com/Bitboards
    //! for more details.
    class BitBoard {

    public:
        //! Raw 64-bin data internal representation.
        typedef std::uint64_t data_type;

        //! Construct empty BitBoard.
        constexpr BitBoard(): data_(0ULL) {}

        //! Construct BitBoard from raw 64-bit data.
        //! @param data raw 64-bit data
        explicit constexpr BitBoard(data_type data): data_(data) {}

        //! Construct BitBoard containing a single square.
        //! @param sq a single square
        explicit constexpr BitBoard(Square sq)
        : data_(data_type(1) << static_cast<unsigned>(sq)) {}
        
        //! Construct BitBoard from another BitBoard.
        //! @param other another BitBoard
        BitBoard(const BitBoard &other) = default;

        //! Construct BitBoard from another BitBoard.
        //! @param other another BitBoard
        BitBoard(BitBoard &&other) = default;

        //! Raw 64-bit data from BitBoard.
        //! @return raw 64-bit data
        constexpr data_type data() const {return data_;}

        //! Check whether BitBoard contains a particular square.
        //! @param sq square to check for
        //! @return true if BitBoard contains square, false otherwise
        constexpr bool operator[](Square sq) const {
            return data_ & (data_type(1) << static_cast<unsigned>(sq));
        };

        //! Unite BitBoard with another one
        //! @param rhs another BitBoard
        //! @return reference to self
        BitBoard &operator|=(BitBoard rhs) {
            data_ |= rhs.data_;
            return *this;
        }

        //! Intersect BitBoard with another one
        //! @param rhs another BitBoard
        //! @return reference to self
        BitBoard &operator&=(BitBoard rhs) {
            data_ &= rhs.data_;
            return *this;
        }

        //! Iterator to traverse all squares in BitBoard
        class iterator: std::iterator<std::forward_iterator_tag, Square>
        {
            data_type data_;

            constexpr static const Square index64[64] = {
                Square::A1, Square::H6, Square::B1, Square::A8,
                Square::A7, Square::D4, Square::C1, Square::E8,
                Square::B8, Square::B7, Square::B6, Square::F5,
                Square::E4, Square::A3, Square::D1, Square::F8,
                Square::G7, Square::C8, Square::D5, Square::E7,
                Square::C7, Square::C6, Square::F3, Square::E6,
                Square::G5, Square::A5, Square::F4, Square::H3,
                Square::B3, Square::D2, Square::E1, Square::G8,
                Square::G6, Square::H7, Square::C4, Square::D8,
                Square::A6, Square::E5, Square::H2, Square::F7,
                Square::C5, Square::D7, Square::E3, Square::D6,
                Square::H4, Square::G3, Square::C2, Square::F6,
                Square::B4, Square::H5, Square::G2, Square::B5,
                Square::D3, Square::G4, Square::B2, Square::A4,
                Square::F2, Square::C3, Square::A2, Square::E2,
                Square::H1, Square::G1, Square::F1, Square::H8
            };

            constexpr static const data_type debruijn64 = 0x03f79d71b4cb0a89ULL;

        public:

            //! Construct empty iterator with no more squares left.
            constexpr iterator(): data_(0ULL) {}

            //! Construct iterator from BitBoard
            constexpr explicit iterator(BitBoard bb): data_(bb.data()) {}

            //! First square among remaining ones
            //! @author Kim Walisch (2012)
            //! @return index (0..63) of least significant one bit
            constexpr Square operator*() const {
                return index64[((data_ ^ (data_-1)) * debruijn64) >> 58];
            }

            //! Move iterator forward (remove first square)
            //! @return reference to self
            iterator &operator++() {
                data_ &= data_ ^ (0 - data_);
                return *this;
            }

            //! Move iterator forward (remove first square)
            //! @return reference to self
            //! This is post-increment operator,
            //! moving iterator forward, but returning
            //! old iterator's value.
            iterator operator++(int) {
                iterator tmp = *this;
                data_ &= data_ ^ (0 - data_);
                return tmp;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are equal
            constexpr bool operator==(iterator rhs) const {
                return data_ == rhs.data_;
            }

            //! Compare iterator with another one
            //! @param rhs another iterator
            //! @return true is iterators are not equal
            constexpr bool operator!=(iterator rhs) const {
                return data_ != rhs.data_;
            }

        };

        //! Make iterator pointing to the beginning of squares
        //! @return new iterator
        constexpr iterator begin() const {
            return iterator(*this);
        }

        //! Make iterator pointing after the end of squares
        //! @return new iterator
        constexpr iterator end() const {
            return iterator();
        }

    private:
        data_type data_;

    };

    //! Make BitBoard united with another one
    //! @param lhs one BitBoard
    //! @param rhs another BitBoard
    //! @return BitBoard containing all squares that any bitboard contains
    constexpr BitBoard operator|(BitBoard lhs, BitBoard rhs) {
        return BitBoard(lhs.data() | rhs.data());
    }

    //! Make BitBoard intersected with another one
    //! @param lhs one BitBoard
    //! @param rhs another BitBoard
    //! @return BitBoard containing all squares that both bitboards contain
    constexpr BitBoard operator&(BitBoard lhs, BitBoard rhs) {
        return BitBoard(lhs.data() & rhs.data());
    }

    //! Make inverted BitBoard
    //! @param bb original BitBoard
    //! @return BitBoard with all bits inverted
    //!
    //! This means that new BitBoard will contain only those squares
    //! that original one does not contain.
    constexpr BitBoard operator~(BitBoard bb) {
        return BitBoard(~bb.data());
    }

    //! Make BitBoard containing two squares
    //! @param lhs one square
    //! @param rhs another square
    //! @return BitBoard containing both squares
    constexpr BitBoard operator|(Square lhs, Square rhs) {
        return BitBoard(BitBoard(lhs) | BitBoard(rhs));
    }

    //! Make BitBoard containing original BitBoard and one more square
    //! @param lhs original BitBoard
    //! @param rhs another square
    //! @return resulting BitBoard
    constexpr BitBoard operator|(BitBoard lhs, Square rhs) {
        return lhs | BitBoard(rhs);
    }

    //! Make BitBoard containing original BitBoard and one more square
    //! @param lhs another square
    //! @param rhs original BitBoard
    //! @return resulting BitBoard
    constexpr BitBoard operator|(Square lhs, BitBoard rhs) {
        return BitBoard(lhs) | rhs;
    }

    //! Make BitBoard containing all squares but one
    //! @param sq square to not include in resulting BitBoard
    //! @return resulting BitBoard
    constexpr BitBoard operator~(Square sq) {
        return ~BitBoard(sq);
    }

    constexpr const Square BitBoard::iterator::index64[];
}

#endif
