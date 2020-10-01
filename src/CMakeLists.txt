file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

add_executable(test_data ${sources})

include_directories("../vendor/glm")
include_directories("../vendor/cxxopts/include")

target_link_libraries(test_data spdlog)

target_link_libraries(test_data simdjson)

set_target_properties(test_data
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

