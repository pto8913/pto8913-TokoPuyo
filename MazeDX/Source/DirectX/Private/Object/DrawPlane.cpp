
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
	auto pTCB = std::make_shared<TransformConstantBuffer>(dx);

	auto model = Plane::Make();
	model.SetTransform(DirectX::XMMatrixScaling(size, size, 1));

	auto pVB = VertexBuffer::Make(dx, "Plane", model.vertices);
	pVB->Bind(dx);
	auto pIB = IndexBuffer::Make(dx, "Plane", model.indices);
	pIB->Bind(dx);

	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	pVS->Bind(dx);
	auto pPS = PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS");
	pPS->Bind(dx);

	//AddTask(std::make_shared<ConstantBuffer>(dx, 1));

	auto pInputLayout = InputLayout::Make(dx, model.vertices.GetLayout(), pVS.get());
	pInputLayout->Bind(dx);

	auto pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTopology->Bind(dx);

	AddTask(ConstantBufferPerFrame::Make(dx, sizeof(cbPerObject), 0));
	AddTask(Texture::Make(dx, L"grass.jpg"));
	AddTask(SamplerState::Make(dx, 0));
	AddTask(Rasterizer::Make(dx, true));
	AddTask(pTCB);

	InitializeTasks();
}
