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

#ifndef _BLOOTO_BISHOPTYPE_HPP
#define _BLOOTO_BISHOPTYPE_HPP

#include <blooto/slidertype.hpp>
#include <blooto/magicmoves.hpp>

namespace blooto {
    //! Class containing bishop-specific operations.
    struct BishopType: SliderType<BMagic<>, BishopType> {
        //! String piece code
        constexpr static const char *codestring = "B";
    };
}

#endif
