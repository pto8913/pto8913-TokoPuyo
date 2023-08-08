#pragma once

namespace Algo
{
	template<typename T>
	void swap(T& a, T& b)
	{
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}
}