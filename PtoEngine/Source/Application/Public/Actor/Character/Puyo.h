#pragma once

#include "Actor/Actor2D.h"
#include "GameSettings.h"

class DirectX11;
struct PuyoSettings;

class Puyo : public Actor2D
{
	friend class World;
public:
	Puyo(DirectX11& dx, const uint8_t& type);
	virtual ~Puyo();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "Puyo" + "#"s + std::to_string(mID);
	}
protected:
	virtual void SetID(int inID) override;
public:
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