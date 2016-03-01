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
#include <boost/lexical_cast.hpp>

#include <blooto/colour.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_colour
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_piececolour) {
    using namespace blooto;
    PieceColour pcn{ColourNeutral()};
    BOOST_CHECK_EQUAL(pcn, ColourNeutral());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(pcn), "neutral");
    PieceColour pcw{ColourWhite()};
    BOOST_CHECK_EQUAL(pcw, ColourWhite());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(pcw), "white");
    PieceColour pcb{ColourBlack()};
    BOOST_CHECK_EQUAL(pcb, ColourBlack());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(pcb), "black");
    // Neutral is friendly_or_neutral for everything.
    BOOST_CHECK(pcn.friendly_or_neutral(pcn));
    BOOST_CHECK(pcn.friendly_or_neutral(pcb));
    BOOST_CHECK(pcn.friendly_or_neutral(pcw));
    // White is friendly_or_neutral for everything but black.
    BOOST_CHECK(pcw.friendly_or_neutral(pcn));
    BOOST_CHECK(pcw.friendly_or_neutral(pcw));
    BOOST_CHECK(!pcw.friendly_or_neutral(pcb));
    // Black is friendly_or_neutral for everything but white.
    BOOST_CHECK(pcb.friendly_or_neutral(pcn));
    BOOST_CHECK(!pcb.friendly_or_neutral(pcw));
    BOOST_CHECK(pcb.friendly_or_neutral(pcb));
    PieceColour pc1{pcn};
    BOOST_CHECK_EQUAL(pc1, ColourNeutral());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(pc1), "neutral");
    pc1 = pcw;
    BOOST_CHECK_EQUAL(pc1, ColourWhite());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(pc1), "white");
    pc1 = ColourBlack();
    BOOST_CHECK_EQUAL(pc1, ColourBlack());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(pc1), "black");
}

BOOST_AUTO_TEST_CASE(test_movecolour) {
    using namespace blooto;
    MoveColour mcw{ColourWhite()};
    BOOST_CHECK_EQUAL(mcw, ColourWhite());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(mcw), "white");
    MoveColour mcb{ColourBlack()};
    BOOST_CHECK_EQUAL(mcb, ColourBlack());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(mcb), "black");
    PieceColour pcn{ColourNeutral()};
    PieceColour pcw{ColourWhite()};
    PieceColour pcb{ColourBlack()};
    BOOST_CHECK_EQUAL(mcw.to_piece_colour(), pcw);
    BOOST_CHECK_EQUAL(mcb.to_piece_colour(), pcb);
    // White is friendly to white only.
    BOOST_CHECK(!mcw.friendly(pcn));
    BOOST_CHECK(mcw.friendly(pcw));
    BOOST_CHECK(!mcw.friendly(pcb));
    // Black is friendly to black only.
    BOOST_CHECK(!mcb.friendly(pcn));
    BOOST_CHECK(!mcb.friendly(pcw));
    BOOST_CHECK(mcb.friendly(pcb));
    // White and neutral pieces can move during white move.
    BOOST_CHECK(mcw.can_move(pcn));
    BOOST_CHECK(mcw.can_move(pcw));
    BOOST_CHECK(!mcw.can_move(pcb));
    // Black and neutral pieces can move during black move.
    BOOST_CHECK(mcb.can_move(pcn));
    BOOST_CHECK(!mcb.can_move(pcw));
    BOOST_CHECK(mcb.can_move(pcb));
    MoveColour mc1{mcw};
    BOOST_CHECK_EQUAL(mc1, ColourWhite());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(mc1), "white");
    mc1 = mcb;
    BOOST_CHECK_EQUAL(mc1, ColourBlack());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(mc1), "black");
    mc1 = ColourWhite();
    BOOST_CHECK_EQUAL(mc1, ColourWhite());
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(mc1), "white");
}
