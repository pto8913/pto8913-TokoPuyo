
#include "Graphics/Geometry/Plane.h"

#include <DirectXMath.h>

TriangleList Plane::Make()
{
	return MakeTesselated(1, 1);
}
TriangleList Plane::Make(float size, float u, float v)
{
	std::vector<DX::FVertex> vertices = {
		DX::FVertex(-size, -size, -size, u,    v,    0.0f, 1.0f, 0.0f),
		DX::FVertex( size, -size, -size, 0.0f, v,    0.0f, 1.0f, 0.0f),
		DX::FVertex( size, -size,  size, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		DX::FVertex(-size, -size,  size, u,    0.0f, 0.0f, 1.0f, 0.0f),
	};

	//std::vector<DX::FVertex> vertices(4);
	//vertices[0].Pos = { -size, -size, -size };
	//vertices[1].Pos = {  size, -size, -size };
	//vertices[2].Pos = {  size, -size,  size };
	//vertices[3].Pos = { -size, -size,  size };

	//vertices[0].texCoord = { u,    v };
	//vertices[1].texCoord = { 0.0f, v };
	//vertices[2].texCoord = { 0.0f, 0.0f };
	//vertices[3].texCoord = { u,    0.0f };

	//vertices[0].Normal = { 0, 1, 0};
	//vertices[1].Normal = { 0, 1, 0};
	//vertices[2].Normal = { 0, 1, 0};
	//vertices[3].Normal = { 0, 1, 0};

	return {
		std::move(vertices),
		{
			0,1,2,
			0,2,3,
		}
	};
}
TriangleList Plane::MakeTesselated(UINT division_x, UINT division_y)
{
	assert(division_x >= 1);
	assert(division_y >= 1);

	constexpr float width = 2.f;
	constexpr float height = 2.f;
	const int nVertices_x = division_x + 1;
	const int nVertices_y = division_y + 1;
	DX::FVertex vertex;
	std::vector<DX::FVertex> vertices;
	{
		const float side_x = width / 2.f;
		const float side_y = height / 2.f;
		const float divisionSize_x = width / float(division_x);
		const float divisionSize_y = height / float(division_y);
		const float divisionSize_x_texCoord = 1.f / float(division_x);
		const float divisionSize_y_texCoord = 1.f / float(division_y);
		for (int y = 0; y < nVertices_y; ++y)
		{
			const float y_pos = float(y) * divisionSize_y - side_y;
			const float y_pos_texCoord = 1.f - float(y) * divisionSize_y_texCoord;
			for (int x = 0; x < nVertices_x; ++x)
			{
				const float x_pos = float(x) * divisionSize_x - side_x;
				const float x_pos_texCoord = float(x) * divisionSize_x_texCoord;

				vertex.Pos = { x_pos, -1, y_pos };
				vertex.texCoord = { x_pos_texCoord, y_pos_texCoord };
				vertex.Normal = { 0, 1, 0 };
				vertices.emplace_back(vertex);
			}
		}
	}

	std::vector<DWORD> indices;
	{
		const auto Pos = [nVertices_x](int x, int y)
		{
			return (DWORD)(y * nVertices_x + x);
		};

		for (UINT y = 0; y < division_y; ++y)
		{
			for (UINT x = 0; x < division_x; ++x)
			{
				DWORD c0 = Pos(x, y);
				DWORD c1 = Pos(x + 1, y);
				DWORD c2 = Pos(x, y + 1);
				DWORD c3 = Pos(x + 1, y + 1);
				indices.push_back(c0);
				indices.push_back(c2);
				indices.push_back(c1);
				indices.push_back(c1);
				indices.push_back(c2);
				indices.push_back(c3);
			}
		}
	}
	return {
		std::move(vertices),
		std::move(indices)
	};
}


TriangleList2D Plane::Make2D()
{
	return MakeTesselated2D(1, 1);
}
TriangleList2D Plane::Make2D(float size, float u, float v)
{
	std::vector<DX::FVertex2D> vertices = {
		DX::FVertex2D(-size, -size, -size, u,    v),
		DX::FVertex2D( size, -size, -size, 0.0f, v),
		DX::FVertex2D( size, -size,  size, 0.0f, 0.0f),
		DX::FVertex2D(-size, -size,  size, u,    0.0f),
	};
	return {
		std::move(vertices),
		{
			0,1,2,
			0,2,3,
		}
	};
}
TriangleList2D Plane::MakeTesselated2D(UINT division_x, UINT division_y)
{
	assert(division_x >= 1);
	assert(division_y >= 1);

	constexpr float width = 2.f;
	constexpr float height = 2.f;
	const int nVertices_x = division_x + 1;
	const int nVertices_y = division_y + 1;
	DX::FVertex2D vertex;
	std::vector<DX::FVertex2D> vertices;
	{
		const float side_x = width / 2.f;
		const float side_y = height / 2.f;
		const float divisionSize_x = width / float(division_x);
		const float divisionSize_y = height / float(division_y);
		const float divisionSize_x_texCoord = 1.f / float(division_x);
		const float divisionSize_y_texCoord = 1.f / float(division_y);
		for (int y = 0; y < nVertices_y; ++y)
		{
			const float y_pos = float(y) * divisionSize_y - side_y;
			const float y_pos_texCoord = 1.f - float(y) * divisionSize_y_texCoord;
			for (int x = 0; x < nVertices_x; ++x)
			{
				const float x_pos = float(x) * divisionSize_x - side_x;
				const float x_pos_texCoord = float(x) * divisionSize_x_texCoord;

				vertex.Pos = { x_pos, -1, y_pos };
				vertex.texCoord = { x_pos_texCoord, y_pos_texCoord };
				vertices.emplace_back(vertex);
			}
		}
	}

	std::vector<DWORD> indices;
	{
		const auto Pos = [nVertices_x](int x, int y)
		{
			return (DWORD)(y * nVertices_x + x);
		};

		for (UINT y = 0; y < division_y; ++y)
		{
			for (UINT x = 0; x < division_x; ++x)
			{
				DWORD c0 = Pos(x, y);
				DWORD c1 = Pos(x + 1, y);
				DWORD c2 = Pos(x, y + 1);
				DWORD c3 = Pos(x + 1, y + 1);
				indices.push_back(c0);
				indices.push_back(c2);
				indices.push_back(c1);
				indices.push_back(c1);
				indices.push_back(c2);
				indices.push_back(c3);
			}
		}
	}
	return {
		std::move(vertices),
		std::move(indices)
	};
}