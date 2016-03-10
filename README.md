# blooto -- Chess composition solver

This is C++ class library for solving chess composition problems.

## Building

These build instructions assume that you have Linux or other Unix-like
operating system.

You need the following prerequisites to build `blooto`:

- C++ compiler (tested with GCC);
- [Boost C++ libraries](http://www.boost.org/);
- [`cmake`](https://cmake.org/);
- `make` or any other build system `cmake` supports (tested with GNU Make);
- [`doxygen`](http://www.stack.nl/~dimitri/doxygen/index.html) (optional).

Once you've extracted source archive or checked out `git` repository,
enter the directory with sources and perform the following commands there.

It's a good practice to build in a seperate directory.
For example, you can create a subdirectory called `build`
and use it as your build directory.

```sh
mkdir build
cd build
```

After you created and entered your build directory,
you can start build process there.

```sh
cmake ..
make
```

If everything works correctly, you'll get the following results:

- `blooto` library in `lib` subdirectory,
- `blooto` program in `utils` subdirectory,
- unit tests in `tests` subdirectory.

## Running tests

Run the following command in your build directory.

```sh
ctest
```

## Solving problems

The `blooto` utility in `utils` subdirectory of your build directory
can solve chess composition problems.

Run `./utils/blooto --help` for list of its command line options.

### Example: directmate

The command

```sh
./utils/blooto -d 2 -b 'White Kf8 Rh1 Pg6 Black Kh8 Bg8 Pg7 Ph7'
```

solves directmate problem with 2 moves.

The `-d` option specifies thet the type of problem to solve is directmate.
Its argument is number of full moves.

The `-b` option specifies initial board content for the problem.
If `-b` option is omited, board content is read from standard input.

The expected output of this problem's solution is following.

```
Rh1-h6
        Pg7*h6
                Pg6-g7
        Bg8-a2
                Rh6*h7
        Bg8-b3
                Rh6*h7
        Bg8-c4
                Rh6*h7
        Bg8-d5
                Rh6*h7
        Bg8-e6
                Rh6*h7
        Bg8-f7
                Rh6*h7
```

### Example: helpmate

The command

```sh
./utils/blooto -h 2 -b 'White Kf3 Re5 Bf8 Ba4 Black Kf6 Pf7 Pd6'
```

solves helpmate problem with 2 moves.

The `-h` option specifies thet the type of problem to solve is helpmate.
Its argument is number of full moves.

The `-b` option specifies initial board content for the problem.
If `-b` option is omited, board content is read from standard input.

The expected output of this problem's solution is following.

```
Kf6*e5
        Ba4-b3
                Pf7-f5
                        Bf8-g7
Kf6-g6
        Re5-h5
                Pf7-f6
                        Ba4-e8
```

The same composition can be solved with a different stipulation:
helpmate with one full move and additional half-move,
using the following command:

```sh
./utils/blooto --helpmate+1 1 -b 'White Ph2 Rg3 Ka5 Bb5 Pe7 Black Pe2 Kf2 Qc5 Pe5 Rf7'
```

The `--helpmate+1` option specifies thet the type of problem to solve
is helpmate with additional half-move.
Its argument is number of full moves.

The expected output of this problem's solution is following.

```
Pe7-e8=Q
        Pe2-e1=R
                Qe8*f7
```

## Library

The `blooto` utility is just a simple wrapper program.
All problem solving logic is inside `blooto` library.

Although `doxygen` dependency is optional, it's highly recommended
to generate library API documentation.

If you have `doxygen` installed, you'll get HTML documentation
in `doc/html` subdirectory of your build directory
as a result of build process.

