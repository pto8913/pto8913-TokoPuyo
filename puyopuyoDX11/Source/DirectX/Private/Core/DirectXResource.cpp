
#include "Core/DirectXResource.h"

ID3D11Device* DirectXResource::GetDevice(DirectX11& dx) noexcept
{
	return dx.m_pID3DDevice;
}
ID3D11DeviceContext* DirectXResource::GetContext(DirectX11& dx) noexcept
{
	return dx.m_pID3DContext;
}