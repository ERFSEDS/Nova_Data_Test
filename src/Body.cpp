#include "Body.h"
#include "Simulation.h"

namespace erfseds_nova {
	namespace test_data {
		
		void Body::Update(const SimulationData& data)
		{
			double totalMass = CalculateMass();
			//Apply the force of weight - W=mg
			ImpartForce(totalMass * data.Gravity);
			if (m_Position.y <= 0.0 && m_Force.y < 0.0)
			{
				//Crude normal force approximation making objects on the ground with not enough thrust stay on the ground
				NOVA_TRACE("on ground at T={}, force {}", data.Time, m_Force.y);
				m_Force.y = 0.0;
			}

			//We assume mass is constant during update cycles so F=ma -> a=F/m
			m_Acceleration = m_Force / totalMass;

			//Integrate
			m_Velocity += m_Acceleration * data.DT;
			m_Position += m_Velocity * data.DT;

			//NOVA_TRACE("Net force [{}, {}, {}]", m_Force.x, m_Force.y, m_Force.z);
			//NOVA_TRACE("Acceleration [{}, {}, {}]", m_Acceleration.x, m_Acceleration.y, m_Acceleration.z);
			//NOVA_TRACE("Velocity [{}, {}, {}]", m_Velocity.x, m_Velocity.y, m_Velocity.z);
			//NOVA_TRACE("Position [{}, {}, {}]", m_Position.x, m_Position.y, m_Position.z);

			//Clear force every tick to prepare for the next one
			m_Force = {0, 0, 0};
		}

		double Body::CalculateMass()
		{
			return m_Mass;
		}

		void Ship::Update(const SimulationData& data)
		{
			for (auto& engine : m_Engines)
			{
				engine.Update(this, data);
			}

			//Must be called last since values are integrated and force is cleared at the end!
			Body::Update(data);
		}

		double Ship::CalculateMass()
		{
			double result = Body::CalculateMass();
			for (const auto& engine : m_Engines)
			{
				result += engine.Mass;
			}
			return result;
		}

		void Engine::Update(Ship* parent, const SimulationData& data)
		{
			double thrust = ForceCurve.Get(data.Time);
			double changeInImpulse = thrust * data.DT;
			ImpulseCounter += changeInImpulse;

			//In order to determine how much mass has burned and is no longer an the rocket we assume that the change in impulse compared to the total impulse
			//is proportional to the chang in mass burned
			double totalImpulseRatio = changeInImpulse / ExpectedImpulse;
			double massLoss = totalImpulseRatio * (WetMass - DryMass);
			Mass -= massLoss;

			//Newton's third law
			parent->ImpartForce(Direction * -thrust);

			//NOVA_TRACE("Thrust {}N, totalImpulseRatio {}, mass {}", thrust, totalImpulseRatio, m_Mass);
		}
bbbbbbbbbbbbbbbbbbbb bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbvvvvvvvc

	}
}

