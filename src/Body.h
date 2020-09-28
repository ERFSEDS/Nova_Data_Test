#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace erfseds_nova {
	namespace test_data {

		class Engine;

		class Body {
		public:
			inline const glm::dvec3& GetPosition() { return m_Position; }
			inline const glm::dvec3& GetVelocity() { return m_Velocity; }
			inline const glm::dvec3& GetAccereration() { return m_Acceleration; }
			inline double GetMass() { return m_Mass; }


		private:
			glm::dvec3 m_Position, m_Velocity, m_Acceleration;
			double m_Mass;
			double m_Delay;
			std::string m_Name;
			std::vector<Engine> m_Engines;
		};

		class Engine : public Body
		{
			glm::dvec3 m_Direction;
			
		};

	}

}

