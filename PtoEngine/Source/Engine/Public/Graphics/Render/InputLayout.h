#pragma once

#include "Bindable.h"
#include "Vertex.h"

class DirectX11;
class VertexShader;

class InputLayout : public Bindable
{
public:
	InputLayout(DirectX11& dx, DX::Layout::VertexType inVertexType, VertexShader* pVertexShader);
	virtual ~InputLayout();

	static std::shared_ptr<InputLayout> Make(DirectX11& dx, DX::Layout::VertexType inVertexType, VertexShader* pVertexShader);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(const DX::Layout::VertexType& inVertexType, VertexShader* vs);

	//DX::VertexLayout GetLayout() const;
private:
	DX::Layout::VertexType vertexType;
	ID3D11InputLayout* pInputLayout = nullptr;
};