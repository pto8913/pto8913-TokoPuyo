
#include "Object/DrawPlane.h"

#include "Geometry/Plane.h"

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

DrawPlane::DrawPlane(DirectX11& dx, float size)
{
	auto model = Plane::Make();
	//model.SetTransform(DirectX::XMMatrixScaling(size, size, 1));

	m_pVertexBuffer = VertexBuffer::Make(dx, "Plane", model.vertices);
	m_pIndexBuffer = IndexBuffer::Make(dx, "Plane", model.indices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto pTCB = std::make_shared<TransformConstantBuffer>(dx);
	
	//AddTask(std::make_shared<ConstantBuffer>(dx, 1));

	/* Shading */
	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");

	auto pIL = InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get());
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));
	
	AddTask(m_pIndexBuffer);
	AddTask(m_pVertexBuffer);

	AddTask(pTCB);

	AddTask(Texture::Make(dx, L"grass.jpg"));
	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::None));
	
	InitializeTasks();

	pIL->Bind(dx);
	m_pTopology->Bind(dx);
}
