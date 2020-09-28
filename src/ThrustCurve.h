#pragma once

#include <vector>

namespace erfseds_nova {
	namespace test_data {

		template<typename T, typename TimeType>
		struct KeyFrame
		{
			TimeType Time;
			T Value;
		};

		template<typename T, typename TimeType = double>
		class KeyFrameManager
		{
			std::vector<KeyFrame<T, TimeType>> m_Data;
		};

	}
}

