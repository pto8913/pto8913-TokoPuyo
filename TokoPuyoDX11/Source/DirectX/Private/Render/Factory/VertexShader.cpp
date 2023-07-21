
#include "Render/Factory/VertexShader.h"

#include "Render/Manager/BindableManager.h"

#include "Utils/String.h"

VertexShader::VertexShader(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint)
	: fileName(inFileName)
{
	if (!Compile(inFileName, pEntryPoint, "vs_4_0", &m_pVS_Buffer))
	{
		MessageBox(NULL, L"Can not Compile VertexShader.", L"Compile VertexShader Error", MB_OK);
		assert(false);
	}

	HRESULT result = GetDevice(dx)->CreateVertexShader(
		m_pVS_Buffer->GetBufferPointer(),
		m_pVS_Buffer->GetBufferSize(),
		NULL,
		&m_pVertexShader
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create VertexShader.", L"Create VertexShader Error", MB_OK);
		assert(false);
	}
}
VertexShader::~VertexShader()
{
	Util::SafeRelease(m_pVertexShader);
	Util::SafeRelease(m_pVS_Buffer);
}

std::shared_ptr<VertexShader> VertexShader::Make(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint)
{
	return BindableManager::Make<VertexShader>(dx, inFileName, pEntryPoint);
}
void VertexShader::Bind(DirectX11& dx)
{
	GetContext(dx)->VSSetShader(m_pVertexShader, nullptr, 0);
}

void VertexShader::ReleaseShaderCode()
{
	Util::SafeRelease(m_pVertexShader);
}
ID3DBlob* VertexShader::GetBuffer()
{
	return m_pVS_Buffer;
}

std::string VertexShader::GenerateID(const std::wstring& inFileName, const char* pEntryPoint)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + Util::w2String(inFileName) + pEntryPoint;
}
std::string VertexShader::GetID() const noexcept
{
	return GenerateID(fileName, "");
}