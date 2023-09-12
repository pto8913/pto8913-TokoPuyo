
#include "Object/DrawPyramid.h"

#include "Core/DirectX.h"

#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/ConstantBuffer.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"
#include "Render/InputLayout.h"
#include "Render/Texture.h"
#include "Render/Topology.h"
#include "Render/Rasterizer.h"
#include "Render/SamplerState.h"
#include "Render/TransformConstantBuffer.h"

DrawPyramid::DrawPyramid(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ)
{
	std::vector<DWORD> indices = {
		0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 
		6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};
	SetVertices(dx, inWidth, inHeight, inNearZ, inFarZ);
	pIndexBuffer = IndexBuffer::Make(dx, "pyramid", indices);

	/* Shading */
	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	AddTask(pVS);
	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get()));

	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	AddTask(pIndexBuffer);
	AddTask(pVertexBuffer);

	auto pTCB = TransformConstantBuffer::Make(dx, 0);
	AddTask(pTCB);

	AddTask(Rasterizer::Make(dx, Rasterizer::Wire, pIndexBuffer->GetCount()));
	InitializeTasks();
}
void DrawPyramid::SetVertices(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ)
{
	std::vector<DX::FVertex> vertices;

	const float zRatio = inFarZ / inNearZ;
	const float nearX = inWidth / 2.f;
	const float nearY = inHeight / 2.f;
	const float farX = nearX * zRatio;
	const float farY = nearY * zRatio;
	vertices.emplace_back(-nearX,  nearY, inNearZ);
	vertices.emplace_back( nearX,  nearY, inNearZ);
	vertices.emplace_back( nearX, -nearY, inNearZ);
	vertices.emplace_back(-nearX, -nearY, inNearZ);
	vertices.emplace_back(-farX,   farY,  inFarZ);
	vertices.emplace_back( farX,   farY,  inFarZ);
	vertices.emplace_back( farX,  -farY,  inFarZ);
	vertices.emplace_back(-farX,  -farY,  inFarZ);

	pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "pyramid", vertices);
}