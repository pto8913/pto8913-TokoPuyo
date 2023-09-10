
#include "Object/Object.h"

void Object::SetTickEnabled(bool inState) noexcept
{
	bEnableTick = inState;
}
bool Object::GetTickEnabled() const noexcept
{
	return bEnableTick;
}