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

// This file is modified version of magicmoves.h by Pradyumna Kannan.
// The original copyright notice follows.

/**
 *magicmoves.h
 *
 *Header file for magic move bitboard generation.  Include this in any files
 *need this functionality.
 *
 *Usage:
 *You can use the following static methods for generating move bitboards by
 *giving them a square and an occupancy.  The method will then return
 *the correct move bitboard for that particular square and occupancy.
 *
 *BitBoard blooto::BMagic<Impl>::moves(Square square, BitBoard occupancy);
 *BitBoard blooto::RMagic<Impl>::moves(Square square, BitBoard occupancy);
 *
 *Square is a Square.
 *Occupancy is a BitBoard that describes which squares on
 *the board are occupied.
 *
 *If you want to
 *minimize the size of the bitboards, use BMagicMin and RMagicMin respectively
 *as Impl template parameter (more info on this later).
 *
 *If BMagicPerfectHash and RMagicPerfectHash are used as Impl template
 *parameter,
 *the move generator will use an additional indrection to make the
 *table sizes smaller : (~50kb+((original size)/sizeof(std::uint16_t)).
 *The size listed from here on out are the sizes without std::uint16_t.
 *
 *The following methods are identical to blooto::RMagic<Impl>::moves and
 *blooto::BMagic<Impl>::moves except that the
 *occupancy is assumed to already have been "masked".  Look at the following
 *source or read up on the internet about magic bitboard move generation to
 *understand the usage of these methods and what it means by "an occupancy that
 *has already been masked".  Using the following methods when possible might be
 *a tiny bit faster than using blooto::RMagic<Impl>::moves and
 *blooto::BMagic<Impl>::moves *because it avoids an array access
 *and a 64-bit & operation.
 *
 *BitBoard blooto::BMagic<Impl>::moves_nomask(Square square,
 *                                            BitBoard occupancy);
 *BitBoard blooto::RMagic<Impl>::moves_nomask(Square square,
 *                                            BitBoard occupancy);
 *
 *If BMagicMin and RMagicMin is used as Impl template parameter:
 *The move bitboard generator will use up 841kb of memory.
 *41kb of memory is used for the bishop database and 800kb is used for the rook
 *database.  If you feel the 800kb rook database is too big, then use
 *BMagicFast and RMagicFast as Impl template parameter for more traditional
 *move bitboard generator in conjunction with
 *the magic move bitboard generator for bishops.
 *
 *If BMagicFast and RMagicFast is used as Impl template parameter:
 *The move bitboard generator will use up 2304kb of memory but might perform
 * a bit faster.
 *
 *Copyright (C) 2007 Pradyumna Kannan.
 *
 *This code is provided 'as-is', without any expressed or implied warranty.
 *In no event will the authors be held liable for any damages arising from
 *the use of this code. Permission is granted to anyone to use this
 *code for any purpose, including commercial applications, and to alter
 *it and redistribute it freely, subject to the following restrictions:
 *
 *1. The origin of this code must not be misrepresented; you must not
 *claim that you wrote the original code. If you use this code in a
 *product, an acknowledgment in the product documentation would be
 *appreciated but is not required.
 *
 *2. Altered source versions must be plainly marked as such, and must not be
 *misrepresented as being the original code.
 *
 *3. This notice may not be removed or altered from any source distribution.
 */

#ifndef _BLOOTO_MAGICMOVES_HPP
#define _BLOOTO_MAGICMOVES_HPP

#include <cstdint>

#include <blooto/bitboard.hpp>

namespace blooto {

    class BMagicCommon {
    protected:
        static const std::uint64_t magics[64];
        static const std::uint64_t mask[64];
        static const unsigned int shift[64];
    };

    class BMagicMin: public BMagicCommon {
        BitBoard db[5248];
        BitBoard *indices[64];
    public:
        BMagicMin();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                *(indices[sq] +
                  (((occupancy.data() & mask[sq]) * magics[sq]) >> shift[sq]));
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                *(indices[sq] + ((occupancy.data() * magics[sq]) >> shift[sq]));
        }
    };

    class BMagicFast: public BMagicCommon {
        BitBoard db[64][1<<9];
        constexpr static inline unsigned minimal_bits_shift(unsigned square) {
            return 55;
        }
    public:
        BMagicFast();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[sq][((occupancy.data() & mask[sq]) * magics[sq]) >>
                       minimal_bits_shift(sq)];
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[sq][(occupancy.data() * magics[sq]) >>
                       minimal_bits_shift(sq)];
        }
    };

    class BMagicPerfectHash: public BMagicCommon {
        BitBoard db[1428];
        std::uint16_t indices[64][1<<9];
        constexpr static inline unsigned minimal_bits_shift(unsigned square) {
            return 55;
        }
    public:
        BMagicPerfectHash();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][((occupancy.data() & mask[sq]) * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][(occupancy.data() * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
    };

    class BMagicPerfectHashVarShift: public BMagicCommon {
        BitBoard db[1428];
        std::uint16_t indices[64][1<<9];
        static inline unsigned minimal_bits_shift(unsigned square) {
            return shift[square];
        }
    public:
        BMagicPerfectHashVarShift();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][((occupancy.data() & mask[sq]) * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][(occupancy.data() * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
    };

    template <typename Impl = BMagicMin> class BMagic {
        Impl impl;
        BMagic() {}
        static const BMagic &instance() {
            static BMagic inst_;
            return inst_;
        }
    public:
        static inline BitBoard moves(const Square square,
                                     const BitBoard occupancy)
        {
            return instance().impl.moves(square, occupancy);
        }
        static inline BitBoard moves_nomask(const Square square,
                                            const BitBoard occupancy)
        {
            return instance().impl.moves_nomask(square, occupancy);
        }
    };

    class RMagicCommon {
    protected:
        static const std::uint64_t magics[64];
        static const std::uint64_t mask[64];
        static const unsigned int shift[64];
    };

    class RMagicMin: public RMagicCommon {
        BitBoard db[102400];
        BitBoard *indices[64];
    public:
        RMagicMin();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                *(indices[sq] +
                  (((occupancy.data() & mask[sq]) * magics[sq]) >> shift[sq]));
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                *(indices[sq] + ((occupancy.data() * magics[sq]) >> shift[sq]));
        }
    };

    class RMagicFast: public RMagicCommon {
        BitBoard db[64][1<<12];
        constexpr static inline unsigned minimal_bits_shift(unsigned square) {
            return 52;
        }
    public:
        RMagicFast();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[sq][((occupancy.data() & mask[sq]) * magics[sq]) >>
                       minimal_bits_shift(sq)];
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[sq][(occupancy.data() * magics[sq]) >>
                       minimal_bits_shift(sq)];
        }
    };

    class RMagicPerfectHash: public RMagicCommon {
        BitBoard db[4900];
        std::uint16_t indices[64][1<<12];
        constexpr static inline unsigned minimal_bits_shift(unsigned square) {
            return 52;
        }
    public:
        RMagicPerfectHash();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][((occupancy.data() & mask[sq]) * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][(occupancy.data() * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
    };

    class RMagicPerfectHashVarShift: public RMagicCommon {
        BitBoard db[4900];
        std::uint16_t indices[64][1<<12];
        static inline unsigned minimal_bits_shift(unsigned square) {
            return shift[square];
        }
    public:
        RMagicPerfectHashVarShift();
        BitBoard moves(const Square square, const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][((occupancy.data() & mask[sq]) * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
        BitBoard moves_nomask(const Square square,
                              const BitBoard occupancy) const
        {
            const unsigned sq = static_cast<unsigned>(square);
            return
                db[indices[sq][(occupancy.data() * magics[sq]) >>
                               minimal_bits_shift(sq)]];
        }
    };

    template <typename Impl = RMagicMin> class RMagic {
        Impl impl;
        RMagic() {}
        static const RMagic &instance() {
            static RMagic inst_;
            return inst_;
        }
    public:
        static inline BitBoard moves(const Square square,
                                     const BitBoard occupancy)
        {
            return instance().impl.moves(square, occupancy);
        }
        static inline BitBoard moves_nomask(const Square square,
                                            const BitBoard occupancy)
        {
            return instance().impl.moves_nomask(square, occupancy);
        }
    };

    template <typename BMagicImpl = BMagicMin, typename RMagicImpl = RMagicMin>
    class QMagic {
    public:
        static inline BitBoard moves(const Square square,
                                     const BitBoard occupancy)
        {
            return BMagic<BMagicImpl>::moves(square, occupancy) |
                   RMagic<RMagicImpl>::moves(square, occupancy);
        }
        inline BitBoard moves_nomask(const Square square,
                                     const BitBoard occupancy)
        {
            return BMagic<BMagicImpl>::moves_nomask(square, occupancy) |
                   RMagic<RMagicImpl>::moves_nomask(square, occupancy);
        }
    };

} // namespace blooto

#endif // _BLOOTO_MAGICMOVES_HPP
