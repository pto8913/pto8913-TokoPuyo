#pragma once

#include "DirectX/DirectXHead.h"

class AppSettings
{
public:
	AppSettings() = default;

	AppSettings(const AppSettings&) = delete;
	AppSettings& operator=(const AppSettings&) = delete;

	// -------------------------------------------------
	// Settings : Application
	// -------------------------------------------------
	static const DirectX::XMFLOAT2 windowSize;
	static const std::wstring windowTitle;
};