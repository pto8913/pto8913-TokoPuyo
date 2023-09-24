#pragma once

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint);
	~VertexShader();

	void ReleaseShaderCode();

	static std::shared_ptr<VertexShader> Make(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(const std::wstring& inFileName, const char* pEntryPoint);
	std::string GetID() const noexcept;

	ID3DBlob* GetBuffer();
private:
	std::wstring fileName;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3DBlob* pVS_Buffer = nullptr;
};
