
#include "UI/WidgetAnimation.h"

#include <cmath>


// ------------------------------------------------------------------------------------------------------------
// Widget Animation Property
// ------------------------------------------------------------------------------------------------------------
WidgetAnimationProperty::~WidgetAnimationProperty()
{
	new (&mData) std::nullptr_t(nullptr);
	new (&mStart) std::nullptr_t(nullptr);
	new (&mEnd) std::nullptr_t(nullptr);
	mFunction = nullptr;
}
void WidgetAnimationProperty::Udpate(float alpha/* 0.f ~ 1.f */)
{
	mFunction(&mData, offset, &mStart, &mEnd, alpha);
}

// ------------------------------------------------------------------------------------------------------------
// Widget Animation
// ------------------------------------------------------------------------------------------------------------
WidgetAnimation::WidgetAnimation(float inStartTime, float inPlaySpeed)
{
	mStartTime = inStartTime;
	mEndTime = 1.f;
	mPlaySpeed = inPlaySpeed;

	mDuration = mStartTime;
};
WidgetAnimation::~WidgetAnimation()
{
	Clear();
}

void WidgetAnimation::AssignProp(WidgetAnimationProperty prop)
{
	props.Add(prop);
}

void WidgetAnimation::Accept()
{
	bIsInitialized = true;
}

void WidgetAnimation::Update(float deltaTime)
{
	if (bIsInitialized)
	{
		mDuration += deltaTime * mPlaySpeed;
		for (auto&& prop : props)
		{
			prop.Udpate(mDuration);
		}

		if (mDuration == mEndTime)
		{
			if (mFunction != nullptr)
			{
				mFunction(&mData);
			}
		}
	}
}
void WidgetAnimation::Clear()
{
	new (&mData) std::nullptr_t(nullptr);
	mFunction = nullptr;

	mStartTime = 0.f;
	mEndTime = 1.f;
	mPlaySpeed = 1.f;
	mDuration = 0.f;

	props.Clear();
}