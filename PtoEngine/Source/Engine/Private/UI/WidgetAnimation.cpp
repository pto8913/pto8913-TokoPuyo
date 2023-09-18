
#include "UI/WidgetAnimation.h"

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

// -------------------------------------------------------
// Main
// -------------------------------------------------------
void WidgetAnimation::SetStartTime(const float& in)
{
	if (!bIsActive)
	{
		mStartTime = in;
		mDuration = in;
	}
}
void WidgetAnimation::SetPlaySpeed(const float& in)
{
	if (!bIsActive)
	{
		mPlaySpeed = in;
	}
}

void WidgetAnimation::Clear()
{
	bIsActive = false;

	OnWidgetAnimationCompleted.ClearBind();

	mStartTime = 0.f;
	mEndTime = 1.f;
	mPlaySpeed = 1.f;
	mDuration = 0.f;

	props.Clear();
}

void WidgetAnimation::AssignProp(FWidgetAnimationProperty* prop)
{
	props.Add(prop);
}

void WidgetAnimation::Activate()
{
	bIsActive = true;
}
void WidgetAnimation::Deactivate()
{
	bIsActive = false;
}

void WidgetAnimation::Update(float deltaTime)
{
	if (bIsActive)
	{
		mDuration += deltaTime * mPlaySpeed;
		for (auto&& prop : props)
		{
			prop->Udpate(mDuration);
		}

		if (Algo::IsNearlyEqual(mDuration, mEndTime, 0.1f))
		{
			if (OnWidgetAnimationCompleted.IsBound())
			{
				OnWidgetAnimationCompleted.Broadcast();
			}
			Deactivate();
		}
	}
}
void WidgetAnimation::Reset()
{
	mDuration = 0.f;
}
