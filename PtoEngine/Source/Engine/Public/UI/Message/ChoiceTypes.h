#pragma once

#include <string>

struct FChoiceInfos
{
public:
	FChoiceInfos() = default;
	FChoiceInfos(std::wstring&& inLabel, std::wstring&& inTag)
		: buttonLabel(std::move(inLabel)), tag(std::move(inTag))
	{}

	bool operator==(const FChoiceInfos& other) const
	{
		return buttonLabel == other.buttonLabel && tag == other.tag;
	}

	std::wstring buttonLabel;
	std::wstring tag;
};