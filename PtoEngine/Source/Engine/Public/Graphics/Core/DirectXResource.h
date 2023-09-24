#pragma once

#include "DirectX.h"

class DirectX11;

class DirectXResource
{
protected:
	static ID3D11Device* GetDevice(DirectX11& dx) noexcept;
	static ID3D11DeviceContext* GetContext(DirectX11& dx) noexcept;
};