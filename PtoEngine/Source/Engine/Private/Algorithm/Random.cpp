
#include "Algorithm/Random.h"
#include "Algorithm/Math.h"

std::random_device rd;
std::mt19937 gen(rd());

namespace Algo
{
	int RandRange(int inMin, int inMax)
	{
		assert(inMin <= inMax);
		if (inMin == inMax)
		{
			return inMin;
		}
		std::uniform_int_distribution<int> distr = std::uniform_int_distribution<int>(inMin, inMax);
		return static_cast<int>(distr(gen));
	}

	/* random 0 ~ inMax */
	int Random(int inMax)
	{
		return RandRange(0, inMax);
	}

	bool RandomBool(float weight)
	{
		weight = Math::MapRange(weight, 0.f, 1.f, 0.f, 100.f);
		return RandRange(0, 100) < (int)weight;
	}
}