
#include "Object/DrawPyramid.h"

#include "Core/DirectX.h"

#include "Render/Factory/VertexBuffer.h"
#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/ConstantBuffer.h"
#include "Render/Factory/VertexShader.h"
#include "Render/Factory/PixelShader.h"
#include "Render/Factory/InputLayout.h"
#include "Render/Factory/Texture.h"
#include "Render/Factory/Topology.h"
#include "Render/Factory/Rasterizer.h"
#include "Render/Factory/SamplerState.h"
#include "Render/Factory/TransformConstantBuffer.h"

DrawPyramid::DrawPyramid(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ)
{
	std::vector<DWORD> indices = {
		0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 
		6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};
	SetVertices(dx, inWidth, inHeight, inNearZ, inFarZ);
	m_pIndexBuffer = IndexBuffer::Make(dx, "pyramid", indices);

	/* Shading */
	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	AddTask(pVS);
	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get()));

	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	AddTask(m_pIndexBuffer);
	AddTask(m_pVertexBuffer);

	auto pTCB = TransformConstantBuffer::Make(dx, 0);
	AddTask(pTCB);

	AddTask(Rasterizer::Make(dx, Rasterizer::Wire, m_pIndexBuffer->GetCount()));
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

	m_pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "pyramid", vertices);
}