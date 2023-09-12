
#include "Render/SamplerState.h"
#include "Render/BindableManager.h"

SamplerState::SamplerState(DirectX11& dx, UINT inSlot)
	: slot(inSlot)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter =         D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU =       D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV =       D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW =       D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD =         0;
	desc.MaxLOD =         D3D11_FLOAT32_MAX;
	//desc.MaxAnisotropy =  D3D11_REQ_MAXANISOTROPY;

	HRESULT result = GetDevice(dx)->CreateSamplerState(
		&desc,
		&pSamplerState
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create SampleState.", L"Create SampleState Error", MB_OK);
		assert(false);
	}
}
SamplerState::~SamplerState()
{
	Util::SafeRelease(pSamplerState);
}

std::shared_ptr<SamplerState> SamplerState::Make(DirectX11& dx, UINT inSlot)
{
	return BindableManager::Make<SamplerState>(dx, inSlot);
}
void SamplerState::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetSamplers(slot, 1, &pSamplerState);
}