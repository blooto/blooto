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

#include <blooto/square.hpp>
#include <blooto/bitscan.hpp>

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
        using value_type = Square;

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

        //! Assignment operator
        //! @param other another BitBoard
        //! @return reference to BitBoard assigned to
        BitBoard &operator=(const BitBoard &other) = default;

        //! Assignment operator
        //! @param other another BitBoard
        //! @return reference to BitBoard assigned to
        BitBoard &operator=(BitBoard &&other) = default;

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

        //! Add Square to BitBoard
        //! @param rhs another BitBoard
        //! @return reference to self
        BitBoard &operator|=(Square rhs) {
            data_ |= BitBoard(rhs).data_;
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
        class iterator {
            data_type data_;

        public:

            //! Iterator tag type
            using iterator_category = std::forward_iterator_tag;

            //! The type pointed to by this iterator
            using value_type = Square;

            //! Difference between iterators
            using difference_type = std::ptrdiff_t;

            //! Pointer to value
            using pointer = value_type *;

            //! Reference to value
            using reference = value_type &;

            //! Construct empty iterator with no more squares left.
            constexpr iterator(): data_(0ULL) {}

            //! Construct iterator from BitBoard
            constexpr explicit iterator(BitBoard bb): data_(bb.data()) {}

            //! First square among remaining ones
            //! @return first remaining square
            constexpr Square operator*() const {
                return Square(BitScan::ls1b(data_));
            }

            //! Move iterator forward (remove first square)
            //! @return reference to self
            iterator &operator++() {
                data_ &= data_ - 1;
                return *this;
            }

            //! Move iterator forward (remove first square)
            //! @return reference to self
            //! This is post-increment operator,
            //! moving iterator forward, but returning
            //! old iterator's value.
            iterator operator++(int) {
                iterator tmp = *this;
                data_ &= data_ - 1;
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

        //! Check BitBoard for emptiness
        //! @return true if bitboard contains no squares
        constexpr bool empty() const {return data_ == 0ULL;}

    private:
        data_type data_;

    };

    //! Comparison for equality operator for bitboards
    //! @param lhs one BitBoard
    //! @param rhs another BitBoard
    //! @return true is both bitboard contain exactly the same squares
    constexpr bool operator==(BitBoard lhs, BitBoard rhs) {
        return lhs.data() == rhs.data();
    }

    //! Comparison for inequality operator for bitboards
    //! @param lhs one BitBoard
    //! @param rhs another BitBoard
    //! @return false is both bitboard contain exactly the same squares
    constexpr bool operator!=(BitBoard lhs, BitBoard rhs) {
        return lhs.data() != rhs.data();
    }

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

}

#endif
