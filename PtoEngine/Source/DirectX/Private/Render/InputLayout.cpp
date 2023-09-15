#include "Render/InputLayout.h"
#include "Render/VertexShader.h"

#include "Render/BindableManager.h"

InputLayout::InputLayout(DirectX11& dx, DX::Layout::VertexType inVertexType, VertexShader* pVertexShader)
	: vertexType(inVertexType)
{
	const auto desc = DX::Layout::GetVertexDesc(inVertexType);
	const auto pByteCode = pVertexShader->GetBuffer();

	HRESULT result = GetDevice(dx)->CreateInputLayout(
		desc.data(),
		(UINT)desc.size(),
		pByteCode->GetBufferPointer(),
		pByteCode->GetBufferSize(),
		&pInputLayout
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create InputLayout.", L"Create InputLayout Error", MB_OK);
		assert(false);
	}
}
InputLayout::~InputLayout()
{
	Util::SafeRelease(pInputLayout);
}

std::shared_ptr<InputLayout> InputLayout::Make(DirectX11& dx, DX::Layout::VertexType inVertexType, VertexShader* pVertexShader)
{
	return BindableManager::Make<InputLayout>(dx, inVertexType, pVertexShader);
}
void InputLayout::Bind(DirectX11& dx)
{
	GetContext(dx)->IASetInputLayout(pInputLayout);
}

std::string InputLayout::GenerateID(const DX::Layout::VertexType&, VertexShader* vs)
{
	return "InputLayout_" + vs->GetID();
}

//DX::VertexLayout InputLayout::GetLayout() const
//{
//	return layout;
//}