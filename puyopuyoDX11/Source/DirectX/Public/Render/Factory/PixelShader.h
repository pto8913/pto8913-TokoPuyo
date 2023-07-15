#pragma once

#include "Shader.h"

class DirectX11;

class PixelShader : public Shader
{
public:
	PixelShader(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint);
	~PixelShader();

	static std::shared_ptr<PixelShader> Make(DirectX11& dx, const std::wstring& inFileName, const char* pEntryPoint);

	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;

	static std::string GenerateID(const std::wstring& inFileName, const char* pEntryPoint);
	std::string GetID() const noexcept;

private:
	std::wstring fileName;
	ID3D11PixelShader* m_pPixelShader;
};