find_package(Boost 1.47.0 REQUIRED)
add_library(blooto bitscan.cpp magicmoves.cpp piece.cpp board.cpp kingtype.cpp)
include_directories(${blooto_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
link_directories(${blooto_BINARY_DIR})
