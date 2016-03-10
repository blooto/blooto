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

#include <initializer_list>
#include <boost/lexical_cast.hpp>

#include <blooto/stipulation.hpp>
#include <blooto/board.hpp>
#include <blooto/solution.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_stupulation
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_stipulation_directmate) {
    using namespace blooto;
    Stipulation st{Stipulation::directmate(2)};
    std::initializer_list<Piece> pieces{
        {Square::F8, KingType::instance, ColourWhite()},
        {Square::H1, RookType::instance, ColourWhite()},
        {Square::G6, PawnType::instance, ColourWhite()},
        {Square::H8, KingType::instance, ColourBlack()},
        {Square::G8, BishopType::instance, ColourBlack()},
        {Square::G7, PawnType::instance, ColourBlack()},
        {Square::H7, PawnType::instance, ColourBlack()},
    };
    Board board{st.first_move_colour(), pieces};
    Solution::list sl{st.solve(board)};
    Solution::list::const_iterator p1 = sl.begin();
    BOOST_CHECK(p1 != sl.end());
    BOOST_CHECK_EQUAL(&p1->move().piecetype(), &RookType::instance);
    BOOST_CHECK_EQUAL(p1->move().from(), Square::H1);
    BOOST_CHECK_EQUAL(p1->move().to(), Square::H6);
    BOOST_CHECK(!p1->move().attack());
    {
        const Solution::list &sl2 = p1->next();
        Solution::list::const_iterator p2 = sl2.begin();
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &PawnType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G7);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::H6);
        BOOST_CHECK(p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &PawnType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::G6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::G7);
            BOOST_CHECK(!p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G8);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::A2);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &RookType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::H6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::H7);
            BOOST_CHECK(p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G8);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::B3);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &RookType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::H6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::H7);
            BOOST_CHECK(p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G8);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::C4);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &RookType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::H6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::H7);
            BOOST_CHECK(p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G8);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::D5);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &RookType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::H6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::H7);
            BOOST_CHECK(p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G8);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::E6);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &RookType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::H6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::H7);
            BOOST_CHECK(p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::G8);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::F7);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &RookType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::H6);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::H7);
            BOOST_CHECK(p3->move().attack());
            BOOST_CHECK(p3->next().empty());
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 == sl2.end());
    }
    ++p1;
    BOOST_CHECK(p1 == sl.end());
}

BOOST_AUTO_TEST_CASE(test_stipulation_helpmate) {
    using namespace blooto;
    Stipulation st{Stipulation::helpmate(2)};
    std::initializer_list<Piece> pieces{
        {Square::F3, KingType::instance, ColourWhite()},
        {Square::E5, RookType::instance, ColourWhite()},
        {Square::F8, BishopType::instance, ColourWhite()},
        {Square::A4, BishopType::instance, ColourWhite()},
        {Square::F6, KingType::instance, ColourBlack()},
        {Square::F7, PawnType::instance, ColourBlack()},
        {Square::D6, PawnType::instance, ColourBlack()},
    };
    Board board{st.first_move_colour(), pieces};
    Solution::list sl{st.solve(board)};
    Solution::list::const_iterator p1 = sl.begin();
    BOOST_CHECK(p1 != sl.end());
    BOOST_CHECK_EQUAL(&p1->move().piecetype(), &KingType::instance);
    BOOST_CHECK_EQUAL(p1->move().from(), Square::F6);
    BOOST_CHECK_EQUAL(p1->move().to(), Square::E5);
    BOOST_CHECK(p1->move().attack());
    {
        const Solution::list &sl2 = p1->next();
        Solution::list::const_iterator p2 = sl2.begin();
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &BishopType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::A4);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::B3);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &PawnType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::F7);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::F5);
            BOOST_CHECK(!p3->move().attack());
            {
                const Solution::list &sl4 = p3->next();
                Solution::list::const_iterator p4 = sl4.begin();
                BOOST_CHECK(p4 != sl4.end());
                BOOST_CHECK_EQUAL(&p4->move().piecetype(),
                                  &BishopType::instance);
                BOOST_CHECK_EQUAL(p4->move().from(), Square::F8);
                BOOST_CHECK_EQUAL(p4->move().to(), Square::G7);
                BOOST_CHECK(!p4->move().attack());
                BOOST_CHECK(p4->next().empty());
                ++p4;
                BOOST_CHECK(p4 == sl4.end());
            }
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 == sl2.end());
    }
    ++p1;
    BOOST_CHECK(p1 != sl.end());
    BOOST_CHECK_EQUAL(&p1->move().piecetype(), &KingType::instance);
    BOOST_CHECK_EQUAL(p1->move().from(), Square::F6);
    BOOST_CHECK_EQUAL(p1->move().to(), Square::G6);
    BOOST_CHECK(!p1->move().attack());
    {
        const Solution::list &sl2 = p1->next();
        Solution::list::const_iterator p2 = sl2.begin();
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &RookType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::E5);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::H5);
        BOOST_CHECK(!p2->move().attack());
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &PawnType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::F7);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::F6);
            BOOST_CHECK(!p3->move().attack());
            {
                const Solution::list &sl4 = p3->next();
                Solution::list::const_iterator p4 = sl4.begin();
                BOOST_CHECK(p4 != sl4.end());
                BOOST_CHECK_EQUAL(&p4->move().piecetype(),
                                  &BishopType::instance);
                BOOST_CHECK_EQUAL(p4->move().from(), Square::A4);
                BOOST_CHECK_EQUAL(p4->move().to(), Square::E8);
                BOOST_CHECK(!p4->move().attack());
                BOOST_CHECK(p4->next().empty());
                ++p4;
                BOOST_CHECK(p4 == sl4.end());
            }
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 == sl2.end());
    }
    ++p1;
    BOOST_CHECK(p1 == sl.end());
}

BOOST_AUTO_TEST_CASE(test_stipulation_helpmate_2) {
    using namespace blooto;
    Stipulation st{Stipulation::helpmate(2)};
    std::initializer_list<Piece> pieces{
        {Square::H2, PawnType::instance, ColourWhite()},
        {Square::G3, RookType::instance, ColourWhite()},
        {Square::A5, KingType::instance, ColourWhite()},
        {Square::B5, BishopType::instance, ColourWhite()},
        {Square::E7, PawnType::instance, ColourWhite()},
        {Square::E2, PawnType::instance, ColourBlack()},
        {Square::F2, KingType::instance, ColourBlack()},
        {Square::C5, QueenType::instance, ColourBlack()},
        {Square::E5, PawnType::instance, ColourBlack()},
        {Square::F7, RookType::instance, ColourBlack()},
    };
    Board board{st.first_move_colour(), pieces};
    Solution::list sl{st.solve(board)};
    Solution::list::const_iterator p1 = sl.begin();
    BOOST_CHECK(p1 != sl.end());
    BOOST_CHECK_EQUAL(&p1->move().piecetype(), &RookType::instance);
    BOOST_CHECK_EQUAL(p1->move().from(), Square::F7);
    BOOST_CHECK_EQUAL(p1->move().to(), Square::F8);
    BOOST_CHECK(!p1->move().attack());
    BOOST_CHECK(p1->move().promotion() == nullptr);
    {
        const Solution::list &sl2 = p1->next();
        Solution::list::const_iterator p2 = sl2.begin();
        BOOST_CHECK(p2 != sl2.end());
        BOOST_CHECK_EQUAL(&p2->move().piecetype(), &PawnType::instance);
        BOOST_CHECK_EQUAL(p2->move().from(), Square::E7);
        BOOST_CHECK_EQUAL(p2->move().to(), Square::F8);
        BOOST_CHECK(p2->move().attack());
        BOOST_CHECK_EQUAL(p2->move().promotion(), &BishopType::instance);
        {
            const Solution::list &sl3 = p2->next();
            Solution::list::const_iterator p3 = sl3.begin();
            BOOST_CHECK(p3 != sl3.end());
            BOOST_CHECK_EQUAL(&p3->move().piecetype(), &PawnType::instance);
            BOOST_CHECK_EQUAL(p3->move().from(), Square::E2);
            BOOST_CHECK_EQUAL(p3->move().to(), Square::E1);
            BOOST_CHECK(!p3->move().attack());
            BOOST_CHECK_EQUAL(p3->move().promotion(), &KnightType::instance);
            {
                const Solution::list &sl4 = p3->next();
                Solution::list::const_iterator p4 = sl4.begin();
                BOOST_CHECK(p4 != sl4.end());
                BOOST_CHECK_EQUAL(&p4->move().piecetype(),
                                  &BishopType::instance);
                BOOST_CHECK_EQUAL(p4->move().from(), Square::F8);
                BOOST_CHECK_EQUAL(p4->move().to(), Square::C5);
                BOOST_CHECK(p4->move().attack());
                BOOST_CHECK(p4->next().empty());
                BOOST_CHECK(p4->move().promotion() == nullptr);
                ++p4;
                BOOST_CHECK(p4 == sl4.end());
            }
            ++p3;
            BOOST_CHECK(p3 == sl3.end());
        }
        ++p2;
        BOOST_CHECK(p2 == sl2.end());
    }
    ++p1;
    BOOST_CHECK(p1 == sl.end());
}
