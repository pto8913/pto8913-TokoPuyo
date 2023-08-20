#pragma once

#include "Engine/Vector.h"
#include <string>

class AppSettings
{
public:
	AppSettings() = default;

	AppSettings(const AppSettings&) = delete;
	AppSettings& operator=(const AppSettings&) = delete;

	// -------------------------------------------------
	// Settings : Application
	// -------------------------------------------------
	static const FVector2D windowSize;
	static const std::wstring windowTitle;
};