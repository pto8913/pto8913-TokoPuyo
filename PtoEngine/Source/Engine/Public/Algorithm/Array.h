#pragma once

#include <vector>
#include <algorithm>

#include "Algorithm/Random.h"

template<typename T>
class TArray
{
public:
	TArray() = default;
	TArray(int size, T initVal) noexcept
	{
		arr.assign(size, initVal);
	}

	T& operator[](int idx)
	{
		return arr[idx];
	}
	const T& operator[](int idx) const
	{
		return arr[idx];
	}

	std::vector<T>::iterator begin() noexcept
	{
		return arr.begin();
	}
	std::vector<T>::iterator end() noexcept
	{
		return arr.end();
	}

	void Init(int size, T initVal) noexcept
	{
		arr.assign(size, initVal);
	}
	template<typename Iter>
	void Init(Iter itrB, Iter itrE) noexcept
	{
		arr.assign(itrB, itrE);
	}
	bool IsValidIndex(int Idx) const noexcept
	{
		return arr.size() > Idx;
	}
	void Add(T in) noexcept
	{
		arr.push_back(in);
	}

	void Remove(int Idx) noexcept
	{
		arr.erase(arr.begin() + Idx);
	}
	void Clear() noexcept
	{
		arr.erase(arr.begin(), arr.end());
		arr.clear();
	}
	size_t Size() const noexcept
	{
		return arr.size();
	}

	T& Last() noexcept
	{
		return arr.back();
	}

	template<typename Compare>
	void Sort()
	{
		std::sort(arr.begin(), arr.end(), Compare);
	}
	void Sort()
	{
		std::sort(arr.begin(), arr.end());
	}

	T RandomValue() const noexcept
	{
		if (Size() == 0)
		{
			return T();
		}
		return arr[Algo::RandRange(0, int(Size() - 1))];
	}

private:
	std::vector<T> arr;
};