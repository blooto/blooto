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

#include <sstream>
#include <iostream>
#include <exception>
#include <boost/program_options.hpp>

#include <blooto/solution.hpp>
#include <blooto/stipulation.hpp>

static void print_solutions(const blooto::Solution::list &sl,
                            unsigned indent = 0)
{
    for (const auto &solution: sl) {
        for (unsigned i = 0; i < indent; i++)
            std::cout << "\t";
        std::cout << solution.move() << "\n";
        print_solutions(solution.next(), indent + 1);
    }
}

static int solve(const blooto::Stipulation &st,
                 std::istream &in)
{
    blooto::Board board{st.first_move_colour()};
    in >> board;
    if (in.fail()) {
        std::cerr << "Error reading board." << std::endl;
        return 1;
    }
    blooto::Solution::list sl{st.solve(board)};
    if (sl.empty()) {
        std::cerr << "No solutions." << std::endl;
        return 1;
    }
    print_solutions(sl);
    return 0;
}

int main(int argc, const char *const *argv) try {
    namespace po = boost::program_options;
    using namespace blooto;
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("directmate,d", po::value<unsigned>(), "solve directmate problem")
    ("helpmate,h", po::value<unsigned>(), "solve helpmate problem")
    ("helpmate+1", po::value<unsigned>(),
     "solve helpmate problem with additional half-move")
    ("board,b", po::value<std::string>(), "board content")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    std::istringstream istr;
    std::istream *in;
    if (vm.count("board")) {
        istr.str(vm["board"].as<std::string>());
        in = &istr;
    } else {
        in = &std::cin;
    }
    if (vm.count("directmate")) {
        return solve(Stipulation::directmate(vm["directmate"].as<unsigned>()),
                     *in);
    } else if (vm.count("helpmate")) {
        return solve(Stipulation::helpmate(vm["helpmate"].as<unsigned>()),
                     *in);
    } else if (vm.count("helpmate+1")) {
        return solve(Stipulation::helpmate_1(vm["helpmate+1"].as<unsigned>()),
                     *in);
    } else {
        std::cerr << "No stipulation specified.";
        return 1;
    }
} catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
} catch (...) {
    std::cerr << "Unknown error!" << std::endl;
    return 1;
}
