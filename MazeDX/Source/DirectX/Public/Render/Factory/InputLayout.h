#pragma once

#include "Bindable.h"
#include "Math/Vertex.h"

class DirectX11;
class VertexShader;

class InputLayout : public Bindable
{
public:
	InputLayout(DirectX11& dx, DX::VertexLayout inLayout, VertexShader* pVertexShader);
	~InputLayout();

	static std::shared_ptr<InputLayout> Make(DirectX11& dx, DX::VertexLayout inLayout, VertexShader* pVertexShader);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(const DX::VertexLayout& layout, VertexShader* vs);

	DX::VertexLayout GetLayout() const;
private:
	DX::VertexLayout layout;
	ID3D11InputLayout* m_pInputLayout;
};