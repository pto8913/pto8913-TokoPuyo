#pragma once

#include "DirectX/DirectXHead.h"

/*
* For the rendering transparent primitives.
*/
class BlendState
{
public:
	~BlendState();

	bool Initialzie(ID3D11Device* pID3DDevice);

	ID3D11BlendState*& Get();
private:
	ID3D11BlendState* m_pBlendState;
};