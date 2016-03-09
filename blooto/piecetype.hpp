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

#ifndef _BLOOTO_PIECETYPE_HPP
#define _BLOOTO_PIECETYPE_HPP

#include <string>
#include <boost/bimap.hpp>

#include <blooto/square.hpp>
#include <blooto/colour.hpp>

namespace blooto {

    class BitBoard;

    //! Base class for singleton classes containing piece specific operations.
    class PieceType {

        class Registry {
            using reg_type = boost::bimap<std::string, const PieceType *>;
            reg_type registry;
            Registry() {};

        public:

            static Registry &instance() {
                static Registry inst;
                return inst;
            }

            static void add(const std::string &code, const PieceType *pt) {
                instance().registry.insert(reg_type::value_type(code, pt));
            }

            static const PieceType *get(const std::string &code) {
                auto p = instance().registry.left.find(code);
                return
                    p == instance().registry.left.end() ? nullptr : p->second;
            }

            static const std::string &get(const PieceType *pt) {
                return instance().registry.right.find(pt)->second;
            }
        };

    protected:

        //! Construct piece type with string code
        //! @param code string piece code
        //! This code is used to register this piece type,
        //! so it can be found by string code
        //! and string code can be foind by piece type.
        PieceType(const std::string &code) {
            Registry::add(code, this);
        }

    public:
        //! Virtual destructor.
        virtual ~PieceType() noexcept {}

        //! Get piece type from its string code.
        //! @param code string code
        //! @return pointer to piece type or nullptr if no piece found
        static const PieceType *get(const std::string &code) {
            return Registry::get(code);
        };

        //! Get string piece code ftom its type.
        //! @param pt piece type
        //! @return piece code
        static const std::string &code(const PieceType *pt) {
            return Registry::get(pt);
        }

        //! One-letter code of the piece type.
        //! @return one-letter code
        const std::string &code() const {return code(this);}

        //! All possible moves from given square for given colour and occupancy.
        //! @param colour move colour
        //! @param square square to originate moves from
        //! @param occupancy BitBoard containing occipoed squares
        //! @return BitBoard containing squares this piece can move to
        virtual BitBoard moves(MoveColour colour,
                               Square square,
                               BitBoard occupancy) const = 0;

        //! Check whether this piece type can be promoted on given square
        //! @param colour move colour
        //! @param square square where the piece is located
        //! @return true if the piece can be promoted
        virtual bool can_be_promoted(MoveColour colour, Square square) const {
            return false;
        }

        //! Check whether this piece type can be a candidate to promote to
        //! @return true if other pieces can be promoted to this one
        virtual bool can_be_promotion() const {return true;}

    };

    //! Base class for piece classes automatically registered with string codes.
    //! @param Type concrete piece type
    //!
    //! Concrete type chould be inherited from this template using itself
    //! as the parameter.
    //! Also concrete type must have static member called codestring
    //! containing piece string code.
    //!
    //! For example:
    //! @code
    //! class KingType: public PieceTypeRegistered<KingType> {
    //!     ...
    //! public:
    //!     constexpr static const char *codestring = "K";
    //! };
    //! @endcode
    template <typename Type> class PieceTypeRegistered: public PieceType {
    protected:
        PieceTypeRegistered(): PieceType(Type::codestring) {}
    public:
        //! Instance of piece type
        static const Type instance;
    };

    template <typename Type> const Type PieceTypeRegistered<Type>::instance;
}

#endif
