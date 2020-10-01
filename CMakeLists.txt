cmake_minimum_required(VERSION 3.0.0)

project(test_data)

add_subdirectory("vendor/spdlog")

set(SIMDJSON_JUST_LIBRARY true)
set(SIMDJSON_BUILD_STATIC true)
add_subdirectory("vendor/simdjson")

add_subdirectory(src)
add_subdirectory(test)
