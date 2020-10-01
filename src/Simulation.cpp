#include "Simulation.h"
#include "Log.h"

namespace erfseds_nova {
	namespace test_data {

		Simulation::Simulation(SimulationParameters& params)  : m_Params(params)
		{
			NOVA_TRACE("Simulation initalized");
		}

		void Simulation::Run()
		{
			double time = 0.0;
			SimulationData data;
			data.DT = m_Params.StepSize / m_Params.OversamplingRate;
			std::size_t i = 0;
			while (true)
			{
				data.Gravity = { 0.0, -9.81, 0.0 };
				data.Time = time;

				for (auto& ship : m_Params.Ships)
				{
					ship.Update(data);
				}

				time += data.DT;
				if (i % m_Params.OversamplingRate == 0)
				{
					NOVA_INFO("Tick: {}. ", data.Time);
					for (const auto& ship : m_Params.Ships)
					{
						NOVA_INFO("{} at [{}, {}, {}]", ship.GetName(), ship.GetPosition().x, ship.GetPosition().y, ship.GetPosition().z);
					}

				}
				i++;
				if (time > 1.5) break;

			}
		}

	}
}


