#pragma once

#include "EngineSettings.h"

class AppSettings : public EngineSettings
{
public:
	AppSettings(const FVector2D& inSize, const std::wstring& inTitle)
		: EngineSettings(inSize, inTitle)
	{}

	AppSettings(const AppSettings&) = delete;
	AppSettings& operator=(const AppSettings&) = delete;

	// -------------------------------------------------
	// Settings : Application
	// -------------------------------------------------
};