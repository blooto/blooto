find_package(Boost 1.47.0 COMPONENTS program_options REQUIRED)
include_directories(${blooto_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
link_directories(${blooto_BINARY_DIR})
add_executable(blooto-bin blooto.cpp)
set_target_properties(blooto-bin PROPERTIES OUTPUT_NAME blooto)
target_link_libraries(blooto-bin blooto ${Boost_LIBRARIES})
