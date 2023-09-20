#pragma once

#include "Algorithm/Math2.h"
#include "Algorithm/algo.h"
#include "Math/Math.h"

#include "Engine/Delegate.h"

#include "WidgetAnimationTypes.h"

#include <format>
#include <vector>

// ------------------------------------------------------------------------------------------------------------
// Widget Animation Property
// ------------------------------------------------------------------------------------------------------------
struct FWidgetAnimationProperty
{
public:
	FWidgetAnimationProperty() = default;
	virtual ~FWidgetAnimationProperty() = default;

	virtual void Udpate(float alpha/* 0.f ~ 1.f */) = 0;
};

template<typename T>
class WidgetAnimationProperty : public FWidgetAnimationProperty
{
public:
	WidgetAnimationProperty()
	{
		new (&mData) std::nullptr_t(nullptr);
		new (&mStart) T();
		new (&mEnd) T();
		mFunction = nullptr;
		offset = 0;
	}
	virtual ~WidgetAnimationProperty()
	{
		Clear();
	}
	void Clear()
	{
		new (&mData) std::nullptr_t(nullptr);
		new (&mStart) std::nullptr_t(nullptr);
		new (&mEnd) std::nullptr_t(nullptr);
		mFunction = nullptr;

		offset = 0;
	}
	
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	/* 
	* NOTE : T object need to these operator and function.
	* 
	* //////////////
	* //// Must ////
	* T operator-(const T& other) const;
	* bool operator(float) const;
	* T Abs()
	* 
	* T operator*(const float& other) const;
	* T operator+(const T& other) const;
	* //// Must ////
	* //////////////
	*
	* //// for debug
	* std::string ToString() const;
	* 
	*/
	template<class Type>
	void Assign(Type& instance, T Type::* member, T start, T end)
	{
		new (&mData) Type* (&instance);
		new (&mStart) T(start);
		new (&mEnd) T(end);
		
#if _DEBUG
		if (Algo::has_ToString<T>::value)
		{
			OutputDebugStringA(std::format("start {}", start.ToString()).c_str());
			OutputDebugStringA(std::format("end   {}", end.ToString()).c_str());
		}
#endif
		offset = GetOffset(member);

		mFunction = +[](StoragePtr data, std::ptrdiff_t offset, StorageT start, StorageT end, float alpha)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			T* ptr = (T*)((unsigned long long)&(*instance) + offset);

			T s = *reinterpret_cast<T*>(start);
			T e = *reinterpret_cast<T*>(end);
			*ptr = Algo::lerp(s, e, Math::Clamp(alpha, 0.f, 1.f));
#if _DEBUG
			if (Algo::has_ToString<T>::value)
			{
				OutputDebugStringA(std::format("c {}", (*ptr).ToString()).c_str());
			}
#endif
		};
	}

	virtual void Udpate(float alpha/* 0.f ~ 1.f */) override
	{
		mFunction(&mData, offset, &mStart, &mEnd, alpha);
	}
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
	using StoragePtr = std::byte(*)[sizeof(void*)];
	using StorageT = std::byte(*)[sizeof(T)];
	using Function = void(*)(StoragePtr, std::ptrdiff_t, StorageT, StorageT, float);

	alignas(void*) std::byte mData[sizeof(void*)];
	alignas(T) std::byte mStart[sizeof(T)];
	alignas(T) std::byte mEnd[sizeof(T)];

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
	WidgetAnimation();
	virtual ~WidgetAnimation();

	FOnWidgetAnimationCompleted OnWidgetAnimationCompleted;

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	void SetStartTime(const float& in);
	void SetPlaySpeed(const float& in);

	void Clear();

	void AssignProp(FWidgetAnimationProperty* prop);

	bool IsActive() const;
	void Activate();
	void Deactivate();

	void PlayForward(const float& inStartTime = 0.f, const float inPlaySpeed = 1.f);
	void PlayReverse(const float& inStartTime = 0.f, const float inPlaySpeed = 1.f);

	void Update(float deltaTime);
	void Reset();
private:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	bool bIsActive = false;

	float mStartTime = 0.f;
	float mEndTime = 1.f;
	float mPlaySpeed = 1.f;
	float mDuration = 0.f;

	std::vector<FWidgetAnimationProperty*> props;
};