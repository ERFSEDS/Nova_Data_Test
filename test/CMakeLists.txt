
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE sources CONFIGURE_DEPENDS "../src/*.cpp")
list(FILTER sources EXCLUDE REGEX ".*Main.cpp$")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

add_executable(test_data_unit ${sources})

include_directories("../test")


include_directories("../vendor/glm")
include_directories("../vendor/cxxopts/include")
include_directories("../vendor/Catch2/single_include")


target_link_libraries(test_data_unit spdlog)

target_link_libraries(test_data_unit simdjson)

set_target_properties(test_data_unit
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)
