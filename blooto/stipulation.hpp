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
            virtual result_type operator()(const Board &) = 0;
            virtual ~Requirement() {}
        };

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
            result_type operator()(const Board &) override {
                if (num_results > 0)
                    return {};
                return {Failed::NotFound};
            }
        };

        struct RequireAllOrMate: Requirement {
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
            result_type operator()(const Board &board) override {
                if (num_results == 0) {
                    Board newboard{board};
                    newboard.flip_colour();
                    if (!threat_to_king(newboard)) // Stalemate
                        return {Failed::NotFound};
                }
                return {};
            }
        };

        struct RequireMate: Requirement {
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
            result_type operator()(const Board &board) override {
                if (num_illegal > 0) {
                    Board newboard{board};
                    newboard.flip_colour();
                    if (!threat_to_king(newboard)) // Stalemate
                        return {Failed::NotFound};
                    return {};
                }
                return {Failed::NotFound};
            }
        };

        class Solver;

        using SolverList = std::list<Solver>;
        using SolverListIterator = SolverList::const_iterator;

        class Solver {
            using result_type = Requirement::Result;
            using func_type = result_type(const Board &, SolverListIterator);
            const func_type *funcp_;
        public:
            Solver(func_type *funcp): funcp_{funcp} {}
            result_type operator()(const Board &board,
                                   SolverListIterator solvp) const
            {
                return (*funcp_)(board, solvp);
            }
        };

        template <typename ReqT> struct SolverFuncBase {
            static Requirement::result_type call(const Board &board,
                                                 SolverListIterator solvp,
                                                 ReqT &req,
                                                 Solution::list &result)
            {
                return {};
            }
        };

        template <typename ReqT, typename Base, typename PT>
        class SolverFuncUnit: public Base {

            static BitBoard pieces_can_move(const Board &board,
                                            boost::mpl::false_)
            {
                return board.friendlies();
            }

            static BitBoard pieces_can_move(const Board &board,
                                            boost::mpl::true_)
            {
                return board.neutrals();
            }

            template <typename Neutral>
            static Requirement::result_type call1(const Board &board,
                                                  SolverListIterator solvp,
                                                  ReqT &req,
                                                  Solution::list &result,
                                                  Neutral neutral)
            {
                BitBoard pieces_bb{
                    pieces_can_move(board, neutral) & board.pieces<PT>()
                };
                for (Square from: pieces_bb) {
                    BitBoard moves_from{
                        PT::instance.moves(board.colour(), from,
                                           board.occupied()) &
                                           ~board.friendlies()
                    };
                    for (Square to: moves_from) {
                        Board newboard{board};
                        MoveColour colour{newboard.colour()};
                        newboard.take_piece(from);
                        newboard.put_piece<PT>(to, neutral);
                        newboard.flip_colour();
                        if (PT::instance.can_be_promoted(colour, to)) {
                            for (auto p = Board::promotions().begin();
                                 p != Board::promotions().end(); ++p)
                            {
                                newboard.make_promotion(to, p);
                                Requirement::Result res{
                                    (*solvp)(newboard, solvp)
                                };
                                Requirement::result_type r{
                                    boost::apply_visitor(req, res)
                                };
                                if (r)
                                    return r;
                                if (auto slp = boost::get<Solution::list>(&res))
                                    result.emplace_back(
                                        Move{
                                            PT::instance,
                                            from, to,
                                            board.occupied()[to],
                                            &*p
                                        },
                                        std::move(*slp)
                                    );
                            }
                        } else {
                            Requirement::Result res{
                                (*solvp)(newboard, solvp)
                            };
                            Requirement::result_type r{
                                boost::apply_visitor(req, res)
                            };
                            if (r)
                                return *r;
                            if (auto slp = boost::get<Solution::list>(&res))
                                result.emplace_back(
                                    Move{
                                        PT::instance,
                                        from, to,
                                        board.occupied()[to]
                                    },
                                    std::move(*slp)
                                );
                        }
                    }
                }
                return {};
            }

        public:
            static Requirement::result_type call(const Board &board,
                                                 SolverListIterator solvp,
                                                 ReqT &req,
                                                 Solution::list &result)
            {
                if (Requirement::result_type r =
                    call1(board, solvp, req, result, boost::mpl::false_()))
                {
                    return r;
                }
                if (Requirement::result_type r =
                    call1(board, solvp, req, result, boost::mpl::true_()))
                {
                    return r;
                }
                return Base::call(board, solvp, req, result);
            }
        };

        template <typename ReqT>
        using SolverFunc =
            typename boost::mpl::fold<Board::piecetypes_t,
                                      SolverFuncBase<ReqT>,
                                      SolverFuncUnit<ReqT,
                                                     boost::mpl::_1,
                                                     boost::mpl::_2>>::type;

        class ThreatFuncBase {
            const Board &board_;
            const BitBoard unfriendly_kings_;
        public:
            ThreatFuncBase(const Board &board)
            : board_{board}
            , unfriendly_kings_{board.pieces<KingType>() & board.unfriendlies()}
            {}
            const Board &board() const {return board_;}
            const BitBoard unfriendly_kings() const {return unfriendly_kings_;}
            bool operator()() const {return false;}
        };

        template <typename Base, typename PT> struct ThreatFuncUnit: Base {
            using Base::Base;
            using Base::board;
            using Base::unfriendly_kings;
            bool operator()() const {
                BitBoard pieces_bb{
                    board().can_move() & board().template pieces<PT>()
                };
                for (Square from: pieces_bb) {
                    BitBoard moves_from{
                        PT::instance.moves(board().colour(), from,
                                           board().occupied())
                    };
                    if (!(moves_from & unfriendly_kings()).empty())
                        return true;
                }
                return Base::operator()();
            }
        };

        using ThreatFunc =
            typename boost::mpl::fold<Board::piecetypes_t,
                                      ThreatFuncBase,
                                      ThreatFuncUnit<boost::mpl::_1,
                                                     boost::mpl::_2>>::type;

        static inline bool threat_to_king(const Board &board) {
            return ThreatFunc(board)();
        }

        static Requirement::Result solver_end(const Board &board,
                                              SolverListIterator solvp)
        {
            Solution::list result;
            if (threat_to_king(board))
                return Failed::IllegalMove;
            return Solution::list{};
        }

        template <typename ReqT>
        static Requirement::Result solver(const Board &board,
                                          SolverListIterator solvp)
        {
            if (threat_to_king(board))
                return Failed::IllegalMove;

            ReqT req;
            ++solvp;
            Solution::list result;
            Requirement::result_type r{
                SolverFunc<ReqT>::call(board, solvp, req, result)
            };
            if (r)
                return *r;
            r = req(board);
            if (r)
                return *r;
            return result;
        }

        MoveColour first_move_colour_;
        SolverList solvlist_;

        Stipulation(MoveColour first_move_colour, SolverList &&solvlist)
        : first_move_colour_ {first_move_colour}, solvlist_{solvlist} {}

    public:

        //! Colour of the first move
        //! @return move colour
        MoveColour first_move_colour() const {return first_move_colour_;}

        //! Create stipulation for directmate
        //! @param num_moves number of moves
        //! @return stipulation
        static Stipulation directmate(unsigned num_moves) {
            SolverList solvlist;
            for (unsigned i = 1; i < num_moves; ++i) {
                solvlist.emplace_back(&solver<RequireAny>);
                solvlist.emplace_back(&solver<RequireAllOrMate>);
            }
            solvlist.emplace_back(&solver<RequireAny>);
            solvlist.emplace_back(&solver<RequireMate>);
            solvlist.emplace_back(&solver_end);
            return {ColourWhite(), std::move(solvlist)};
        }

        //! Create stipulation for helpmate
        //! @param num_moves number of moves
        //! @return stipulation
        static Stipulation helpmate(unsigned num_moves) {
            SolverList solvlist;
            for (unsigned i = 0; i < num_moves; ++i) {
                solvlist.emplace_back(&solver<RequireAny>);
                solvlist.emplace_back(&solver<RequireAny>);
            }
            solvlist.emplace_back(&solver<RequireMate>);
            solvlist.emplace_back(&solver_end);
            return {ColourBlack(), std::move(solvlist)};
        }

        //! Create stipulation for helpmate for uneven number of half-moves
        //! @param num_moves number of full moves
        //! @return stipulation
        static Stipulation helpmate_1(unsigned num_moves) {
            SolverList solvlist;
            for (unsigned i = 0; i < num_moves; ++i) {
                solvlist.emplace_back(&solver<RequireAny>);
                solvlist.emplace_back(&solver<RequireAny>);
            }
            solvlist.emplace_back(&solver<RequireAny>);
            solvlist.emplace_back(&solver<RequireMate>);
            solvlist.emplace_back(&solver_end);
            return {ColourWhite(), std::move(solvlist)};
        }

        //! Solve chess composition problem with given board
        //! @param board board to solve problem for
        //! @result list of solutions (empty if no solutions found)
        Solution::list solve(const Board &board) const {
            const Solver &solv{solvlist_.front()};
            Requirement::Result res{solv(board, solvlist_.begin())};
            if (auto slp = boost::get<Solution::list>(&res))
                return std::move(*slp);
            else
                return Solution::list();
        }

    };

}

#endif

