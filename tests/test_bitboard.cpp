#include "bitboard.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_bitboard
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_bitboard) {
    blooto::BitBoard empty_bb;
    BOOST_CHECK(!empty_bb[blooto::Square::A1]);
    BOOST_CHECK(empty_bb.data() == 0);
    blooto::BitBoard all_bb(~empty_bb);
    BOOST_CHECK(all_bb[blooto::Square::A1]);
    blooto::BitBoard d4_bb(blooto::Square::D4);
    BOOST_CHECK(!d4_bb[blooto::Square::A1]);
    BOOST_CHECK(d4_bb[blooto::Square::D4]);
    blooto::BitBoard b2_f6_bb(blooto::Square::B2 | blooto::Square::F6);
    BOOST_CHECK(!b2_f6_bb[blooto::Square::A1]);
    BOOST_CHECK(!b2_f6_bb[blooto::Square::D4]);
    BOOST_CHECK(b2_f6_bb[blooto::Square::B2]);
    BOOST_CHECK(b2_f6_bb[blooto::Square::F6]);
}
