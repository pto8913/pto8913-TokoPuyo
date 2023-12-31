
#include "Graphics/Render/VertexShader.h"

#include "Graphics/Render/BindableManager.h"

#include "Utils/String.h"

VertexShader::VertexShader(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint)
	: fileName(inFileName)
{
	if (!Compile(inFileName, pEntryPoint, "vs_4_0", &pVS_Buffer))
	{
		MessageBox(NULL, L"Can not Compile VertexShader.", L"Compile VertexShader Error", MB_OK);
		assert(false);
	}

	HRESULT result = GetDevice(dx)->CreateVertexShader(
		pVS_Buffer->GetBufferPointer(),
		pVS_Buffer->GetBufferSize(),
		NULL,
		&pVertexShader
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create VertexShader.", L"Create VertexShader Error", MB_OK);
		assert(false);
	}
}
VertexShader::~VertexShader()
{
	Util::SafeRelease(pVertexShader);
	Util::SafeRelease(pVS_Buffer);
}

std::shared_ptr<VertexShader> VertexShader::Make(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint)
{
	return BindableManager::Make<VertexShader>(dx, inFileName, pEntryPoint);
}
void VertexShader::Bind(DirectX11& dx)
{
	GetContext(dx)->VSSetShader(pVertexShader, nullptr, 0);
}

void VertexShader::ReleaseShaderCode()
{
	Util::SafeRelease(pVertexShader);
}
ID3DBlob* VertexShader::GetBuffer()
{
	return pVS_Buffer;
}

std::string VertexShader::GenerateID(const std::wstring& inFileName, const char* pEntryPoint)
{
	return "VertexShader_" + Util::w2String(inFileName) + pEntryPoint;
}
std::string VertexShader::GetID() const noexcept
{
	return GenerateID(fileName, "");
}