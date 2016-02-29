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

#include <boost/lexical_cast.hpp>

#include <blooto/piece.hpp>

blooto::Piece blooto::Piece::from_code(const std::string& code,
                                       blooto::PieceColour colour)
{
    std::string::const_iterator p = code.begin();
    if (*p == '\0' || (*p >= '0' && *p <= '9'))
        throw ParseError("Empty piece code: " + code);
    do {
        ++p;
        if (*p == '\0')
            throw ParseError("Empty piece position: " + code);
    } while (*p < '0' || *p > '9');
    --p;
    std::string piececode(code.begin(), p);
    const PieceType *pt = PieceType::get(piececode);
    if (pt == nullptr)
        throw ParseError("Unknown piece code: " + code);
    try {
        Square sq = boost::lexical_cast<Square>(std::string(p, code.end()));
        return Piece(sq, *pt, colour);
    } catch (const boost::bad_lexical_cast &) {
        throw ParseError("Wrong piece position: " + code);
    }
}
