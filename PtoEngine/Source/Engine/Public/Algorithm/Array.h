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

	std::vector<T>::iterator begin() noexcept
	{
		return arr.begin();
	}
	std::vector<T>::iterator end() noexcept
	{
		return arr.end();
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
		return arr[Algo::RandRange(0, int(Size() - 1))];
	}
	
	T& operator[](int Idx)
	{
		return arr[Idx];
	}
private:
	std::vector<T> arr;
};