cmake_minimum_required(VERSION 3.18)
project(cmake_demo)

find_package(fmt)

set(CMAKE_CXX_STANDARD 20)

# Make compiler wild! Try to use these options everywhere
add_compile_options(-Wall -Werror)

add_executable(cmake_demo main.cpp)
target_link_libraries(cmake_demo A B ce_demo fmt::fmt)

add_library(A STATIC A/libA.cpp)
add_subdirectory(B)

file(GLOB COPY_ELISION_DEMO_SRC CopyElisionDemo/*.cpp)
add_library(ce_demo STATIC ${COPY_ELISION_DEMO_SRC})

add_subdirectory(tests)