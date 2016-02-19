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

// This file is modified version of magicmoves.c by Pradyumna Kannan.
// The original copyright notice follows.

/**
 *magicmoves.h
 *
 *Source file for magic move bitboard generation.
 *
 *See header file for instructions on usage.
 *
 *The magic keys are not optimal for all squares but they are very close
 *to optimal.
 *
 *Copyright (C) 2007 Pradyumna Kannan.
 *
 *This code is provided 'as-is', without any express or implied warranty.
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

#include <blooto/magicmoves.hpp>

#ifdef _MSC_VER
#pragma warning( disable : 4312)
#pragma warning( disable : 4146)
#endif

//For rooks

//original 12 bit keys
//0x0000002040810402ULL - H8 12 bit
//0x0000102040800101ULL - A8 12 bit
//0x0000102040008101ULL - B8 11 bit
//0x0000081020004101ULL - C8 11 bit

//Adapted Grant Osborne's keys
//0x0001FFFAABFAD1A2ULL - H8 11 bit
//0x00FFFCDDFCED714AULL - A8 11 bit
//0x007FFCDDFCED714AULL - B8 10 bit
//0x003FFFCDFFD88096ULL - C8 10 bit

const unsigned int blooto::RMagic::shift[64] = {
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 53, 53, 53, 53, 53
};

const std::uint64_t blooto::RMagic::magics[64] = {
    0x0080001020400080ULL, 0x0040001000200040ULL,
    0x0080081000200080ULL, 0x0080040800100080ULL,
    0x0080020400080080ULL, 0x0080010200040080ULL,
    0x0080008001000200ULL, 0x0080002040800100ULL,
    0x0000800020400080ULL, 0x0000400020005000ULL,
    0x0000801000200080ULL, 0x0000800800100080ULL,
    0x0000800400080080ULL, 0x0000800200040080ULL,
    0x0000800100020080ULL, 0x0000800040800100ULL,
    0x0000208000400080ULL, 0x0000404000201000ULL,
    0x0000808010002000ULL, 0x0000808008001000ULL,
    0x0000808004000800ULL, 0x0000808002000400ULL,
    0x0000010100020004ULL, 0x0000020000408104ULL,
    0x0000208080004000ULL, 0x0000200040005000ULL,
    0x0000100080200080ULL, 0x0000080080100080ULL,
    0x0000040080080080ULL, 0x0000020080040080ULL,
    0x0000010080800200ULL, 0x0000800080004100ULL,
    0x0000204000800080ULL, 0x0000200040401000ULL,
    0x0000100080802000ULL, 0x0000080080801000ULL,
    0x0000040080800800ULL, 0x0000020080800400ULL,
    0x0000020001010004ULL, 0x0000800040800100ULL,
    0x0000204000808000ULL, 0x0000200040008080ULL,
    0x0000100020008080ULL, 0x0000080010008080ULL,
    0x0000040008008080ULL, 0x0000020004008080ULL,
    0x0000010002008080ULL, 0x0000004081020004ULL,
    0x0000204000800080ULL, 0x0000200040008080ULL,
    0x0000100020008080ULL, 0x0000080010008080ULL,
    0x0000040008008080ULL, 0x0000020004008080ULL,
    0x0000800100020080ULL, 0x0000800041000080ULL,
    0x00FFFCDDFCED714AULL, 0x007FFCDDFCED714AULL,
    0x003FFFCDFFD88096ULL, 0x0000040810002101ULL,
    0x0001000204080011ULL, 0x0001000204000801ULL,
    0x0001000082000401ULL, 0x0001FFFAABFAD1A2ULL
};

const std::uint64_t blooto::RMagic::mask[64] = {
    0x000101010101017EULL, 0x000202020202027CULL,
    0x000404040404047AULL, 0x0008080808080876ULL,
    0x001010101010106EULL, 0x002020202020205EULL,
    0x004040404040403EULL, 0x008080808080807EULL,
    0x0001010101017E00ULL, 0x0002020202027C00ULL,
    0x0004040404047A00ULL, 0x0008080808087600ULL,
    0x0010101010106E00ULL, 0x0020202020205E00ULL,
    0x0040404040403E00ULL, 0x0080808080807E00ULL,
    0x00010101017E0100ULL, 0x00020202027C0200ULL,
    0x00040404047A0400ULL, 0x0008080808760800ULL,
    0x00101010106E1000ULL, 0x00202020205E2000ULL,
    0x00404040403E4000ULL, 0x00808080807E8000ULL,
    0x000101017E010100ULL, 0x000202027C020200ULL,
    0x000404047A040400ULL, 0x0008080876080800ULL,
    0x001010106E101000ULL, 0x002020205E202000ULL,
    0x004040403E404000ULL, 0x008080807E808000ULL,
    0x0001017E01010100ULL, 0x0002027C02020200ULL,
    0x0004047A04040400ULL, 0x0008087608080800ULL,
    0x0010106E10101000ULL, 0x0020205E20202000ULL,
    0x0040403E40404000ULL, 0x0080807E80808000ULL,
    0x00017E0101010100ULL, 0x00027C0202020200ULL,
    0x00047A0404040400ULL, 0x0008760808080800ULL,
    0x00106E1010101000ULL, 0x00205E2020202000ULL,
    0x00403E4040404000ULL, 0x00807E8080808000ULL,
    0x007E010101010100ULL, 0x007C020202020200ULL,
    0x007A040404040400ULL, 0x0076080808080800ULL,
    0x006E101010101000ULL, 0x005E202020202000ULL,
    0x003E404040404000ULL, 0x007E808080808000ULL,
    0x7E01010101010100ULL, 0x7C02020202020200ULL,
    0x7A04040404040400ULL, 0x7608080808080800ULL,
    0x6E10101010101000ULL, 0x5E20202020202000ULL,
    0x3E40404040404000ULL, 0x7E80808080808000ULL
};

//my original tables for bishops
const unsigned int blooto::BMagic::shift[64] = {
    58, 59, 59, 59, 59, 59, 59, 58,
    59, 59, 59, 59, 59, 59, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 55, 55, 57, 59, 59,
    59, 59, 57, 57, 57, 57, 59, 59,
    59, 59, 59, 59, 59, 59, 59, 59,
    58, 59, 59, 59, 59, 59, 59, 58
};

const std::uint64_t blooto::BMagic::magics[64] = {
    0x0002020202020200ULL, 0x0002020202020000ULL,
    0x0004010202000000ULL, 0x0004040080000000ULL,
    0x0001104000000000ULL, 0x0000821040000000ULL,
    0x0000410410400000ULL, 0x0000104104104000ULL,
    0x0000040404040400ULL, 0x0000020202020200ULL,
    0x0000040102020000ULL, 0x0000040400800000ULL,
    0x0000011040000000ULL, 0x0000008210400000ULL,
    0x0000004104104000ULL, 0x0000002082082000ULL,
    0x0004000808080800ULL, 0x0002000404040400ULL,
    0x0001000202020200ULL, 0x0000800802004000ULL,
    0x0000800400A00000ULL, 0x0000200100884000ULL,
    0x0000400082082000ULL, 0x0000200041041000ULL,
    0x0002080010101000ULL, 0x0001040008080800ULL,
    0x0000208004010400ULL, 0x0000404004010200ULL,
    0x0000840000802000ULL, 0x0000404002011000ULL,
    0x0000808001041000ULL, 0x0000404000820800ULL,
    0x0001041000202000ULL, 0x0000820800101000ULL,
    0x0000104400080800ULL, 0x0000020080080080ULL,
    0x0000404040040100ULL, 0x0000808100020100ULL,
    0x0001010100020800ULL, 0x0000808080010400ULL,
    0x0000820820004000ULL, 0x0000410410002000ULL,
    0x0000082088001000ULL, 0x0000002011000800ULL,
    0x0000080100400400ULL, 0x0001010101000200ULL,
    0x0002020202000400ULL, 0x0001010101000200ULL,
    0x0000410410400000ULL, 0x0000208208200000ULL,
    0x0000002084100000ULL, 0x0000000020880000ULL,
    0x0000001002020000ULL, 0x0000040408020000ULL,
    0x0004040404040000ULL, 0x0002020202020000ULL,
    0x0000104104104000ULL, 0x0000002082082000ULL,
    0x0000000020841000ULL, 0x0000000000208800ULL,
    0x0000000010020200ULL, 0x0000000404080200ULL,
    0x0000040404040400ULL, 0x0002020202020200ULL
};

const std::uint64_t blooto::BMagic::mask[64] = {
    0x0040201008040200ULL, 0x0000402010080400ULL,
    0x0000004020100A00ULL, 0x0000000040221400ULL,
    0x0000000002442800ULL, 0x0000000204085000ULL,
    0x0000020408102000ULL, 0x0002040810204000ULL,
    0x0020100804020000ULL, 0x0040201008040000ULL,
    0x00004020100A0000ULL, 0x0000004022140000ULL,
    0x0000000244280000ULL, 0x0000020408500000ULL,
    0x0002040810200000ULL, 0x0004081020400000ULL,
    0x0010080402000200ULL, 0x0020100804000400ULL,
    0x004020100A000A00ULL, 0x0000402214001400ULL,
    0x0000024428002800ULL, 0x0002040850005000ULL,
    0x0004081020002000ULL, 0x0008102040004000ULL,
    0x0008040200020400ULL, 0x0010080400040800ULL,
    0x0020100A000A1000ULL, 0x0040221400142200ULL,
    0x0002442800284400ULL, 0x0004085000500800ULL,
    0x0008102000201000ULL, 0x0010204000402000ULL,
    0x0004020002040800ULL, 0x0008040004081000ULL,
    0x00100A000A102000ULL, 0x0022140014224000ULL,
    0x0044280028440200ULL, 0x0008500050080400ULL,
    0x0010200020100800ULL, 0x0020400040201000ULL,
    0x0002000204081000ULL, 0x0004000408102000ULL,
    0x000A000A10204000ULL, 0x0014001422400000ULL,
    0x0028002844020000ULL, 0x0050005008040200ULL,
    0x0020002010080400ULL, 0x0040004020100800ULL,
    0x0000020408102000ULL, 0x0000040810204000ULL,
    0x00000A1020400000ULL, 0x0000142240000000ULL,
    0x0000284402000000ULL, 0x0000500804020000ULL,
    0x0000201008040200ULL, 0x0000402010080400ULL,
    0x0002040810204000ULL, 0x0004081020400000ULL,
    0x000A102040000000ULL, 0x0014224000000000ULL,
    0x0028440200000000ULL, 0x0050080402000000ULL,
    0x0020100804020000ULL, 0x0040201008040200ULL
};

static std::uint64_t initmagicmoves_occ(const int* squares,
                                        const int numSquares,
                                        const std::uint64_t linocc)
{
    int i;
    std::uint64_t ret=0;
    for (i = 0; i < numSquares; i++) {
        if (linocc & (((std::uint64_t)(1)) << i))
            ret |= (((std::uint64_t)(1)) << squares[i]);
    }
    return ret;
}

static blooto::BitBoard initmagicmoves_Rmoves(const int square,
                                              const std::uint64_t occ)
{
    std::uint64_t ret = 0;
    std::uint64_t bit;
    std::uint64_t rowbits = std::uint64_t(0xFF) << (8 * (square / 8));

    bit = std::uint64_t(1) << square;
    do {
        bit <<= 8;
        ret |= bit;
    } while(bit && !(bit & occ));

    bit = std::uint64_t(1) << square;
    do {
        bit >>= 8;
        ret |= bit;
    } while (bit && !(bit & occ));

    bit = std::uint64_t(1) << square;
    do {
        bit <<= 1;
        if(bit & rowbits)
            ret |= bit;
        else
            break;
    } while (!(bit & occ));

    bit = std::uint64_t(1) << square;
    do {
        bit >>= 1;
        if (bit & rowbits)
            ret |= bit;
        else
            break;
    } while (!(bit & occ));

    return blooto::BitBoard(ret);
}

static blooto::BitBoard initmagicmoves_Bmoves(const int square,
                                              const std::uint64_t occ)
{
    std::uint64_t ret = 0;
    std::uint64_t bit;
    std::uint64_t bit2;
    std::uint64_t rowbits = std::uint64_t(0xFF) << (8 * (square / 8));

    bit = std::uint64_t(1) << square;
    bit2 = bit;
    do {
        bit <<= 8 - 1;
        bit2 >>= 1;
        if (bit2 & rowbits)
            ret |= bit;
        else
            break;
    } while (bit && !(bit & occ));

    bit = std::uint64_t(1) << square;
    bit2 = bit;
    do {
        bit <<= 8 + 1;
        bit2 <<= 1;
        if (bit2 & rowbits)
            ret |= bit;
        else
            break;
    } while (bit && !(bit & occ));

    bit = std::uint64_t(1) << square;
    bit2 = bit;
    do {
        bit >>= 8 - 1;
        bit2 <<= 1;
        if (bit2 & rowbits)
            ret |= bit;
        else
            break;
    } while (bit && !(bit & occ));

    bit = std::uint64_t(1) << square;
    bit2 = bit;
    do {
        bit >>= 8 + 1;
        bit2 >>= 1;
        if (bit2 & rowbits)
            ret |= bit;
        else
            break;
    } while (bit && !(bit & occ));

    return blooto::BitBoard(ret);
}

// used so that the original indices can be left as const
// so that the compiler can optimize better

#ifndef PERFECT_MAGIC_HASH
#ifdef MINIMIZE_MAGIC
#define BmagicNOMASK2(square, occupancy) \
  *(indices[square] + \
  (((occupancy)*magics[square]) >> shift[square]))
#define RmagicNOMASK2(square, occupancy) \
  *(indices[square] + \
  (((occupancy)*magics[square]) >> shift[square]))
#else
#define BmagicNOMASK2(square, occupancy) \
  db[square][((occupancy) * magics[square]) >> \
             MINIMAL_B_BITS_SHIFT(*this, square)]
#define RmagicNOMASK2(square, occupancy) \
  db[square][((occupancy) * magics[square]) >> \
             MINIMAL_R_BITS_SHIFT(*this, square)]
#endif
/*#else
#define BmagicNOMASK2(square, occupancy) \
  db[indices[square][((occupancy) * magics[square])>>MINIMAL_B_BITS_SHIFT]]
#define RmagicNOMASK2(square, occupancy) \
  db[indices[square][((occupancy) * magics[square])>>MINIMAL_R_BITS_SHIFT]]
*/
#endif

//for bitscans :
//initmagicmoves_bitpos64_database[(x*0x07EDD5E59A4E28C2ULL)>>58]
static const int bitpos64_database[64] = {
    63,  0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5
};

blooto::BMagic::BMagic()
#ifdef MINIMIZE_MAGIC
: indices{
    db+4992, db+2624, db+256,  db+896,
    db+1280, db+1664, db+4800, db+5120,
    db+2560, db+2656, db+288,  db+928,
    db+1312, db+1696, db+4832, db+4928,
    db+0,    db+128,  db+320,  db+960,
    db+1344, db+1728, db+2304, db+2432,
    db+32,   db+160,  db+448,  db+2752,
    db+3776, db+1856, db+2336, db+2464,
    db+64,   db+192,  db+576,  db+3264,
    db+4288, db+1984, db+2368, db+2496,
    db+96,   db+224,  db+704,  db+1088,
    db+1472, db+2112, db+2400, db+2528,
    db+2592, db+2688, db+832,  db+1216,
    db+1600, db+2240, db+4864, db+4960,
    db+5056, db+2720, db+864,  db+1248,
    db+1632, db+2272, db+4896, db+5184
}
#endif
{
    int i;

#ifdef PERFECT_MAGIC_HASH
    for (i = 0; i < 1428; i++)
        db[i] = BitBoard{};
#endif

    for (i = 0; i < 64; i++) {
        int squares[64];
        int numsquares = 0;
        std::uint64_t temp = mask[i];
        while (temp) {
            std::uint64_t bit = temp & -temp;
            squares[numsquares++] =
                bitpos64_database[(bit * 0x07EDD5E59A4E28C2ULL) >> 58];
            temp ^= bit;
        }
        for (temp = 0; temp < (std::uint64_t(1) << numsquares); temp++) {
            std::uint64_t tempocc = initmagicmoves_occ(squares,
                                                       numsquares,
                                                       temp);
#ifndef PERFECT_MAGIC_HASH
            BmagicNOMASK2(i, tempocc) = initmagicmoves_Bmoves(i, tempocc);
#else
            BitBoard moves = initmagicmoves_Bmoves(i, tempocc);
            std::uint64_t index =
                (tempocc * magics[i]) >> MINIMAL_B_BITS_SHIFT(*this, i);
            int j;
            for (j = 0; j < 1428; j++) {
                if (db[j].empty()) {
                    db[j] = moves;
                    indices[i][index] = j;
                    break;
                } else if (db[j] == moves) {
                    indices[i][index] = j;
                    break;
                }
            }
#endif
        }
    }

}

blooto::RMagic::RMagic()
#ifdef MINIMIZE_MAGIC
: indices{
    db+86016, db+73728, db+36864, db+43008,
    db+47104, db+51200, db+77824, db+94208,
    db+69632, db+32768, db+38912, db+10240,
    db+14336, db+53248, db+57344, db+81920,
    db+24576, db+33792, db+6144,  db+11264,
    db+15360, db+18432, db+58368, db+61440,
    db+26624, db+4096,  db+7168,  db+0,
    db+2048,  db+19456, db+22528, db+63488,
    db+28672, db+5120,  db+8192,  db+1024,
    db+3072,  db+20480, db+23552, db+65536,
    db+30720, db+34816, db+9216,  db+12288,
    db+16384, db+21504, db+59392, db+67584,
    db+71680, db+35840, db+39936, db+13312,
    db+17408, db+54272, db+60416, db+83968,
    db+90112, db+75776, db+40960, db+45056,
    db+49152, db+55296, db+79872, db+98304
}
#endif
{
    int i;

#ifdef PERFECT_MAGIC_HASH
    for (i = 0; i < 4900; i++)
        db[i] = BitBoard{};
#endif

    for (i = 0; i < 64; i++) {
        int squares[64];
        int numsquares = 0;
        std::uint64_t temp = mask[i];
        while(temp) {
            std::uint64_t bit = temp & -temp;
            squares[numsquares++] =
                bitpos64_database[(bit * 0x07EDD5E59A4E28C2ULL) >> 58];
                temp ^= bit;
        }
        for (temp = 0; temp < (std::uint64_t(1) << numsquares); temp++) {
            std::uint64_t tempocc = initmagicmoves_occ(squares,
                                                       numsquares,
                                                       temp);
#ifndef PERFECT_MAGIC_HASH
            RmagicNOMASK2(i, tempocc) = initmagicmoves_Rmoves(i, tempocc);
#else
            BitBoard moves = initmagicmoves_Rmoves(i, tempocc);
            std::uint64_t index =
                (tempocc * magics[i]) >> MINIMAL_R_BITS_SHIFT(*this, i);
            int j;
            for (j = 0; j < 4900; j++) {
                if (db[j].empty()) {
                    db[j] = moves;
                    indices[i][index] = j;
                    break;
                } else if (db[j] == moves) {
                    indices[i][index] = j;
                    break;
                }
            }
#endif
        }
    }
}
