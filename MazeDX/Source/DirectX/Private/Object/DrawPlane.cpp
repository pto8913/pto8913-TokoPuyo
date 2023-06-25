
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

DrawPlane::DrawPlane(DirectX11& dx, float size)
{
	auto model = Plane::Make();
	model.SetTransform(DirectX::XMMatrixScaling(size, size, 1));

	auto pVB = VertexBuffer::Make(dx, "Plane", model.vertices);
	pVB->Bind(dx);

	auto pIB = IndexBuffer::Make(dx, "Plane", model.indices);
	pIB->Bind(dx);

	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	//auto pVSBuffer = pVS->GetBuffer();
	AddTask(pVS);

	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));

	//AddTask(std::make_shared<ConstantBuffer>(dx, 1));

	AddTask(InputLayout::Make(dx, model.vertices.GetLayout(), pVS.get()));

	auto pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pTopology->Bind(dx);

	AddTask(Texture::Make(dx, L"grass.jpg"));

	AddTask(SamplerState::Make(dx, 0));

	AddTask(ConstantBufferPerFrame::Make(dx, sizeof(cbPerObject), 0));

	AddTask(Rasterizer::Make(dx, true));
}
