#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "ThrustCurve.h"

namespace erfseds_nova {
	namespace test_data {

		class Engine;
		struct SimulationData;

		//A body is anything with a mass and a position
		class Body
		{
		public:
			inline const glm::dvec3& GetPosition() const { return m_Position; }
			inline const glm::dvec3& GetVelocity() const { return m_Velocity; }
			inline const glm::dvec3& GetAccereration() const { return m_Acceleration; }
			inline double GetMass() const { return m_Mass; }
			inline const std::string& GetName() const { return m_Name; }

			inline double GetActivationTime(double simTime) const { return simTime - m_Delay; }

			inline void ImpartForce(const glm::dvec3& force) { m_Force += force; }

			virtual void Update(const SimulationData& data);//dt is timestep in seconds since the last update and simTime is the amount of seconds that have elapsed since the start of the simulation
			virtual double CalculateMass();

		private:
			glm::dvec3 m_Position, m_Velocity, m_Acceleration, m_Force;

			double m_Mass;//Mass now. Constant for regular rocket parts, changes as fuel is burned for engines
			double m_Delay;
			std::string m_Name;

			friend class Parser;
		};

		//A ship represents a rocket with a position and gravity that can be ignited
		class Ship : public Body
		{
		public:
			virtual double CalculateMass() override;

		private:
			std::vector<Engine> m_Engines;
			friend class Parser;
		};

		struct Engine
		{
			glm::dvec3 m_Direction;//What direction this engine points relative to the rocket
			glm::dvec3 m_Offset;

			double m_Mass;//Current mass - recalculated as fuel is burned
			double m_DryMass;
			double m_WetMass;

			ThrustCurve m_ThrustCurve;

			void Update(Ship* parent, const SimulationData& data);
			Engine(std::vector<KeyFrame<double, double>> curveData) : m_ThrustCurve(curveData) {}

			friend class Parser;
		};

	}
}

