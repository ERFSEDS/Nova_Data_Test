#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "Body.h"

namespace erfseds_nova {
	namespace test_data {
		namespace Parser
		{
			//Parses the json string into a vector of body structs, returning an error string if the json cannot be parsed. 
			//Returns an empty string on success
			std::string ParseBodyJSON(const std::filesystem::path& path, std::vector<Body>& result);
		}

	}
}
