#pragma once

namespace DX
{
	struct ICameraInterface
	{
		virtual void AddMoveLeftRight(float Val) = 0;
		virtual void AddMoveBackForward(float Val) = 0;
		virtual void AddMoveUpDownward(float Val) = 0;

		virtual void AddYaw(float Val) = 0;
		virtual void AddPitch(float Val) = 0;
	};
}