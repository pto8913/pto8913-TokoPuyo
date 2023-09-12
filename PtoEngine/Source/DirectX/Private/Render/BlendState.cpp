
#include "Render/BlendState.h"

#include "Core/DirectX.h"

#include "Render/BindableManager.h"

BlendState::BlendState(DirectX11& dx, UINT inSlot)
	: slot(inSlot)
{
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

	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[0] = rtbd;

	HRESULT result = GetDevice(dx)->CreateBlendState(
		&desc,
		&pBlendState
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create BlendState.", L"Create BlendState Error", MB_OK);
		assert(false);
	}
}

BlendState::~BlendState()
{
	Util::SafeRelease(pBlendState);
}

std::shared_ptr<BlendState> BlendState::Make(DirectX11& dx, UINT inSlot)
{
	return BindableManager::Make<BlendState>(dx, inSlot);
}

void BlendState::Bind(DirectX11& dx)
{
	GetContext(dx)->OMSetBlendState(pBlendState, NULL, 0xffffffff);
}