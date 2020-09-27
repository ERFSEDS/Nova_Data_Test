cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

project (test_data)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

add_executable(test_data ${sources})

include_directories("vendor/glm")
include_directories("vendor/cxxopts/include")
