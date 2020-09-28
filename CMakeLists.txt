cmake_minimum_required(VERSION 3.0.0)

project(test_data)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")

add_executable(test_data ${sources})

set_property(TARGET test_data PROPERTY CXX_STANDARD 17)
set_property(TARGET test_data PROPERTY CXX_STANDARD_REQUIRED ON)


include_directories("vendor/glm")
include_directories("vendor/cxxopts/include")

add_subdirectory("vendor/spdlog")
target_link_libraries(test_data spdlog)

set(SIMDJSON_JUST_LIBRARY true)
set(SIMDJSON_BUILD_STATIC true)
add_subdirectory("vendor/simdjson")
target_link_libraries(test_data simdjson)
