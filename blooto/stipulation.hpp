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

#ifndef _BLOOTO_STIPULATION_HPP
#define _BLOOTO_STIPULATION_HPP

#include <list>
#include <cstdint>
#include <utility>
#include <memory>
#include <functional>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <blooto/colour.hpp>
#include <blooto/board.hpp>
#include <blooto/solution.hpp>

namespace blooto {

    //! Chess composition stipulation and solver
    class Stipulation {

        enum class Failed: std::uint8_t {NotFound, IllegalMove};

        struct Requirement: boost::static_visitor<boost::optional<Failed>> {
            using Result = boost::variant<Solution::list, Failed>;
            virtual result_type operator()(const Solution::list &) = 0;
            virtual result_type operator()(Failed) = 0;
            virtual result_type operator()() = 0;
            virtual ~Requirement() {}
        };

        template <typename ReqT> struct create_req {
            std::unique_ptr<Requirement> operator()() const {
                return std::unique_ptr<ReqT>{new ReqT};
            }
        };

        using ReqFactory = std::function<std::unique_ptr<Requirement>()>;
        using ReqFactoryList = std::list<ReqFactory>;
        using ReqFactoryListIterator = ReqFactoryList::const_iterator;

        struct RequireAny: Requirement {
            unsigned num_results = 0;
            result_type operator()(Failed fail) override {
                switch (fail) {
                case Failed::NotFound: return {};
                case Failed::IllegalMove: return {};
                }
            }
            result_type operator()(const Solution::list &) override {
                ++num_results;
                return {};
            }
            result_type operator()() override {
                if (num_results > 0)
                    return {};
                return {Failed::NotFound};
            }
        };

        struct RequireAll: Requirement {
            unsigned num_results = 0;
            result_type operator()(Failed fail) override {
                switch (fail) {
                case Failed::NotFound: return {Failed::NotFound};
                case Failed::IllegalMove: return {};
                }
            }
            result_type operator()(const Solution::list &) override {
                ++num_results;
                return {};
            }
            result_type operator()() override {
                if (num_results > 0)
                    return {};
                return {Failed::NotFound};
            }
        };

        struct RequireAllIllegal: Requirement {
            unsigned num_illegal = 0;
            result_type operator()(Failed fail) override {
                switch (fail) {
                case Failed::NotFound: return {Failed::NotFound};
                case Failed::IllegalMove: ++num_illegal; return {};
                }
            }
            result_type operator()(const Solution::list &) override {
                return {Failed::NotFound};
            }
            result_type operator()() override {
                if (num_illegal > 0)
                    return {};
                return {Failed::NotFound};
            }
        };

        static Requirement::Result solve(const Board &board,
                                         ReqFactoryListIterator reqp,
                                         ReqFactoryListIterator reqend)
        {
            Solution::list result;
            for (Square from: board.can_move())
                for (Square to: board.moves_from(from))
                    if (board.is_unfriendly_king(to))
                        return Failed::IllegalMove;
            if (reqp == reqend)
                return result;
            std::unique_ptr<Requirement> req{(*reqp)()};
            ++reqp;
            for (Square from: board.can_move()) {
                for (Square to: board.moves_from(from)) {
                    Board newboard{board};
                    newboard.make_move(from, to);
                    newboard.flip_colour();
                    Requirement::Result res{solve(newboard, reqp, reqend)};
                    Requirement::result_type r{boost::apply_visitor(*req, res)};
                    if (r)
                        return *r;
                    if (auto slp = boost::get<Solution::list>(&res))
                        result.emplace_back(board.move(from, to),
                                            std::move(*slp));
                }
            }
            Requirement::result_type r{(*req)()};
            if (r)
                return *r;
            return result;
        }

        MoveColour first_move_colour_;
        ReqFactoryList reqlist_;

        Stipulation(MoveColour first_move_colour, ReqFactoryList &&reqlist)
        : first_move_colour_ {first_move_colour}, reqlist_{reqlist} {}

    public:

        //! Colour of the first move
        //! @return move colour
        MoveColour first_move_colour() const {return first_move_colour_;}

        //! Create stipulation for directmate
        //! @param num_moves number ov moves
        //! @return stipulation
        static Stipulation directmate(unsigned num_moves) {
            ReqFactoryList reqlist;
            for (unsigned i = 1; i < num_moves; ++i) {
                reqlist.push_back(create_req<RequireAny>());
                reqlist.push_back(create_req<RequireAll>());
            }
            reqlist.push_back(create_req<RequireAny>());
            reqlist.push_back(create_req<RequireAllIllegal>());
            return {ColourWhite(), std::move(reqlist)};
        }

        //! Create stipulation for helpmate
        //! @param num_moves number ov moves
        //! @return stipulation
        static Stipulation helpmate(unsigned num_moves) {
            ReqFactoryList reqlist;
            for (unsigned i = 0; i < num_moves; ++i) {
                reqlist.push_back(create_req<RequireAny>());
                reqlist.push_back(create_req<RequireAny>());
            }
            reqlist.push_back(create_req<RequireAllIllegal>());
            return {ColourBlack(), std::move(reqlist)};
        }

        //! Solve chess composition problem with given board
        //! @param board board to solve problem for
        //! @result list of solutions (empty if no solutions found)
        Solution::list solve(const Board &board) const {
            Requirement::Result res{
                solve(board, reqlist_.begin(), reqlist_.end())
            };
            if (auto slp = boost::get<Solution::list>(&res))
                return std::move(*slp);
            else
                return Solution::list();
        }

    };

}

#endif
