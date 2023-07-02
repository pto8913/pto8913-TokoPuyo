
#include "Object/DrawPlane.h"
#include "Core/DirectX.h"

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
	//SetWorldLocation(location);
	//SetWorldRotation(rotation);
	//SetWorldScale(scale);
	//SetWorldTransform();

	auto model = Plane::Make();
	model.SetTransform(DirectX::XMMatrixScaling(size, 1, size));

	/* Shading */
	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	AddTask(pVS);
	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));

	m_pIndexBuffer = IndexBuffer::Make(dx, "Plane", model.indices);
	m_pVertexBuffer = VertexBuffer::Make(dx, "Plane", model.vertices);

	auto pIL = InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get());
	pIL->Bind(dx);

	auto pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTopology->Bind(dx);

	AddTask(m_pIndexBuffer);
	AddTask(m_pVertexBuffer);

	auto pTCB = std::make_shared<TransformConstantBuffer>(dx);
	AddTask(pTCB);

	AddTask(Texture::Make(dx, L"grass.jpg", 0));
	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::Transparent2, m_pIndexBuffer->GetCount()));
	InitializeTasks();
}
