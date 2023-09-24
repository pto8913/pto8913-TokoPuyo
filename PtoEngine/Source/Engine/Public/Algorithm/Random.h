#pragma once

#include "Graphics/DirectXHead.h"
#include <random>

namespace Algo
{
	int RandRange(int inMin, int inMax);
	template<typename T>
	int RandRange(T inMin, T inMax)
	{
		return RandRange(static_cast<int>(inMin), static_cast<int>(inMax));
	}

	/* random 0 ~ inMax */
	int Random(int inMax);

	/* weight 0 ~ 1 */
	bool RandomBool(float weight = 0.5f);
}