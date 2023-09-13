#pragma once

#include "Framework/Level/Level.h"
#include "Input/MouseInterface.h"

class TitleUI;

class Level_Title : public Level
{
public:
	Level_Title(DirectX11& dx);
	virtual ~Level_Title();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	void OnClickedTokoPuyo(DX::MouseEvent inMouseEvent);
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	TitleUI* pTitleUI = nullptr;
};