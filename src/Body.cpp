#include "Body.h"
#include "Simulation.h"

namespace erfseds_nova {
	namespace test_data {
		
		void Body::Update(const SimulationData& data)
		{
			double totalMass = CalculateMass();
			if (m_Position.y >= 0.0)
			{
				//Apply the force of weight - W=mg for all objects above the ground
				ImpartForce(m_Mass * data.Gravity);
			}
			else
			{
				//Use this as a primitive form of collision detection for now
				m_Position.y = 0.0;
			}

			//We assume mass is constant during update cycles so F=ma -> a=F/m
			m_Acceleration = m_Force / m_Mass;

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

		double Ship::CalculateMass()
		{
			double result = Body::CalculateMass();
			for (const auto& engine : m_Engines)
			{
				result += engine.m_Mass;
			}
			return result;
		}


		void Engine::Update(Ship* parent, const SimulationData& data)
		{
			double thrust = m_ThrustCurve.Get(data.Time);
			
		}


	}
}

