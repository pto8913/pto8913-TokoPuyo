
#include "Object/Object.h"

Object::Object()
{
}

Object::~Object()
{
}

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
	mName = GetName();
}

void Object::SetID(int inID)
{
	mID = inID;
}
int Object::GetID() const
{
	return mID;
}

void Object::AddTag(const std::wstring& inTag)
{
	mTags.emplace(inTag);
}
void Object::RemoveTag(const std::wstring& inTag)
{
	mTags.erase(inTag);
}
bool Object::HasTag(const std::wstring& inTag) const
{
	return mTags.contains(inTag);
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
