#pragma once

#include "DirectX/DirectXHead.h"

class ViewPort
{
public:
	~ViewPort();

	void CreateViewPort(ID3D11DeviceContext* pID3DContext, float width, float height);

	// ------------------------------------------------------
	// ViewPort State
	// ------------------------------------------------------
	void SetModeWireFrame(ID3D11Device* pID3DDevice, ID3D11DeviceContext* pID3DContext);
	void SetModeTransparent(ID3D11Device* pID3DDevice);
	void SetModePixelClipping(ID3D11Device* pID3DDevice);

	void GetTransparentBuffer(ID3D11RasterizerState** OutCCWcull, ID3D11RasterizerState** OutCWcull);
private:
	ID3D11RasterizerState* m_pWireFrame;
	ID3D11RasterizerState* m_pCCWcull;
	ID3D11RasterizerState* m_pCWcull;
	ID3D11RasterizerState* m_pNoCull;
};