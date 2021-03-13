#pragma once

class Time
{
public:
	Time() : deltaTime(0), time(0) {};

	void Update(float deltaTime)
	{
		this->deltaTime = deltaTime;
		this->time += deltaTime;
	}

	const float& Now() const { return time; };
	const float& DeltaTime() { return deltaTime; }

private:
	float deltaTime;
	float time;
};