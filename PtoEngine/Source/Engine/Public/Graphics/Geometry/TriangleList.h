#pragma once

#include "Graphics/Render/Vertex.h"
#include <vector>
#include <DirectXMath.h>

class TriangleList
{
public:
	TriangleList() = default;
	TriangleList(std::vector<DX::FVertex> inVertices, std::vector<DWORD> inIndicies)
		: vertices(std::move(inVertices)), indices(std::move(inIndicies))
	{
	}

	void SetTransform(DirectX::FXMMATRIX matrix)
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			auto& pos = vertices[i].Pos;
			DirectX::XMStoreFloat3(
				&pos,
				DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&pos), matrix)
			);
		}
	}

	std::vector<DX::FVertex> vertices;
	std::vector<DWORD> indices;
};

class TriangleList2D
{
public:
	TriangleList2D() = default;
	TriangleList2D(std::vector<DX::FVertex2D> inVertices, std::vector<DWORD> inIndicies)
		: vertices(std::move(inVertices)), indices(std::move(inIndicies))
	{
	}

	void SetTransform(DirectX::FXMMATRIX matrix)
	{
		for (int i = 0; i < vertices.size(); ++i)
		{
			auto& pos = vertices[i].Pos;
			DirectX::XMStoreFloat3(
				&pos,
				DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&pos), matrix)
			);
		}
	}

	std::vector<DX::FVertex2D> vertices;
	std::vector<DWORD> indices;
};