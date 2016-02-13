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

#include <blooto/square.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_square
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_square) {
    using namespace blooto;
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(Square::A1), "a1");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(Square::D3), "d3");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(Square::F6), "f6");
    BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(Square::H8), "h8");
}
