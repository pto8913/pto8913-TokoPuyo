
#include "Render/PixelShader.h"

#include "Render/BindableManager.h"

#include "Utils/String.h"

PixelShader::PixelShader(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint)
	: fileName(inFileName)
{
	ID3DBlob* m_pPS_Buffer;
	if (!Compile(inFileName, pEntryPoint, "ps_4_0", &m_pPS_Buffer))
	{
		MessageBox(NULL, L"Can not compile PixelShader.", L"Compile PixelShader Error", MB_OK);
		assert(false);
	}

	HRESULT result = GetDevice(dx)->CreatePixelShader(
		m_pPS_Buffer->GetBufferPointer(),
		m_pPS_Buffer->GetBufferSize(),
		NULL,
		&pPixelShader
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create PixelShader.", L"Create PixelShader Error", MB_OK);
		assert(false);
	}
}
PixelShader::~PixelShader()
{
	Util::SafeRelease(pPixelShader);
}

std::shared_ptr<PixelShader> PixelShader::Make(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint)
{
	return BindableManager::Make<PixelShader>(dx, inFileName, pEntryPoint);
}
void PixelShader::Bind(DirectX11& dx)
{
	GetContext(dx)->PSSetShader(pPixelShader, nullptr, 0);
}

std::string PixelShader::GenerateID(const std::wstring& inFileName, const char* pEntryPoint)
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + Util::w2String(inFileName) + pEntryPoint;
}
std::string PixelShader::GetID() const noexcept
{
	return GenerateID(fileName, "");
}