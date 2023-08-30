#pragma once

enum class EActor2DLayer
{
	Default,
	Background,
	Entities,
	Foreground,
	UI,
};

namespace Layer
{
	enum EOrder
	{
		Ground,
		Event,
		Item,
		Building,
		Character,
		Effect,
		UnOrder,
	};
}