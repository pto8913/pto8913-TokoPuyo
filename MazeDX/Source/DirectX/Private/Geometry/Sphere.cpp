
#include "Geometry/Sphere.h"
#include <DirectXMath.h>

#include "Math/Math.h"

TriangleList Sphere::MakeTesselated(int latDiv, int longDiv)
{
	constexpr float radius = 1.f;
	const DirectX::XMVECTOR base = DirectX::XMVectorSet(0, 0, radius, 0);
	const float latAngle = PI / latDiv;
	const float longAngle = 2.f * PI / longDiv;
	DX::FVertex vertex;
	std::vector<DX::FVertex> vertices;
	for (int lat_i = 1; lat_i < latDiv; ++lat_i)
	{
		const auto latBase = DirectX::XMVector3Transform(
			base,
			DirectX::XMMatrixRotationX(latAngle * lat_i)
		);
		for (int long_i = 0; long_i < longDiv; ++long_i)
		{
			DirectX::XMFLOAT3 calcPos;
			DirectX::XMVECTOR v = DirectX::XMVector3Transform(
				latBase,
				DirectX::XMMatrixRotationZ(longAngle * long_i)
			);
			DirectX::XMStoreFloat3(&calcPos, v);
			vertex.Pos = calcPos;
			vertices.emplace_back(vertex);
		}
	}
	const DWORD North = (DWORD)vertices.size();
	DirectX::XMFLOAT3 northPos;
	DirectX::XMStoreFloat3(&northPos, base);
	vertex.Pos = northPos;
	vertices.emplace_back(vertex);
	
	const DWORD South = (DWORD)vertices.size();
	DirectX::XMFLOAT3 southPos;
	DirectX::XMStoreFloat3(&southPos, base);
	vertex.Pos = southPos;
	vertices.emplace_back(vertex);

	const auto Pos = [longDiv](int lat_i, int long_i)
	{
		return lat_i * longDiv + long_i;
	};

	std::vector<DWORD> indices;
	for (int lat_i = 0; lat_i < latDiv - 2; ++lat_i)
	{
		for (int long_i = 0; long_i < longDiv - 1; ++long_i)
		{
			indices.push_back(Pos(lat_i,     long_i));
			indices.push_back(Pos(lat_i + 1, long_i));
			indices.push_back(Pos(lat_i,     long_i + 1));
			indices.push_back(Pos(lat_i,     long_i + 1));
			indices.push_back(Pos(lat_i + 1, long_i));
			indices.push_back(Pos(lat_i + 1, long_i + 1));
		}
		indices.push_back(Pos(lat_i,     longDiv - 1));
		indices.push_back(Pos(lat_i + 1, longDiv - 1));
		indices.push_back(Pos(lat_i,     0));
		indices.push_back(Pos(lat_i,     0));
		indices.push_back(Pos(lat_i + 1, longDiv - 1));
		indices.push_back(Pos(lat_i + 1, 0));
	}
	for (int long_i = 0; long_i < longDiv - 1; ++long_i)
	{
		indices.push_back(North);
		indices.push_back(Pos(0, long_i));
		indices.push_back(Pos(0, long_i + 1));
		
		indices.push_back(Pos(latDiv - 2, long_i + 1));
		indices.push_back(Pos(latDiv - 2, long_i));
		indices.push_back(South);
	}

	indices.push_back(North);
	indices.push_back(Pos(0,          longDiv - 1));
	indices.push_back(Pos(0,          0));

	indices.push_back(Pos(latDiv - 2, 0));
	indices.push_back(Pos(latDiv - 2, longDiv - 1));
	indices.push_back(South);

	return {
		std::move(vertices),
		std::move(indices)
	};
}
TriangleList Sphere::Make()
{
	return MakeTesselated(12, 24);
}