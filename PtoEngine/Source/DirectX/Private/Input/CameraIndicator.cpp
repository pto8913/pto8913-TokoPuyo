
#include "Input/CameraIndicator.h"
#include "Core/DirectX.h"

#include "Math/Vertex.h"

#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/ConstantBuffer.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"
#include "Render/InputLayout.h"
#include "Render/Topology.h"
#include "Render/Rasterizer.h"
#include "Render/TransformConstantBuffer.h"

CameraIndicator::CameraIndicator(DirectX11& dx)
{
	std::vector<DX::FVertex> vertices;

	const float x = 4.f / 3.f * 0.75f;
	const float y = 1.f * 0.75f;
	const float z = -2.f;
	const float thalf = x * 0.5f;
	const float tspace = y * 0.2f;
	vertices.emplace_back(-x, y, 0.f );
	vertices.emplace_back( x, y, 0.f );
	vertices.emplace_back( x, -y, 0.f);
	vertices.emplace_back(-x, -y, 0.f);
	vertices.emplace_back(0.f, 0.f, z);
	vertices.emplace_back(-thalf, y + tspace, 0.f);
	vertices.emplace_back(thalf, y + tspace, 0.f);
	vertices.emplace_back(0.f, y + tspace + thalf, 0.f);

	std::vector<DWORD> indices = {
		0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1,
		4, 2, 4, 3, 4, 5, 6, 6, 7, 7, 5
	};

	const std::string tag = "camera";
	m_pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, tag, vertices);
	m_pIndexBuffer = IndexBuffer::Make(dx, tag, indices);

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

	AddTask(Rasterizer::Make(dx, Rasterizer::Transparent, m_pIndexBuffer->GetCount()));
	InitializeTasks();
}