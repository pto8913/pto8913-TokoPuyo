
#include "UI/WidgetAnimation.h"

// ------------------------------------------------------------------------------------------------------------
// Widget Animation
// ------------------------------------------------------------------------------------------------------------
WidgetAnimation::WidgetAnimation()
{
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

	auto iter = props.begin();
	while (iter != props.end())
	{
		auto& obj = *iter;
		if (obj)
		{
			delete obj;
			obj = nullptr;
			iter = props.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	props.clear();
}

void WidgetAnimation::AssignProp(FWidgetAnimationProperty* prop)
{
	props.push_back(prop);
}

bool WidgetAnimation::IsActive() const
{
	return bIsActive;
}
void WidgetAnimation::Activate()
{
	bIsActive = true;
}
void WidgetAnimation::Deactivate()
{
	bIsActive = false;
}


void WidgetAnimation::PlayForward(const float& inStartTime, const float inPlaySpeed)
{
	Deactivate();
	SetStartTime(inStartTime);
	SetPlaySpeed(inPlaySpeed);
	Activate();
}
void WidgetAnimation::PlayReverse(const float& inStartTime, const float inPlaySpeed)
{

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
