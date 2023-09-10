
#include "Object/Object.h"

void Object::SetTickEnabled(bool inState) noexcept
{
	bEnableTick = inState;
}
bool Object::GetTickEnabled() const noexcept
{
	return bEnableTick;
}

bool Object::IsPendingKill() const noexcept
{
	return bIsPendingKill;
}
void Object::MarkPendingKill()
{
	bIsPendingKill = true;
}

const Layer::EActorLayer& Object::GetLayer() const
{
	return mLayer;
}
void Object::SetLayer(const Layer::EActorLayer& in)
{
	mLayer = in;
}

bool IsValid(std::shared_ptr<Object> in)
{
	if (in != nullptr)
	{
		return IsValid(in.get());
	}
	return false;
}
bool IsValid(Object* in)
{
	if (in != nullptr)
	{
		if (!in->IsPendingKill())
		{
			return true;
		}
	}
	return false;
}
