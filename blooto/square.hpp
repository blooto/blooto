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

#ifndef _BLOOTO_SQUARE_HPP
#define _BLOOTO_SQUARE_HPP

#include <cstdint>
#include <iostream>

namespace blooto {

    //! Chessboard square.

    //! This enum class is a representation of chessboard square.
    enum class Square: std::uint8_t {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8
    };

    //! Input square's name from input stream
    //! @param in input stream
    //! @param sq square
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_istream<CharT, CharTraits> &
    operator>>(std::basic_istream<CharT, CharTraits> &in, Square &sq)
    {
        if (!in.good())
            return in;
        CharT file_ch;
        if (!in.get(file_ch))
            return in;
        if (file_ch < CharT{'a'} || file_ch > CharT{'h'}) {
            in.setstate(std::basic_istream<CharT, CharTraits>::failbit);
            return in;
        }
        CharT rank_ch;
        if (!in.get(rank_ch))
            return in;
        if (rank_ch < CharT{'1'} || rank_ch > CharT{'8'}) {
            in.setstate(std::basic_istream<CharT, CharTraits>::failbit);
            return in;
        }
        sq = Square((std::uint8_t((rank_ch - CharT{'1'}) & 7) << 3) |
                    (std::uint8_t((file_ch - CharT{'a'}) & 7)));
        return in;
    }

    //! Output square's name to output stream
    //! @param out output stream
    //! @param sq square
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_ostream<CharT, CharTraits> &
    operator<<(std::basic_ostream<CharT, CharTraits> &out, Square sq)
    {
        static const CharT ranks[8] = {
            CharT{'1'}, CharT{'2'}, CharT{'3'}, CharT{'4'},
            CharT{'5'}, CharT{'6'}, CharT{'7'}, CharT{'8'}
        };
        static const CharT files[8] = {
            CharT{'a'}, CharT{'b'}, CharT{'c'}, CharT{'d'},
            CharT{'e'}, CharT{'f'}, CharT{'g'}, CharT{'h'}
        };
        unsigned d = static_cast<unsigned>(sq);
        return out << files[d & 7] << ranks[(d >> 3) & 7];
    }

}

#endif
