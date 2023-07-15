#pragma once

#include "Shader.h"

#include "Utils/String.h"

class VertexShader : public Shader
{
public:
	VertexShader(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint);
	~VertexShader();

	void ReleaseShaderCode();

	static std::shared_ptr<VertexShader> Make(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint);
	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;

	static std::string GenerateID(const std::wstring& inFileName, const char* pEntryPoint);
	std::string GetID() const noexcept;

	ID3DBlob* GetBuffer();
private:
	std::wstring fileName;
	ID3D11VertexShader* m_pVertexShader;
	ID3DBlob* m_pVS_Buffer;
};
