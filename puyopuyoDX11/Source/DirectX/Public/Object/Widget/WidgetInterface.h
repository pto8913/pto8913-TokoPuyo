#pragma once

#include "Input/MouseInterface.h"

namespace DX
{
	struct IWidgetInterface
	{
		virtual bool OnMouseButtonDown(DX::MouseEvent inMouseEvent) = 0;
		virtual bool OnMouseButtonHeld(DX::MouseEvent inMouseEvent) = 0;
		virtual bool OnMouseButtonUp(DX::MouseEvent inMouseEvent) = 0;
		virtual bool OnMouseEnter(DX::MouseEvent inMouseEvent) = 0;
		virtual bool OnMouseLeave(DX::MouseEvent inMouseEvent) = 0;
		virtual bool OnKeyDown(DX::MouseEvent inMouseEvent) = 0;
		virtual bool OnKeyUp(DX::MouseEvent inMouseEvent) = 0;
	};
};