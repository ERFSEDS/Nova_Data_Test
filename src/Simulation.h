#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Body.h"

namespace erfseds_nova {
	namespace test_data {

		struct SimulationData
		{
			double Time;//Total time in seconds since the simulation started
			double DT;//The change in time since the last update
			glm::dvec3 Gravity;//Gravity in m/s^2 along each axis. Constant for now
		};

		struct SimulationParameters {
			std::fstream OutputFile;
			std::vector<Ship> Ships;
			double StepSize;
			int OversamplingRate;
		};

		class Simulation {
			public:
				Simulation(SimulationParameters& params);

				void Run();

			private:
				SimulationParameters& m_Params;
				
		};

	}
}

