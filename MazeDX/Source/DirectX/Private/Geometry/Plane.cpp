
#include "Geometry/Plane.h"

#include <DirectXMath.h>

TriangleList Plane::Make()//DX::VertexLayout layout, float size, float u, float v)
{
	//std::string a;
	//for (auto v : plane.vertices)
	//{
	//	a += std::to_string(v.Pos.x);
	//	a += " ";
	//	a += std::to_string(v.Pos.y);
	//	a += " ";
	//	a += std::to_string(v.Pos.z);
	//	a += "\n";
	//}
	//MessageBox(NULL, std::wstring(a.begin(), a.end()).c_str(), L"vertices", MB_OK);

	//std::string b;
	//for (auto v : plane.indices)
	//{
	//	b += std::to_string(v);
	//}
	//MessageBox(NULL, std::wstring(b.begin(), b.end()).c_str(), L"indicies", MB_OK);

	//std::vector<DX::FVertex> vertices{ std::move(layout) };
	//vertices[0].Pos = { -size, -size, -size };
	//vertices[1].Pos = {  size, -size, -size };
	//vertices[2].Pos = {  size, -size,  size };
	//vertices[3].Pos = { -size, -size,  size };
	//
	//vertices[0].texCoord = { u,    v };
	//vertices[1].texCoord = { 0.0f, v };
	//vertices[2].texCoord = { 0.0f, 0.0f };
	//vertices[3].texCoord = { u,    0.0f };

	//return {
	//	std::move(vertices),
	//	{
	//		0,1,2,
	//		0,2,3,
	//	}
	//};

	DX::VertexLayout vl;
	vl.Append(DX::VertexLayout::Position3D);
	vl.Append(DX::VertexLayout::Normal);
	vl.Append(DX::VertexLayout::Texture2D);
	return MakeTesselated(std::move(vl), 1, 1);
}
TriangleList Plane::Make(float size, float u, float v)
{
	DX::VertexLayout vl;
	vl.Append(DX::VertexLayout::Position3D);
	vl.Append(DX::VertexLayout::Normal);
	vl.Append(DX::VertexLayout::Texture2D);

	std::vector<DX::FVertex> vertices(4);
	vertices[0].Pos = { -size, -size, -size };
	vertices[1].Pos = { size, -size, -size };
	vertices[2].Pos = { size, -size,  size };
	vertices[3].Pos = { -size, -size,  size };

	vertices[0].texCoord = { u,    v };
	vertices[1].texCoord = { 0.0f, v };
	vertices[2].texCoord = { 0.0f, 0.0f };
	vertices[3].texCoord = { u,    0.0f };

	vertices[0].Normal = { 0, 1, 0};
	vertices[1].Normal = { 0, 1, 0};
	vertices[2].Normal = { 0, 1, 0};
	vertices[3].Normal = { 0, 1, 0};

	return {
		std::move(vertices),
		{
			0,1,2,
			0,2,3,
		}
	};
}
TriangleList Plane::MakeTesselated(DX::VertexLayout layout, UINT division_x, UINT division_y)
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
				indices.push_back(c1);
				indices.push_back(c3);
				indices.push_back(c0);
				indices.push_back(c3);
				indices.push_back(c2);
			}
		}
	}
	return {
		std::move(vertices),
		std::move(indices)
	};
}