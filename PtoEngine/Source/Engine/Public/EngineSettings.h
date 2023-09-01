#pragma once

#include "Math/Vector.h"
#include <string>

class EngineSettings
{
public:
	EngineSettings(const FVector2D& inSize, const std::wstring& inTitle);

	EngineSettings(const EngineSettings&) = delete;
	EngineSettings& operator=(const EngineSettings&) = delete;

	// -------------------------------------------------
	// Settings : Engine
	// -------------------------------------------------
	/* default (x, y) = (1280, 720) */
	static void SetWindowSize(const FVector2D& inWindowSize);
	/* default (x, y) = (1280, 720) */
	static FVector2D GetWindowSize();

	/* default (x, y) = (1280, 720) */
	static void SetWindowTitle(const std::wstring& inWindowTitle);
	static std::wstring GetWindowTitle();

	/* 
		default (x, y) = (1, 1) 
		will be update from Window.cpp -> Constructor
	*/
	static void SetWindowAspectRatio(const FVector2D& inWindowAspectRatio);
	/*
		default (x, y) = (1, 1)
		will be update from Window.cpp -> Constructor
	*/
	static FVector2D GetWindowAspectRatio();

	// -----------------------
	// Settings : Engine 2D
	// -----------------------
	/* default (x, y) = (80.f, 80.f) */
	static void SetGameScreen2DCellSize(const FVector2D& inGameScreen2DCellSize);
	/* default (x, y) = (80.f, 80.f) */
	static FVector2D GetGameScreen2DCellSize();
	static FVector2D GetCELL();
	static FVector2D GETCELL(const FVector2D& inSize);

	/* default (x, y) = (9, 7) */
	static void SetGameScreen2DSize(const FVector2D& inGameScreen2DSize);
	/* default (x, y) = (9, 7) */
	static FVector2D GetGameScreen2DSize();

	/* default (x, y) = (5, 4) */
	static void SetGameScreen2DCenter(const FVector2D& inGameScreen2DCenter);
	/* default (x, y) = (5, 4) */
	static FVector2D GetGameScreen2DCenter();
};