#pragma once

#include <d2d1.h>
#include <memory>

class SlateBase;
class Level2D;
class DirectX11;

namespace DX
{
	struct HUDInterface
	{
		// ------------------------------------------------------
		// HUDInterface
		// ------------------------------------------------------
		virtual ID2D1RenderTarget* GetRt2D() = 0;

		// --------------------------
		// HUDInterface : External
		// --------------------------
		virtual void AddSlate(std::shared_ptr<SlateBase> inSlate) = 0;
		virtual void RemoveSlate(std::shared_ptr<SlateBase> inSlate) = 0;

		// --------------------------
		// HUDInterface : Game Infos : Map
		// --------------------------
		virtual void UpdateMap(const Level2D* pWorld) = 0;

		// --------------------------
		// HUDInterface : Effect
		// --------------------------
		virtual void Effect_Blackout(float playRate = 0.5f) = 0;
	};
}