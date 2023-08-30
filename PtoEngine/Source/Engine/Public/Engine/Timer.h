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

// ------------------------------------------------------------------------------------------------------------
// Timer
// ------------------------------------------------------------------------------------------------------------
struct FTimerDelegate
{
public:
	FTimerDelegate();
	void Tick();

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

	void ClearTimer();
	explicit operator bool() const noexcept;
	bool IsBound() const noexcept;
private:
	void Initialize(float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f);
	void Invoke();

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


// ------------------------------------------------------------------------------------------------------------
// TimerHandle
// ------------------------------------------------------------------------------------------------------------
struct FTimerHandle
{
private:
	friend class TimerManager;
	std::string tag;
public:
	FTimerHandle();
	FTimerHandle(const std::string& in);
	FTimerHandle(std::string&& in);
	~FTimerHandle();
	explicit operator bool() const noexcept;
	bool operator==(const FTimerHandle& other) const noexcept;
	bool operator==(FTimerHandle&& other) const noexcept;
	bool operator<(const FTimerHandle& other) const noexcept;

	void Clear();
};

// ------------------------------------------------------------------------------------------------------------
// TimerManager
// ------------------------------------------------------------------------------------------------------------
class TimerManager
{
public:
	TimerManager() = default;

	TimerManager(const TimerManager&) = delete;
	TimerManager& operator=(const TimerManager&) = delete;

	explicit operator bool() const noexcept;

	/* global function */
	template<auto tFunction, typename = typename std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<decltype(tFunction)>>&& std::is_invocable_r_v<void, decltype(tFunction)>>>
	FTimerHandle SetTimer(float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		FTimerDelegate timer;
		timer.SetTimer<tFunction>(inInitialDelay, inLoop, inDelay);

		FTimerHandle handle(GetTag());
		timers.emplace(handle, timer);
		return handle;
	}

	/* member function */
	template<auto tFunction, typename Type, typename = typename std::enable_if_t<std::is_member_function_pointer_v<decltype(tFunction)>&& std::is_invocable_r_v<void, decltype(tFunction), Type>>>
	FTimerHandle SetTimer(Type& instance, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		FTimerDelegate timer;
		timer.SetTimer<tFunction>(instance, inInitialDelay, inLoop, inDelay);

		FTimerHandle handle(GetTag());
		timers.emplace(handle, timer);
		return handle;
	}

	/* lvalue lambda function */
	template<typename Type>
	FTimerHandle SetTimer(Type& funcPtr, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		FTimerDelegate timer;
		timer.SetTimer(&funcPtr, inInitialDelay, inLoop, inDelay);

		FTimerHandle handle(GetTag());
		timers.emplace(handle, timer);
		return handle;
	}

	/* rvalue lambda function */
	template<typename Type>
	FTimerHandle SetTimer(Type&& funcPtr, float inInitialDelay = 0.f, bool inLoop = false, float inDelay = 1.f)
	{
		FTimerDelegate timer;
		timer.SetTimer(std::move(funcPtr), inInitialDelay, inLoop, inDelay);

		FTimerHandle handle(GetTag());
		timers.emplace(handle, timer);

		return handle;
	}

	FTimerHandle SetTimer(FTimerDelegate in)
	{
		FTimerHandle handle(GetTag());
		timers.emplace(handle, in);
		return handle;
	}

	void ClearTimer(FTimerHandle& handle);
	void Clear();
	void Tick();

private:
	std::string GetTag();

	std::map<FTimerHandle, FTimerDelegate> timers;
	bool bEnableTick = true;
};
