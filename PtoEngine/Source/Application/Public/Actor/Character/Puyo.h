#pragma once

#include "Actor/Actor2D.h"
#include "GameSettings.h"

class DirectX11;
struct PuyoSettings;

class Puyo : public Actor2D
{
public:
	Puyo(DirectX11& dx, const uint8_t& type);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetType(const uint8_t& type);
	uint8_t GetType() const;

	FPuyoInfos& GetPuyoInfos();
private:
	// ------------------------------------------------------
	// Sub
	// ------------------------------------------------------
	uint8_t mType;
	FPuyoInfos mPuyoInfos;
};