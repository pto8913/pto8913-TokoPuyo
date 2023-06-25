
#include "Render/Factory/BlendState.h"

BlendState::~BlendState()
{
	Util::SafeRelease(m_pBlendState);
}

bool BlendState::Initialzie(ID3D11Device* pID3DDevice)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable           = TRUE;
	rtbd.SrcBlend              = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp               = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha         = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha        = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0] = rtbd;

	HRESULT result = pID3DDevice->CreateBlendState(&desc, &m_pBlendState);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create BlendState.", L"Create BlendState Error", MB_OK);
		assert(false);
		return false;
	}
	return true;
}

ID3D11BlendState*& BlendState::Get()
{
	return m_pBlendState;
}
