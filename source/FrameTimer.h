#pragma once

#include <stdint.h>
#include <SDL_timer.h>
#include <vector>

struct Timestamp
{
	uint64_t e;
};

class FrameTimer
{
public:
	FrameTimer(float fps);
	
	FrameTimer() = delete;
	FrameTimer(FrameTimer&) = delete;

	static Timestamp Now();
	
	void Sleep(Timestamp frameBegin) const;
	void UpdateEstimatedRenderTime(Timestamp renderBegin);

	void PrintDebugStats() const;

private:
	static constexpr int renderTimesCount = 10;
	std::vector<float> renderTimes;
	int nextRenderTimeIndex;

	const float prefFrequencySeconds;
	const float fixedDeltaTime;
	const uint64_t fixedDeltaTimeTicks;

	float GetSecondsElapsed(Timestamp start, Timestamp end) const;
	float EstimatedRenderTime() const;
};