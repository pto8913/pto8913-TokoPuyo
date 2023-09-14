#pragma once

#include "UserWidget.h"

class S_TextBlock;

class DebugUI : public UserWidget
{
public:
	DebugUI(Object* inOwner, DirectX11& dx, DX::IMouseInterface* mouse);
	DebugUI(DirectX11& dx, DX::IMouseInterface* mouse);
	virtual ~DebugUI();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	// -------------------------------------------------------
	// Main : CPU Used
	// -------------------------------------------------------
	void InitCPUUsed();
	float GetCPUUsed();

	// -------------------------------------------------------
	// Main : CPU Current Used
	// -------------------------------------------------------
	void InitCPUCurrentUsed();
	float GetCPUCurrentUsed();
protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------

	// -------------------------------------------------------
	// State : FPS
	// -------------------------------------------------------
	std::shared_ptr<S_TextBlock> pText_FPS = nullptr;

	// -------------------------------------------------------
	// State : CPU
	// -------------------------------------------------------
	std::shared_ptr<S_TextBlock> pText_CPUUsed = nullptr;
	std::shared_ptr<S_TextBlock> pText_CPUCurrentUsed = nullptr;

	// -------------------------------------------------------
	// State : virtual memory
	// -------------------------------------------------------
	std::shared_ptr<S_TextBlock> pText_TotalVirtual = nullptr;
	std::shared_ptr<S_TextBlock> pText_VirtualUsed = nullptr;
	std::shared_ptr<S_TextBlock> pText_VirtualCurrentUsed = nullptr;

	// -------------------------------------------------------
	// State : physical memory
	// -------------------------------------------------------
	std::shared_ptr<S_TextBlock> pText_TotalPhys = nullptr;
	std::shared_ptr<S_TextBlock> pText_PhysUsed = nullptr;
	std::shared_ptr<S_TextBlock> pText_PhysCurrentUsed = nullptr;
};