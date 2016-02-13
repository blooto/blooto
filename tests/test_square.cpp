#include <string>
#include <boost/lexical_cast.hpp>

#include "square.hpp"

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
