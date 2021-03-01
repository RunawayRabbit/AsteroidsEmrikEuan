#pragma once

#include <cmath> // for fmod
#include <algorithm> // for clamp
#include <random> // for mersenne twister

namespace Math
{
	static constexpr float TAU = 6.28318530717958647692f;
	static constexpr float PI = 3.14159265358979323846f;
	static constexpr float DEG2RAD = 0.01745329252f;
	static constexpr float RAD2DEG = 57.29577951f;

	// Angle Deltas

	static float AngleDelta(float a, float b)
	{
		return fmod((b - a + PI), TAU) - PI;
	}
	static float AngleDeltaDeg(float a, float b)
	{
		return fmod((b - a + 180.0f), 360.0f) - 180.0f;
	}

	// Angle Lerps

	static float LerpAngle(float a, float b, float t)
	{
		float delta = AngleDelta(a, b);
		if (delta > PI) delta -= TAU;

		return a + delta * t;
	}
	static float LerpAngleDeg(float a, float b, float t)
	{
		float delta = AngleDeltaDeg(a, b);
		if (delta > 180.0f) delta -= 360.0f;

		return a + delta * t;
	}

	// Clamped Angle Lerps

	static float LerpAngleClamped(float a, float b, float t)
	{
		float delta = AngleDelta(a, b);
		if (delta > PI) delta -= TAU;

		return a + delta * std::clamp(t, 0.0f, 1.0f);
	}
	static float LerpAngleDegClamped(float a, float b, float t)
	{
		float delta = AngleDeltaDeg(a, b);
		if (delta > 180.0f) delta -= 360.0f;

		return a + delta * std::clamp(t, 0.0f, 1.0f);
	}

	// RNG

	static float RandomRange(float min, float max)
	{
		static std::random_device device;
		static std::mt19937 generator(device());
		std::uniform_real<float> range(min, max);
		return range(generator);
	}

	// Modulo

	static float Repeat(float t, float length)
	{
		return std::clamp(t - floor(t / length) * length, 0.0f, length);
	}
}