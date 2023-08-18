
#include "Engine/Timer.h"

using namespace std::chrono;

WorldTimer::WorldTimer()
{
	last = steady_clock::now();
}

float WorldTimer::GetDelta() noexcept
{
	const auto p = std::move(last);
	last = steady_clock::now();
	duration frame = last - p;
	//milliseconds out = duration_cast<milliseconds>(frame);

	duration<float> out = last - p;
	return out.count();
}
