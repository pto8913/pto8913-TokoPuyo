#pragma once

#include "DirectX/DirectXHead.h"

#include "Utils/String.h"

#define VERTEX_TYPE \
	X(V3D) \
	X(V2D) \

namespace DX
{
	class Layout
	{
	public:
		enum VertexType
		{
			#define X(el) el,
			VERTEX_TYPE
			#undef X
		};

		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexDesc(const VertexType& type);
	};

	struct FVertex
	{
	public:
		FVertex()
			: Pos(0, 0, 0), texCoord(0, 0), Normal(0, 1, 0) {};
		FVertex(float x, float y, float z)
			: Pos(x, y, z), texCoord(0, 0), Normal(0, 1, 0) {};
		FVertex(float x, float y, float z, float u, float v)
			: Pos(x, y, z), texCoord(u, v), Normal(0, 1, 0) {};
		FVertex(float u, float v)
			: Pos(0, 0, 0), texCoord(u, v), Normal(0, 1, 0) {};
		FVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
			: Pos(x, y, z), texCoord(u, v), Normal(nx, ny, nz) {};

		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 Normal;
	};

	struct FVertex2D
	{
	public:
		FVertex2D()
			: Pos(0, 0, 0), texCoord(0, 0) {}
		FVertex2D(float x, float y, float z)
			: Pos(x, y, z), texCoord(0, 0) {}
		FVertex2D(float x, float y, float z, float u, float v)
			: Pos(x, y, z), texCoord(u, v) {}
		FVertex2D(float u, float v)
			: Pos(0, 0, 0), texCoord(u, v) {}

		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT2 texCoord;
	};
}

struct Point 
{
public:
	float X, Y, Z, Rhw;
	DWORD Diffuse;
};
