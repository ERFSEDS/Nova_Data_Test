#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "Body.h"

namespace erfseds_nova {
	namespace test_data {

		struct Parser
		{
			//Parses the json string into a vector of body structs, returning an error string if the json cannot be parsed. 
			//Returns an empty string on success
			static std::string ParseShipJSON(const std::filesystem::path& path, std::vector<Ship>& result);
		};

	}
}
