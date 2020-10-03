#pragma once

#include <vector>
#include <exception>

#include "Log.h"


namespace erfseds_nova {
	namespace test_data {
		
		//Interpolation related math functions
		template<typename T>
		static T lerp(const T& a, const T& b, const T& f)
		{
			//Linear interpolation
			return a + f * (b - a);
		}

		template<typename T>
		static T normalize(const T& min, const T& max, const T& value)
		{
			return (value - min) / (max - min);
		}

		template<typename T>
		static T map(const T& value, const T& sourceMin, const T& sourceMax, const T& destMin, const T& destMax)
		{
			//Map a value from one range of values to another using linear interpolation
			T n = normalize(sourceMin, sourceMax, value);
			return lerp(destMin, destMax, n);
		}

		template<typename T, typename TimeType>
		struct KeyFrame
		{
			TimeType Time;
			T Value;
		};

		template<typename T, typename TimeType = double>
		class KeyFrameManager
		{
		public:
			KeyFrameManager(std::vector<KeyFrame<T, TimeType>> data) : m_Data(data)
			{
				if (m_Data.size() == 0)
				{
					throw "KeyFrameManager must have at least one key frame to manage!";
				}
				for (int i = 1; i < data.size(); i++)
				{
					//All time values must be increasing+
					if (data[i].Time < data[i - 1].Time)
					{
						NOVA_ERROR("Timestamps at index {} and {} are not sorted! Expected second value {} to be larger than {}", i - 1, i, data[i].Time, data[i - 1].Time);
						throw "KeyFrameManager must have all keyframes sorted from lowest to highest chronologically";
					}
				}
			}

			T Get(TimeType time)
			{
				//NOVA_TRACE("Getting value for time {}", time);

				//Check the extremes and clamp the out of range values to the first or last one
				if (time <= m_Data[0].Time)
				{
					//NOVA_TRACE("Aborting early since time is less than the first keyframe value: {}", time);
					return m_Data[0].Value;
				}
				if (time >= m_Data[m_Data.size() - 1].Time)
				{
					//NOVA_TRACE("Aborting early since time is greater than the last keyframe value: {}", time);
					return m_Data[m_Data.size() - 1].Value;
				}

				//We need to interpolate between two values
				//So find the pair of keyframes that surround the time passed in
				TimeType lastTime = m_Data[0].Time;
				for (int i = 1; i < m_Data.size(); i++)
				{
					TimeType nowTime = m_Data[i].Time;
					if (lastTime <= time && nowTime >= time)
					{
						const T& a = m_Data[i - 1].Value;
						const T& b = m_Data[i].Value;
						//NOVA_TRACE("Using a and b = [{}, {}]", a, b);

						//Interpolate between the two values based on time
						return map(time, lastTime, nowTime, a, b);
					}
					lastTime = nowTime;
				}
				NOVA_CRITICAL("Failed to find matching time points! This should never happen: https://xkcd.com/2200");
				exit(69);
			}

			inline std::size_t DataPoints() const { return m_Data.size(); }

		private:
			//Sorted from smallest to largest time type
			std::vector<KeyFrame<T, TimeType>> m_Data;
		};

		using ThrustCurve = KeyFrameManager<double>;

	}
}

