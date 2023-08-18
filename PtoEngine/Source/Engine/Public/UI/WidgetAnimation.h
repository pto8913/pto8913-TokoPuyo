#pragma once

#include "Algorithm/Array.h"
#include "Algorithm/Math2.h"

// ------------------------------------------------------------------------------------------------------------
// Widget Animation Property
// ------------------------------------------------------------------------------------------------------------
class WidgetAnimationProperty
{
public:
	virtual ~WidgetAnimationProperty();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	template<class Type, typename T>
	void Assign(Type& instance, T Type::* member, T start, T end)
	{
		new (&mData) Type* (&instance);
		new (&mStart) T(start);
		new (&mEnd) T(end);

		offset = GetOffset(member);

		mFunction = +[](Storage data, std::ptrdiff_t offset, Storage start, Storage end, float alpha)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			T* ptr = (T*)((unsigned long long) & (*instance) + offset);

			T s = *reinterpret_cast<T*>(start);
			T e = *reinterpret_cast<T*>(end);
			*ptr = Algo::lerp(s, e, alpha);
		};
	}

	void Udpate(float alpha/* 0.f ~ 1.f */);
private:
	template<class Type, typename T>
	std::ptrdiff_t GetOffset(T Type::* member)
	{
		return reinterpret_cast<std::ptrdiff_t>(
			&(reinterpret_cast<Type const volatile*>(0)->*member)
		);
	}


	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	using Storage = std::byte(*)[sizeof(void*)];
	using Function = void(*)(Storage, std::ptrdiff_t, Storage, Storage, float);

	alignas(void*) std::byte mData[sizeof(void*)];
	alignas(void*) std::byte mStart[sizeof(void*)];
	alignas(void*) std::byte mEnd[sizeof(void*)];

	Function mFunction;
	std::ptrdiff_t offset;
};

// ------------------------------------------------------------------------------------------------------------
// Widget Animation
// ------------------------------------------------------------------------------------------------------------
/*
* NOTE : Must be call Accept If you complete assign properties.
*
* @inStartTime : 0.f ~ 1.f
* @Callback : Will be call when the duration time reach to 1.f;
*
*/
class WidgetAnimation
{
public:
	WidgetAnimation(float inStartTime = 0.f, float inPlaySpeed = 1.f);
	virtual ~WidgetAnimation();

	/* global function */
	template<auto Callback, typename = typename std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<decltype(Callback)>>&& std::is_invocable_r_v<void, decltype(Callback)>>>
	void SetCallback()
	{
		new (&mData) std::nullptr_t(nullptr);

		mFunction = [](Storage)
		{
			return Callback();
		};
	}

	/* member function */
	template<auto Callback, typename Type, typename = typename std::enable_if_t<std::is_member_function_pointer_v<decltype(Callback)>&& std::is_invocable_r_v<void, decltype(Callback), Type>>>
	void SetCallback(Type& instance)
	{
		new (&mData) Type* (&instance);

		mFunction = [](Storage data)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(Callback, *instance);
		};
	}

	/* lvalue lambda function */
	template<typename Type>
	void SetCallback(Type& funcPtr)
	{
		new (&mData) Type* (&funcPtr);

		mFunction = [](Storage data)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(*instance);
		};
	}

	/* rvalue lambda function */
	template<typename Type>
	void SetCallback(Type&& funcPtr)
	{
		new (&mData) Type(std::move(funcPtr));

		mFunction = [](Storage data)
		{
			Type* instance = *reinterpret_cast<Type*>(data);
			return std::invoke(*instance);
		};
	}

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	void Clear();

	void AssignProp(WidgetAnimationProperty prop);

	void Accept();

	void Update(float deltaTime);

private:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	using Storage = std::byte(*)[sizeof(void*)];
	using Function = void(*)(Storage);

	bool bIsInitialized = false;

	alignas(void*) std::byte mData[sizeof(void*)];
	Function mFunction;

	float mStartTime = 0.f;
	float mEndTime = 1.f;
	float mPlaySpeed = 1.f;
	float mDuration = 0.f;

	TArray<WidgetAnimationProperty> props;
};