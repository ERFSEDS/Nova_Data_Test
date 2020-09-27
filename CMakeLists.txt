cmake_minimum_required(VERSION 3.0.0)
project (test_data)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

add_executable(test_data ${sources})

include_directories("vendor/glm")
