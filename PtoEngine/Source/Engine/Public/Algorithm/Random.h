#pragma once

#include "DirectX/DirectXHead.h"
#include <random>

namespace Algo
{
	int RandRange(int inMin, int inMax);

	/* random 0 ~ inMax */
	int Random(int inMax);

	/* weight 0 ~ 1 */
	bool RandomBool(float weight = 0.5f);
}