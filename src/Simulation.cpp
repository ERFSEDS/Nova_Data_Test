#include "Simulation.h"
#include "Log.h"

namespace erfseds_nova {
	namespace test_data {

		Simulation::Simulation(SimulationParameters& params)  : m_Params(params)
		{
			NOVA_TRACE("Simulation initalized");
		}

		void Simulation::Begin()
		{
			
		}

	}
}


