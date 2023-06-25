#pragma once

#include "Math/Vertex.h"
#include <vector>
#include <DirectXMath.h>

class TriangleList
{
public:
	TriangleList() = default;
	TriangleList(DX::VertexBuffer inVertices, std::vector<DWORD> inIndicies)
		: vertices(std::move(inVertices)), indices(std::move(inIndicies))
	{
	}

	void SetTransform(DirectX::FXMMATRIX matrix)
	{
		for (int i = 0; i < vertices.Size(); ++i)
		{
			auto& pos = vertices[i].Attr<DX::VertexLayout::ElementType::Position3D>();
			DirectX::XMStoreFloat3(
				&pos,
				DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&pos), matrix)
			);
		}
	}

	DX::VertexBuffer vertices;
	std::vector<DWORD> indices;
};