
#include "Algorithm/Random.h"

namespace Algo
{
	int RandRange(int inMin, int inMax)
	{
		if (inMin > inMax)
		{
			assert(false);
		}
		if (inMin == inMax)
		{
			return inMin;
		}
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distr = std::uniform_int_distribution<int>(inMin, inMax);
		return static_cast<int>(distr(gen));
	}

	/* random 0 ~ inMax */
	int Random(int inMax)
	{
		return RandRange(0, inMax);
	}
}