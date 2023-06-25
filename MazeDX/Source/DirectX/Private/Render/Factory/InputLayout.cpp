#include "Render/Factory/InputLayout.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Manager/BindableManager.h"

InputLayout::InputLayout(DirectX11& dx, DX::VertexLayout inLayout, VertexShader* pVertexShader)
	: layout(inLayout)
{
	const auto d3dLayout = layout.GetD3DLayout();
	const auto pByteCode = pVertexShader->GetBuffer();
	
	HRESULT result = GetDevice(dx)->CreateInputLayout(
		d3dLayout.data(),
		(UINT)d3dLayout.size(),
		pByteCode->GetBufferPointer(),
		pByteCode->GetBufferSize(),
		&m_pInputLayout
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create InputLayout.", L"Create InputLayout Error", MB_OK);
		assert(false);
	}
}
InputLayout::~InputLayout()
{
	Util::SafeRelease(m_pInputLayout);
	ZeroMemory(&layout, layout.Size());
}

std::shared_ptr<InputLayout> InputLayout::Make(DirectX11& dx, DX::VertexLayout inLayout, VertexShader* pVertexShader)
{
	return BindableManager::Make<InputLayout>(dx, inLayout, pVertexShader);
}
void InputLayout::Bind(DirectX11& dx)
{
	GetContext(dx)->IASetInputLayout(m_pInputLayout);
}

std::string InputLayout::GenerateID(const DX::VertexLayout& layout, VertexShader* vs)
{
	using namespace std::string_literals;
	return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vs->GetID();
}

DX::VertexLayout InputLayout::GetLayout() const
{
	return layout;
}