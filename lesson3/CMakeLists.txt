cmake_minimum_required(VERSION 3.18)
project(cmake_demo)

find_package(fmt)

set(CMAKE_CXX_STANDARD 20)

# Make compiler wild! Try to use these options everywhere
add_compile_options(-Wall -Werror) # -fsanitize=thread)
#add_link_options(-fsanitize=thread)


add_executable(bench main.cpp)
target_link_libraries(bench fmt::fmt)

add_executable(pc pc.cpp)
target_link_libraries(pc fmt::fmt)