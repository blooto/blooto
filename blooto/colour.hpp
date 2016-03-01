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

#ifndef _BLOOTO_COOUR_HPP
#define _BLOOTO_COOUR_HPP

#include <boost/variant.hpp>

namespace blooto {

    //! Tag for neutral pieces.
    struct ColourNeutral {};

    //! Tag for white pieces.
    struct ColourWhite {};

    //! Tag for black pieces.
    struct ColourBlack {};

    //! Output neutral colour name to output stream
    //! @param out output stream
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_ostream<CharT, CharTraits> &
    operator<<(std::basic_ostream<CharT, CharTraits> &out, ColourNeutral)
    {
        static const CharT name[] = {'n', 'e', 'u', 't', 'r', 'a', 'l', '\0'};
        return out << name;
    }

    //! Output white colour name to output stream
    //! @param out output stream
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_ostream<CharT, CharTraits> &
    operator<<(std::basic_ostream<CharT, CharTraits> &out, ColourWhite)
    {
        static const CharT name[] = {'w', 'h', 'i', 't', 'e', '\0'};
        return out << name;
    }

    //! Output black colour name to output stream
    //! @param out output stream
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_ostream<CharT, CharTraits> &
    operator<<(std::basic_ostream<CharT, CharTraits> &out, ColourBlack)
    {
        static const CharT name[] = {'b', 'l', 'a', 'c', 'k', '\0'};
        return out << name;
    }

    //! @cond detail
    namespace detail {

        struct friendly_or_neutral_visitor: boost::static_visitor<bool> {
            bool operator()(ColourWhite, ColourBlack) const {return false;}
            bool operator()(ColourBlack, ColourWhite) const {return false;}
            template <typename C1, typename C2>
            bool operator()(C1, C2) const {return true;}
        };

        struct friendly_only_visitor: boost::static_visitor<bool> {
            bool operator()(ColourWhite, ColourWhite) const {return true;}
            bool operator()(ColourBlack, ColourBlack) const {return true;}
            template <typename C1, typename C2>
            bool operator()(C1, C2) const {return false;}
        };

        struct equal_type_visitor: boost::static_visitor<bool> {
            template <typename T, typename U>
            bool operator()(T, U) const {return false;}
            template <typename T> bool operator()(T, T) const {return true;}
        };

        template <typename CharT, typename CharTraits>
        class output_visitor:
        public boost::static_visitor<std::basic_ostream<CharT, CharTraits> &>
        {
            using ostream_type = std::basic_ostream<CharT, CharTraits>;
            ostream_type &out_;
        public:
            constexpr output_visitor(ostream_type &out): out_(out) {}
            template <typename T> ostream_type &operator()(const T &obj) const
            {
                return out_ << obj;
            }
        };
    }
    //! @endcond

    //! Piece colour
    class PieceColour: public boost::variant<ColourNeutral,
                                             ColourWhite,
                                             ColourBlack>
    {
        using base_type = boost::variant<ColourNeutral,
                                         ColourWhite,
                                         ColourBlack>;

    public:

        //! Construct colour for neutral piece
        //! @param arg colour tag
        PieceColour(ColourNeutral arg): base_type{arg} {}

        //! Construct colour for white piece
        //! @param arg colour tag
        PieceColour(ColourWhite arg): base_type{arg} {}

        //! Construct colour for black piece
        //! @param arg colour tag
        PieceColour(ColourBlack arg): base_type{arg} {}

        //! Default copy constructor
        //! @param other object to construct from
        PieceColour(const PieceColour &other) = default;

        //! Default move constructor
        //! @param other object to construct from
        PieceColour(PieceColour &&other) = default;

        //! Default copy assignment
        //! @param other piece colour to assign
        PieceColour &operator=(const PieceColour &other) = default;

        //! Default move assignment
        //! @param other piece colour to assign
        PieceColour &operator=(PieceColour &&other) = default;

        //! Check for other piece's friendliness.
        //! @param c colour of other piece
        //! @return true if other piece is friendly or neutral, false otherwise
        bool friendly_or_neutral(PieceColour c) const {
            return boost::apply_visitor(detail::friendly_or_neutral_visitor(),
                                        *this, c);
        }

        //! Compare for equality.
        //! @param rhs other colour
        //! @return true if both colours are the same
        bool operator==(PieceColour rhs) const {
            return boost::apply_visitor(detail::equal_type_visitor(),
                                        *this, rhs);
        }

        //! Compare for inequality.
        //! @param rhs other colour
        //! @return true if both colours are not the same
        bool operator!=(PieceColour rhs) const {
            return !boost::apply_visitor(detail::equal_type_visitor(),
                                         *this, rhs);
        }

    };

    //! Output piece colour to output stream
    //! @param out output stream
    //! @param pc piece colour to output
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_ostream<CharT, CharTraits> &
    operator<<(std::basic_ostream<CharT, CharTraits> &out, PieceColour pc)
    {
        using visitor = detail::output_visitor<CharT, CharTraits>;
        return boost::apply_visitor(visitor(out), pc);
    }

    //! Move colour
    class MoveColour: public boost::variant<ColourWhite, ColourBlack> {
        using base_type = boost::variant<ColourWhite, ColourBlack>;

    public:

        //! Construct white move colour
        //! @param arg colour tag
        MoveColour(ColourWhite arg): base_type{arg} {}

        //! Construct black move colour
        //! @param arg colour tag
        MoveColour(ColourBlack arg): base_type{arg} {}

        //! Default copy constructor
        //! @param other object to construct from
        MoveColour(const MoveColour &other) = default;

        //! Default move constructor
        //! @param other object to construct from
        MoveColour(MoveColour &&other) = default;

        //! Default copy assignment
        //! @param other move colour to assign
        MoveColour &operator=(const MoveColour &other) = default;

        //! Default move assignment
        //! @param other move colour to assign
        MoveColour &operator=(MoveColour &&other) = default;

        //! Opposite move colour.
        //! @return colour opposite to this one
        MoveColour opposite() const {
            struct visitor: boost::static_visitor<MoveColour> {
                MoveColour operator()(ColourWhite) const {return ColourBlack();}
                MoveColour operator()(ColourBlack) const {return ColourWhite();}
            };
            return boost::apply_visitor(visitor(), *this);
        }

        //! Check if a piece can be moved during this move.
        //! @param c colour of the piece
        //! @return true if the piece can move, false otherwise
        bool can_move(PieceColour c) const {
            return boost::apply_visitor(detail::friendly_or_neutral_visitor(),
                                        *this, c);
        }

        //! Check is a piece is friendly (can't be attacked).
        //! @param c colour of the piece
        //! @return true if other piece can't be attacked, false otherwise
        bool friendly(PieceColour c) const {
            return boost::apply_visitor(detail::friendly_only_visitor(),
                                        *this, c);
        }

        //! Convert move colour to piece colour
        //! @return piece colour corresponding to move colour
        PieceColour to_piece_colour() const {
            struct visitor: boost::static_visitor<PieceColour> {
                PieceColour operator()(ColourWhite) const {
                    return ColourWhite();
                }
                PieceColour operator()(ColourBlack) const {
                    return ColourBlack();
                }
            };
            return boost::apply_visitor(visitor(), *this);
        }

        //! Compare for equality.
        //! @param rhs other colour
        //! @return true if both colours are the same
        bool operator==(MoveColour rhs) const {
            return boost::apply_visitor(detail::equal_type_visitor(),
                                        *this, rhs);
        }

        //! Compare for inequality.
        //! @param rhs other colour
        //! @return true if both colours are not the same
        bool operator!=(MoveColour rhs) const {
            return !boost::apply_visitor(detail::equal_type_visitor(),
                                         *this, rhs);
        }

    };

    //! Output move colour to output stream
    //! @param out output stream
    //! @param mc move colour to output
    //! @return reference to output stream for operator chaining
    template<typename CharT, typename CharTraits>
    std::basic_ostream<CharT, CharTraits> &
    operator<<(std::basic_ostream<CharT, CharTraits> &out, MoveColour mc)
    {
        using visitor = detail::output_visitor<CharT, CharTraits>;
        return boost::apply_visitor(visitor(out), mc);
    }

}

#endif
