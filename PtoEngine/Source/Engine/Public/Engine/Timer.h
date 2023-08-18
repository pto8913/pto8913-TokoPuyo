#pragma once

#include <chrono>
#include <map>
#include <string>

class WorldTimer
{
public:
	WorldTimer();
	
	WorldTimer(const WorldTimer&) = delete;
	WorldTimer& operator=(const WorldTimer&) = delete;

	float GetDelta() noexcept;
private:
	std::chrono::steady_clock::time_point last;
};

class InvalidTimer : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "Invalid Timer. Return value can not use.";
	}
};

/*
* Only one void funtion(void) in the one timer.
*/
template<typename T>
class Timer;

template<>
class Timer<void()>
{
public:
	Timer()
	{
		ClearTimer();

		mLastTime = std::chrono::system_clock::now();
	}

	void Tick()
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

	/* global function */
	template<auto tFunction, typename = typename std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<decltype(tFunction)>> && std::is_invocable_r_v<void, decltype(tFunction)>>>
	void SetTimer(float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		new (&mData) std::nullptr_t(nullptr);

		Initialize(inInitialDelay, inLoop, inDelay);

		mFunction = [](Storage)
		{
			return tFunction();
		};
	}

	/* member function */
	template<auto tFunction, typename Type, typename = typename std::enable_if_t<std::is_member_function_pointer_v<decltype(tFunction)>&& std::is_invocable_r_v<void, decltype(tFunction), Type>>>
	void SetTimer(Type& instance, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		new (&mData) Type* (&instance);

		Initialize(inInitialDelay, inLoop, inDelay);

		mFunction = [](Storage data)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(tFunction, *instance);
		};
	}

	/* lvalue lambda function */
	template<typename Type>
	void SetTimer(Type& funcPtr, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		new (&mData) Type* (&funcPtr);

		Initialize(inInitialDelay, inLoop, inDelay);

		mFunction = [](Storage data)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(*instance);
		};
	}

	/* rvalue lambda function */
	template<typename Type>
	void SetTimer(Type&& funcPtr, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		new (&mData) Type(std::move(funcPtr));

		Initialize(inInitialDelay, inLoop, inDelay);

		mFunction = [](Storage data)
		{
			Type* instance = *reinterpret_cast<Type*>(data);
			return std::invoke(*instance);
		};
	}

	void ClearTimer()
	{
		new (&mData) std::nullptr_t(nullptr);
		mFunction = nullptr;

		bIsLoop = false;
		bIsCompleteInitialLoop = false;
		mInitialDelay = 1.f;
		mDelay = 1.f;
	}

	explicit operator bool() const noexcept
	{
		return mFunction;
	}
private:
	void Initialize(float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		if (inInitialDelay == 0.f)
		{
			bIsCompleteInitialLoop = true;
		}
		bIsLoop = inLoop;
		mDelay = inDelay;
	}

	void Invoke()
	{
		if (!*this)
		{
			throw InvalidTimer();
		}
		mFunction(&mData);
	}

	bool bIsLoop = false;
	bool bIsCompleteInitialLoop = false;
	float mInitialDelay = 1.f;
	float mDelay = 1.f;

	std::chrono::system_clock::time_point mLastTime;

	using Storage = std::byte(*)[sizeof(void*)];
	using Function = void(*)(Storage);

	alignas(void*) std::byte mData[sizeof(void*)];
	Function mFunction;
};

struct FTimerHandle
{
private:
	friend class TimerManager;
	std::string tag;
public:
	FTimerHandle() = default;
	bool operator==(const FTimerHandle& other) const noexcept
	{
		return tag == other.tag;
	}
	bool operator==(FTimerHandle&& other) const noexcept
	{
		return tag == other.tag;
	}
	bool operator<(const FTimerHandle& other) const noexcept
	{
		return tag < other.tag;
	}
};

class TimerManager
{
public:
	TimerManager() = default;

	TimerManager(const TimerManager&) = delete;
	TimerManager& operator=(const TimerManager&) = delete;

	/* global function */
	template<auto tFunction, typename = typename std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<decltype(tFunction)>>&& std::is_invocable_r_v<void, decltype(tFunction)>>>
	FTimerHandle SetTimer(float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		Timer<void()> timer;
		timer.SetTimer<tFunction>(inInitialDelay, inLoop, inDelay);

		FTimerHandle handle;
		handle.tag = std::to_string(timers.size());
		timers.emplace(handle, timer);
		return handle;
	}

	/* member function */
	template<auto tFunction, typename Type, typename = typename std::enable_if_t<std::is_member_function_pointer_v<decltype(tFunction)>&& std::is_invocable_r_v<void, decltype(tFunction), Type>>>
	FTimerHandle SetTimer(Type& instance, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		Timer<void()> timer;
		timer.SetTimer<tFunction>(&instance, inInitialDelay, inLoop, inDelay);

		FTimerHandle handle;
		handle.tag = std::to_string(timers.size());
		timers.emplace(handle, timer);
		return handle;
	}

	/* lvalue lambda function */
	template<typename Type>
	FTimerHandle SetTimer(Type& funcPtr, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		Timer<void()> timer;
		timer.SetTimer(&funcPtr, inInitialDelay, inLoop, inDelay);

		FTimerHandle handle;
		handle.tag = std::to_string(timers.size());
		timers.emplace(handle, timer);
		return handle;
	}

	/* rvalue lambda function */
	template<typename Type>
	FTimerHandle SetTimer(Type&& funcPtr, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		Timer<void()> timer;
		timer.SetTimer(std::move(funcPtr), inInitialDelay, inLoop, inDelay);

		FTimerHandle handle;
		handle.tag = std::to_string(timers.size());
		timers.emplace(handle, timer);

		return handle;
	}

	void ClearTimer(FTimerHandle handle)
	{
		auto timer = timers.at(handle);
		if (timer)
		{
			timer.ClearTimer();
		}
		timers.erase(handle);
	}

	void Clear()
	{
		for (auto&& e : timers)
		{
			e.second.ClearTimer();
		}
		timers.empty();
	}
private:
	std::map<FTimerHandle, Timer<void()>> timers;
};
