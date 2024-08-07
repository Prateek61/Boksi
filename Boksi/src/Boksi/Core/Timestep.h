#pragma once
#include "bkpch.h"

namespace Boksi
{
	class TimeStep
	{
	public:
		TimeStep(float time_seconds = 0.0f)
			: m_Time(time_seconds)
		{
		}

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

	public:
		// Operator overloads
		// Add
		TimeStep operator+(const TimeStep& other) const
		{
			return TimeStep(m_Time + other.m_Time);
		}

		TimeStep& operator+=(const TimeStep& other)
		{
			m_Time += other.m_Time;
			return *this;
		}

		// Subtract
		TimeStep operator-(const TimeStep& other) const
		{
			return TimeStep(m_Time - other.m_Time);
		}

		TimeStep& operator-=(const TimeStep& other)
		{
			m_Time -= other.m_Time;
			return *this;
		}

	private:
		float m_Time;
	};
}