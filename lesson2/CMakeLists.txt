cmake_minimum_required(VERSION 3.18)
project(cmake_demo)

find_package(fmt)

set(CMAKE_CXX_STANDARD 20)

# Make compiler wild! Try to use these options everywhere
add_compile_options(-Wall -Werror)

add_library(extmem SHARED src/memdev.cpp src/ranked_device.cpp src/sequential_device.cpp)

add_subdirectory(tests)