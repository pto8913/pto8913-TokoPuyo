#pragma once

#include <xutility>

namespace Algo
{
	template<typename T>
	void swap(T& a, T& b)
	{
		T temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	// C++ 20
	template<typename T>
	class has_ToString
	{
	public:
		enum {
			value = requires(const T & t) { t.ToString(); }
		};
	};
}