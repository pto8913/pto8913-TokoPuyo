#pragma once

#include "Bindable.h"
#include "Math/Vertex.h"

class DirectX11;
class VertexShader;

class InputLayout : public Bindable
{
public:
	InputLayout(DirectX11& dx, DX::Layout::VertexType inVertexType, VertexShader* pVertexShader);
	~InputLayout();

	static std::shared_ptr<InputLayout> Make(DirectX11& dx, DX::Layout::VertexType inVertexType, VertexShader* pVertexShader);
	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;

	static std::string GenerateID(const DX::Layout::VertexType& inVertexType, VertexShader* vs);

	//DX::VertexLayout GetLayout() const;
private:
	DX::Layout::VertexType vertexType;
	ID3D11InputLayout* m_pInputLayout;
};