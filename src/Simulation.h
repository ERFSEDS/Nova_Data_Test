#pragma once

#include <filesystem>
#include <vector>

#include "Body.h"

namespace erfseds_nova {
	namespace test_data {


		struct SimulationParameters {
			std::filesystem::path OutputFile;
			std::vector<Body> Bodies;
			double StepSize;
		};

		class Simulation {
			public:
				Simulation(SimulationParameters& params);

				void Begin();

			private:
				SimulationParameters& m_Params;
				
		};

	}
}