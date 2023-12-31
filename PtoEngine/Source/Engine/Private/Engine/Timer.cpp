
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

// ------------------------------------------------------------------------------------------------------------
// Timer
// ------------------------------------------------------------------------------------------------------------
FTimerDelegate::FTimerDelegate()
{
	ClearTimer();

	mLastTime = std::chrono::system_clock::now();
}
void FTimerDelegate::Tick()
{
	if (*this)
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::chrono::system_clock::duration duration = now - mLastTime;
		std::chrono::milliseconds durationSec = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		if (bIsCompleteInitialLoop)
		{
			if (durationSec.count() / 1000.f > mDelay)
			{
				if (bIsLoop)
				{
					// call function
					Invoke();
					mLastTime = now;
				}
				else
				{
					// call function
					Invoke();
					ClearTimer();
				}
			}
		}
		else
		{
			if (durationSec.count() / 1000.f > mInitialDelay)
			{
				bIsCompleteInitialLoop = true;
				mLastTime = now;
			}
		}
	}
}
void FTimerDelegate::ClearTimer()
{
	new (&mData) std::nullptr_t(nullptr);
	mFunction = nullptr;

	bIsLoop = false;
	bIsCompleteInitialLoop = false;
	mInitialDelay = 1.f;
	mDelay = 1.f;
}

FTimerDelegate::operator bool() const noexcept
{
	return mFunction;
}
bool FTimerDelegate::IsBound() const noexcept
{
	return mFunction;
}

void FTimerDelegate::Initialize(float inInitialDelay, bool inLoop, float inDelay)
{
	if (inInitialDelay == 0.f)
	{
		bIsCompleteInitialLoop = true;
	}
	bIsLoop = inLoop;
	mDelay = inDelay;
}

void FTimerDelegate::Invoke()
{
	if (!*this)
	{
		throw InvalidTimer();
	}
	mFunction(&mData);
}

// ------------------------------------------------------------------------------------------------------------
// TimerHandle
// ------------------------------------------------------------------------------------------------------------
FTimerHandle::FTimerHandle()
	: tag("")
{
}
FTimerHandle::FTimerHandle(const std::string& in)
	: tag(in)
{
}
FTimerHandle::FTimerHandle(std::string&& in)
	: tag(std::move(in))
{

}
FTimerHandle::~FTimerHandle()
{
	tag = "";
}
FTimerHandle::operator bool() const noexcept
{
	return tag != "";
}
bool FTimerHandle::operator==(const FTimerHandle& other) const noexcept
{
	return tag == other.tag;
}
bool FTimerHandle::operator==(FTimerHandle&& other) const noexcept
{
	return tag == other.tag;
}
bool FTimerHandle::operator<(const FTimerHandle& other) const noexcept
{
	return tag < other.tag;
}
void FTimerHandle::Clear()
{
	tag = "";
}
// ------------------------------------------------------------------------------------------------------------
// TimerManager
// ------------------------------------------------------------------------------------------------------------
TimerManager::operator bool() const noexcept
{
	return (timers.size() > 0 || nextTickTimers.size()) && bEnableTick;
	//return timers.size() > 0 && bEnableTick;
}
void TimerManager::ClearTimer(FTimerHandle& handle)
{
	bEnableTick = false;
	auto timer = timers.at(handle);
	if (timer)
	{
		timer.ClearTimer();
	}
	timers.erase(handle);
	handle.Clear();
	bEnableTick = true;
}

void TimerManager::Clear()
{
	bEnableTick = false;
	for (auto&& e : timers)
	{
		e.second.ClearTimer();
	}
	timers.empty();
	bEnableTick = true;
}

void TimerManager::Tick()
{
	if (*this)
	{
		for (auto&& timer : timers)
		{
			if (timer.first)
			{
				timer.second.Tick();
			}
		}
		auto iter = nextTickTimers.begin();
		while (iter != nextTickTimers.end())
		{
			auto& timer = *iter;
			if (timer.first)
			{
				if (!timer.second.nextTick)
				{
					timer.second.nextTick = true;
					++iter;
				}
				else
				{
					timer.second.Invoke();
					iter = nextTickTimers.erase(iter);
				}
			}
			else
			{
				++iter;
			}
		}
	}
}

std::string TimerManager::GetTag()
{
	return std::to_string(timers.size());
}