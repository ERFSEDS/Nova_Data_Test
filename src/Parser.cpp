
#include "Parser.h"
#include "Log.h"

#include <simdjson.h>
#include <chrono>

namespace erfseds_nova {
	namespace test_data {

		glm::dvec3 ReadDVec3(const simdjson::dom::object& object, const char* name)
		{
			const auto& array = object[name].get_array();
			double x = array.at(0).get_double();
			double y = array.at(1).get_double();
			double z = array.at(2).get_double();

			return { x, y, z };
		}

		std::string Parser::ParseShipJSON(const std::string& path, std::vector<Ship>& result)
		{
			NOVA_TRACE("About to open rockets file {}", path);

			auto start = std::chrono::high_resolution_clock::now();
			simdjson::dom::parser parser;
			simdjson::dom::element ships = parser.load(path);
			if (!ships.is_array()) return "Rockets JSON file must contain an array of rocket objects!";

			const auto& root = ships.get_array();
			for (int shipIndex = 0; shipIndex < root.size(); shipIndex++)
			{
				NOVA_TRACE("Beginning read of ship at root index {}", shipIndex);
				const auto& element = root.at(shipIndex);
				if (!element.is_object()) return std::string("root array index ") + std::to_string(shipIndex) + " must be an object";\
				const auto& object = element.get_object().first;
				Ship ship;

				ship.m_Delay = object["delay"].get_double();
				ship.m_Mass = object["mass"].get_double();

				const auto& stringView = object["name"].get_string().first;
				ship.m_Name.assign(stringView.begin(), stringView.end());
				ship.m_Position = ReadDVec3(object, "position");

				const auto& engines = object["engines"].get_array();
				for (int engineIndex = 0; engineIndex < engines.size(); engineIndex++)
				{
					NOVA_TRACE("Reading engine #{} from ship: {}", engineIndex, shipIndex);
					const auto& engineJSON = engines.at(engineIndex);

					std::vector<KeyFrame<double, double>> curve;

					const auto& thrustCurveData = engineJSON["thrust_curve"].get_array();
					for (int keyFrameIndex = 0; keyFrameIndex < thrustCurveData.size(); keyFrameIndex++)
					{
						const auto& keyFrame = thrustCurveData.at(keyFrameIndex);
						double time = keyFrame["time"].get_double();
						double thrust = keyFrame["thrust"].get_double();

						curve.push_back({ time, thrust });
						NOVA_TRACE("Reading data point at T={}, Thrust={}N", time, thrust);
					}

					Engine engine(curve);

					engine.Direction = ReadDVec3(engineJSON, "direction");
					engine.Offset = ReadDVec3(engineJSON, "offset");
					engine.DryMass = engineJSON["dry_mass"].get_double();
					engine.WetMass = engineJSON["wet_mass"].get_double();
					engine.ExpectedImpulse = engineJSON["total_impulse"].get_double();
					engine.Mass = engine.WetMass;
					ship.m_Engines.push_back(engine);

					NOVA_TRACE("Successfully parsed engine wet mass: {}, dry mass: {}, direction: [{}, {}, {}], offset: [{}, {}, {}], and {} thrust curve points", 
						engine.WetMass, engine.DryMass, engine.Direction.x, engine.Direction.y, engine.Direction.z, engine.Offset.x, engine.Offset.y, engine.Offset.z, engine.ForceCurve.DataPoints());
				}
				NOVA_TRACE("Successfully parsed ship #{} - \"{}\"  ", shipIndex, ship.m_Name);

				result.push_back(ship);
			}

			auto length = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
			NOVA_TRACE("Parsed file successfully in {:.5} milliseconds", length / 1000.0);
			return "";//Success
		}

	}
}

