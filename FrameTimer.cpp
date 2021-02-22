
#include <math.h> // for floor
#include <numeric> // for accumulate
#include <iostream> // For printing debug info
#include <string> // for std::to_string

#include "FrameTimer.h"


FrameTimer::FrameTimer(float fps) :
	prefFrequencySeconds((float)SDL_GetPerformanceFrequency()),
	fixedDeltaTime(1000.0f / fps),
	fixedDeltaTimeTicks(Now().e / fps)
{
	renderTimes = std::vector<float>(renderTimesCount, 0.0f);
}

float FrameTimer::GetSecondsElapsed(Timestamp start, Timestamp end = Now()) const
{
	return (float)(end.e - start.e) / prefFrequencySeconds;
}

float FrameTimer::EstimatedRenderTime() const
{
	return (std::accumulate(renderTimes.begin(), renderTimes.end(), 0.0f) / renderTimes.size());
}

void FrameTimer::PrintDebugStats() const
{
	static Timestamp lastPrint = Now();
	static int framesElapsed = 0;
	
	if(GetSecondsElapsed(lastPrint) > 1.0f)
	{
		Timestamp now = Now();
		float debugDisplayTime = GetSecondsElapsed(lastPrint, now);
		std::cout << "Average Framerate:" << framesElapsed << ". Average Render Time: " << std::to_string(EstimatedRenderTime()) << "." << std::endl;
		
		lastPrint = now;
		framesElapsed = 0;
	}
	else
	{
		framesElapsed++;
	}
}

Timestamp FrameTimer::Now()
{
	return { SDL_GetPerformanceCounter() };
}

void FrameTimer::Sleep(Timestamp frameBegin) const
{
	float frameTime = GetSecondsElapsed(frameBegin);
	float renderTime = EstimatedRenderTime();

	int32_t sleepTime = floor((int32_t)(fixedDeltaTime - renderTime - frameTime)) - 1;
	if (sleepTime > 1)
	{
		SDL_Delay(sleepTime);
	}

	//spin for the rest
	while (GetSecondsElapsed(frameBegin) < fixedDeltaTime - renderTime);
}

void FrameTimer::UpdateEstimatedRenderTime(Timestamp renderBegin)
{
	nextRenderTimeIndex += 1;
	nextRenderTimeIndex %= renderTimes.size();

	renderTimes.at(nextRenderTimeIndex) = GetSecondsElapsed(renderBegin);
}