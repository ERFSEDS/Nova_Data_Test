
#include "Parser.h"
#include "Log.h"

#include <simdjson.h>
#include <chrono>

namespace erfseds_nova {
	namespace test_data {
		namespace Parser
		{
			std::string ParseBodyJSON(const std::filesystem::path& path, std::vector<Body>& result)
			{
				NOVA_TRACE("About to open rockets file {}", path);

				auto start = std::chrono::high_resolution_clock::now();
				simdjson::dom::parser parser;
				simdjson::dom::element bodies = parser.load(path);
				if (!bodies.is_array()) return "Rockets JSON file must contain an array of rocket objects!";

				auto root = bodies.get_array();
				for (int i = 0; i < root.size(); i++)
				{
					NOVA_TRACE("Beginning read of body at root index {}", i);
					auto element = root.at(i).get_object();
					if (!element.is_object()) return std::string("root array index ") + std::to_string(i) + " must be an object";
					std
				}

				auto length = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
				NOVA_TRACE("Parsed file successfully in {:.1} milliseconds", length / 1000.0);
				return "";//Success
			}
		}

	}
}

