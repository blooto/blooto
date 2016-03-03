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

#include <string>

#include <blooto/board.hpp>

constexpr const blooto::PieceType *blooto::Board::piecetypes[];

std::istream &blooto::operator>>(std::istream &in, blooto::Board &board) {
    std::string str;
    PieceColour colour{ColourWhite()};
    while (std::getline(in, str, ' ')) {
        if (str == "\n") {
        } else if (str == "Neutral") {
            colour = ColourNeutral();
        } else if (str == "White") {
            colour = ColourWhite();
        } else if (str == "Black") {
            colour = ColourBlack();
        } else {
            try {
                board.insert(Piece::from_code(str, colour));
            } catch (const Piece::ParseError &) {
                in.setstate(std::istream::failbit);
                break;
            }
        }
        if (in.eof())
            break;
    }
    return in;
}

std::ostream &blooto::operator<<(std::ostream &out, const blooto::Board &board)
{
    bool first_neutral = true;
    for (auto p: board) {
        if (p.colour() == ColourNeutral()) {
            if (first_neutral) {
                out << "Neutral";
                first_neutral = false;
            }
            out << " " << p;
        }
    }
    bool first_white = true;
    for (auto p: board) {
        if (p.colour() == ColourWhite()) {
            if (first_white) {
                if (!first_neutral)
                    out << " ";
                out << "White";
                first_white = false;
            }
            out << " " << p;
        }
    }
    bool first_black = true;
    for (auto p: board) {
        if (p.colour() == ColourBlack()) {
            if (first_black) {
                if (!first_neutral || !first_white)
                    out << " ";
                out << "Black";
                first_black = false;
            }
            out << " " << p;
        }
    }
    return out;
}
