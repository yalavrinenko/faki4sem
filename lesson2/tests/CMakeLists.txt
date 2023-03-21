cmake_minimum_required(VERSION 3.18)

find_package(fmt)

# Here we fetch and include gtest repository
# This approach do not spam into system libs and fetch predictable version of gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

add_executable(all_test any.cpp variant.cpp devmem.cpp)
target_link_libraries(all_test PRIVATE gtest_main extmem fmt::fmt)
target_include_directories(all_test PRIVATE ${CMAKE_SOURCE_DIR})

# Find all tests in test target. Now we can use internal cmake tools to run tests. Also, clion can handle target as a tests.
gtest_discover_tests(all_test)