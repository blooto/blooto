/**
 *magicmoves.h
 *
 *Header file for magic move bitboard generation.  Include this in any files
 *need this functionality.
 *
 *Usage:
 *You must first initialize the generator with a call to initmagicmoves().
 *Then you can use the following macros for generating move bitboards by
 *giving them a square and an occupancy.  The macro will then "return"
 *the correct move bitboard for that particular square and occupancy. It
 *has been named Rmagic and Bmagic so that it will not conflict with
 *any functions/macros in your chess program called Rmoves/Bmoves. You
 *can macro Bmagic/Rmagic to Bmoves/Rmoves if you wish.  If you want to
 *minimize the size of the bitboards, make MINIMIZE_MAGIC uncommented in this
 *header (more info on this later).  Where you typedef your unsigned 64-bit
 *integer declare __64_BIT_INTEGER_DEFINED__.  If PERFECT_MAGIC_HASH is
 *uncomment, the move generator will use an additional indrection to make the
 *table sizes smaller : (~50kb+((original size)/sizeof(PERFECT_MAGIC_HASH)).
 *The size listed from here on out are the sizes without PERFECT_MAGIC_HASH.
 *
 *Bmagic(square, occupancy)
 *Rmagic(square, occupancy)
 *
 *Square is an integer that is greater than or equal to zero and less than 64.
 *Occupancy is any unsigned 64-bit integer that describes which squares on
 *the board are occupied.
 *
 *The following macros are identical to Rmagic and Bmagic except that the
 *occupancy is assumed to already have been "masked".  Look at the following
 *source or read up on the internet about magic bitboard move generation to
 *understand the usage of these macros and what it means by "an occupancy that
 *has already been masked".  Using the following macros when possible might be
 *a tiny bit faster than using Rmagic and Bmagic because it avoids an array
 *access and a 64-bit & operation.
 *
 *BmagicNOMASK(square, occupancy)
 *RmagicNOMASK(square, occupancy)
 *
 *If MINIMIZE_MAGIC is defined before including this file:
 *The move bitboard generator will use up 841kb of memory.
 *41kb of memory is used for the bishop database and 800kb is used for the rook
 *database.  If you feel the 800kb rook database is too big, then comment it
 *out and use a more traditional move bitboard generator in conjunction with
 *the magic move bitboard generator for bishops.
 *
 *If MINIMIAZE_MAGIC is not defined before including this file:
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

/*********MODIFY THE FOLLOWING IF NECESSARY********/
//the default configuration is the best

//Uncommont either one of the following or none
#define MINIMIZE_MAGIC
//#define PERFECT_MAGIC_HASH unsigned short

//the following works only for perfect magic hash or no defenitions above
//it uses variable shift for each square
//#define VARIABLE_SHIFT

namespace blooto {

    extern const std::uint64_t magicmoves_r_magics[64];
    extern const std::uint64_t magicmoves_r_mask[64];
    extern const std::uint64_t magicmoves_b_magics[64];
    extern const std::uint64_t magicmoves_b_mask[64];
    extern const unsigned int magicmoves_b_shift[64];
    extern const unsigned int magicmoves_r_shift[64];

#ifndef VARIABLE_SHIFT
#define MINIMAL_B_BITS_SHIFT(square) 55
#define MINIMAL_R_BITS_SHIFT(square) 52
#else
#define MINIMAL_B_BITS_SHIFT(square) magicmoves_b_shift[square]
#define MINIMAL_R_BITS_SHIFT(square) magicmoves_r_shift[square]
#endif

#ifndef PERFECT_MAGIC_HASH
#ifdef MINIMIZE_MAGIC

    //extern BitBoard magicmovesbdb[5248];
    extern const BitBoard *magicmoves_b_indices[64];

    //extern BitBoard magicmovesrdb[102400];
    extern const BitBoard *magicmoves_r_indices[64];

#else //Don't Minimize database size

    extern BitBoard magicmovesbdb[64][1<<9];
    extern BitBoard magicmovesrdb[64][1<<12];

#endif //MINIMIAZE_MAGICMOVES

#else //PERFCT_MAGIC_HASH defined

#ifndef MINIMIZE_MAGIC

    extern BitBoard magicmovesbdb[1428];
    extern BitBoard magicmovesrdb[4900];
    extern PERFECT_MAGIC_HASH magicmoves_b_indices[64][1<<9];
    extern PERFECT_MAGIC_HASH magicmoves_r_indices[64][1<<12];

#else

#error magicmoves - MINIMIZED_MAGIC and PERFECT_MAGIC_HASH \
       cannot be used together

#endif

#endif //PERFCT_MAGIC_HASH

    inline BitBoard Bmagic(const Square square, const BitBoard occupancy)
    {
        const unsigned sq = static_cast<unsigned>(square);

#ifndef PERFECT_MAGIC_HASH

#ifdef MINIMIZE_MAGIC
        return
            *(magicmoves_b_indices[sq] +
              (((occupancy.data() & magicmoves_b_mask[sq]) *
                magicmoves_b_magics[sq]) >> magicmoves_b_shift[sq]));
#else
        return
            magicmovesbdb[sq][((occupancy.data() & magicmoves_b_mask[sq]) *
                                   magicmoves_b_magics[sq]) >>
                                  MINIMAL_B_BITS_SHIFT(sq)];
#endif

#else

        return
            magicmovesbdb[magicmoves_b_indices[sq]
                                              [((occupancy.data() &
                                                 magicmoves_b_mask[sq]) *
                                                magicmoves_b_magics[sq]) >>
                                               MINIMAL_B_BITS_SHIFT(sq)]];

#endif
    }

    inline BitBoard Rmagic(const Square square, const BitBoard occupancy)
    {
        const unsigned sq = static_cast<unsigned>(square);

#ifndef PERFECT_MAGIC_HASH

#ifdef MINIMIZE_MAGIC
        return
            *(magicmoves_r_indices[sq] +
              (((occupancy.data() & magicmoves_r_mask[sq]) *
                magicmoves_r_magics[sq]) >> magicmoves_r_shift[sq]));

#else
        return
            magicmovesrdb[sq][((occupancy.data() & magicmoves_r_mask[sq]) *
                                   magicmoves_r_magics[sq]) >>
                                  MINIMAL_R_BITS_SHIFT(sq)];
#endif

#else

        return
            magicmovesrdb[magicmoves_r_indices[sq]
                                              [((occupancy.data() &
                                                 magicmoves_r_mask[sq]) *
                                                magicmoves_r_magics[sq]) >>
                                               MINIMAL_R_BITS_SHIFT(sq)]];

#endif
    }

    inline BitBoard BmagicNOMASK(const Square square, const BitBoard occupancy)
    {
        const unsigned sq = static_cast<unsigned>(square);

#ifndef PERFECT_MAGIC_HASH

#ifdef MINIMIZE_MAGIC
        return
            *(magicmoves_b_indices[sq] +
              ((occupancy.data() * magicmoves_b_magics[sq]) >>
               magicmoves_b_shift[sq]));
#else
        return
            magicmovesbdb[sq][(occupancy.data() * magicmoves_b_magics[sq]) >>
                              MINIMAL_B_BITS_SHIFT(sq)];
#endif

#else
        return
            magicmovesbdb[magicmoves_b_indices[sq]
                                              [(occupancy.data() *
                                                magicmoves_b_magics[sq]) >>
                                               MINIMAL_B_BITS_SHIFT(sq)]];

#endif
    }

    inline BitBoard RmagicNOMASK(const Square square, const BitBoard occupancy)
    {
        const unsigned sq = static_cast<unsigned>(square);

#ifndef PERFECT_MAGIC_HASH

#ifdef MINIMIZE_MAGIC
        return
            *(magicmoves_r_indices[sq] +
              ((occupancy.data() * magicmoves_r_magics[sq]) >>
               magicmoves_r_shift[sq]));
#else

        return
            magicmovesrdb[sq][(occupancy.data() * magicmoves_r_magics[sq]) >>
                              MINIMAL_R_BITS_SHIFT(sq)];

#endif

#else
        return
            magicmovesrdb[magicmoves_r_indices[sq]
                                              [(occupancy.data() *
                                                magicmoves_r_magics[sq]) >>
                                               MINIMAL_R_BITS_SHIFT(sq)]];
#endif
    }

    inline BitBoard Qmagic(const Square square, const BitBoard occupancy)
    {
        return Bmagic(square, occupancy) | Rmagic(square, occupancy);
    }

    inline BitBoard QmagicNOMASK(const Square square, const BitBoard occupancy)
    {
        return BmagicNOMASK(square, occupancy) |
               RmagicNOMASK(square, occupancy);
    }

    void initmagicmoves();

} // namespace blooto

#endif // _BLOOTO_MAGICMOVES_HPP
