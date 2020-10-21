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
			SimulationData data;
			data.DT = m_Params.StepSize / m_Params.OversamplingRate;
			data.Time = 0;
			std::size_t i = 0;

			m_Params.OutputFile << "TimeStep,StepNum,AccelX,AccelY,AccelZ,GryoX,GryoY,GryoZ,Pressure,TrueAltitude,TrueAngle" << std::endl;
			auto start = std::chrono::high_resolution_clock::now();

			while (true)
			{
				data.Gravity = { 0.0, -9.81, 0.0 };

				for (auto& ship : m_Params.Ships)
				{
					ship.Update(data);
				}

				data.Time += data.DT;

				if (i % m_Params.OversamplingRate == 0)
				{
					for (const auto& ship : m_Params.Ships)
					{
						m_Params.OutputFile
							<< data.Time << "," << (i / m_Params.OversamplingRate) << ","
							<< ship.GetAccereration().x << "," << ship.GetAccereration().y << "," << ship.GetAccereration().z << ","
							<< ship.GetAttitude().x << "," << ship.GetAttitude().y << "," << ship.GetAttitude().z << "," << /*pressure*/ 0.0 << "," << ship.GetPosition().y << "," << /*true angle*/ 0.0 << "," << std::endl;
						;

					}

				}
				i++;
				if (data.Time > 15) break;
			}

			NOVA_INFO("Simulation Complete");
			auto micros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
			auto seconds = micros / 1000000.0;

			NOVA_TRACE("Simulation took {:.5} seconds", seconds);
			auto iterations = data.Time / data.DT;
			NOVA_TRACE("Computed {} iterations ({} iterations per second)", static_cast<std::uint64_t>(iterations), iterations / seconds);

			for (auto& ship : m_Params.Ships)
			{
				for (auto& engine : ship.GetEngines())
				{
					NOVA_INFO("Mass burned error for ship {} is {:.2}%, Expected: {}kg, got: {}kg", ship.GetName(), glm::abs(engine.Mass - engine.DryMass) / engine.DryMass * 100.0, engine.DryMass, engine.Mass);
				}
			}
		}

	}
}


