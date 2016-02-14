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

#ifndef _BLOOTO_BITSCAN_HPP
#define _BLOOTO_BITSCAN_HPP

#include <cstdint>
#include <boost/config.hpp>

namespace blooto {

    //! Implementation of forward bit scan using de Bruijn Sequences.
    class BitScanGeneric {

        constexpr static const unsigned index64[64] = {
            0, 47,  1, 56, 48, 27,  2, 60,
            57, 49, 41, 37, 28, 16,  3, 61,
            54, 58, 35, 52, 50, 42, 21, 44,
            38, 32, 29, 23, 17, 11,  4, 62,
            46, 55, 26, 59, 40, 36, 15, 53,
            34, 51, 20, 43, 31, 22, 10, 45,
            25, 39, 14, 33, 19, 30,  9, 24,
            13, 18,  8, 12,  7,  6,  5, 63
        };

        constexpr static const std::uint64_t debruijn64 = 0x03f79d71b4cb0a89ULL;

    public:
        //! Find least significant one bit in 64-bit number
        //! @author Kim Walisch (2012)
        //! @param data 64-bit number to scan
        //! @return index (0..63) of least significant one bit
        //! This algorithm uses de Bruijn Sequences to find LS1B.
        constexpr static unsigned ls1b(std::uint64_t data) {
            return index64[((data ^ (data-1)) * debruijn64) >> 58];
        }
    };

#if defined(BOOST_MSVC) && defined(_M_X64)
#pragma intrinsic(_BitScanForward64)

    // TODO: Untested! Somebody please test with MSVC
    // and remove this comment if test is successful.

    //! MSVC-specific implementation of forward bit scan.
    class BitScanMSVC {
    public:
        //! Find least significant one bit in 64-bit number
        //! @param data 64-bit number to scan
        //! @return index (0..63) of least significant one bit
        //! This algorithm uses _BitScanForward64 intrinsic to find LS1B.
        constexpr static unsigned ls1b(std::uint64_t data) {
            unsigned long result;
            _BitScanForward64(&result, mask);
            return result;
        }
    };

    struct BitScanOptimised: BitScanMSVC {};

#elif defined(BOOST_GCC) || defined(__clang__) || \
    (defined(BOOST_INTEL) && defined(__GNUC__))

    //! GCC (and clang) specific Implementation of forward bit scan.
    class BitScanGCC {
    public:
        //! Find least significant one bit in 64-bit number
        //! @param data 64-bit number to scan
        //! @return index (0..63) of least significant one bit
        //! This algorithm uses __builtin_ctzll intrinsic to find LS1B.
        constexpr static unsigned ls1b(std::uint64_t data) {
            return __builtin_ctzll(data);
        }
    };

    struct BitScanOptimised: BitScanGCC {};

#else

    struct BitScanOptimised: BitScanGeneric {};

#endif

    //! Most optimal implementation of forward bit scan.
    struct BitScan: BitScanOptimised {};

}

#endif
