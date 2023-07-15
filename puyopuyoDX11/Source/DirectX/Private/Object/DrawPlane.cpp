
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

	m_pIndexBuffer = IndexBuffer::Make(dx, "Plane", model.indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "Plane", model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto pTCB = std::make_shared<TransformConstantBuffer>(dx, 0);
	
	AddTask(Texture::Make(dx, Texture::TextureType::WIC, L"Content/Textures/grass.jpg"));
	AddTask(SamplerState::Make(dx));

	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));
	AddTask(pTCB);

	AddTask(Rasterizer::Make(dx, Rasterizer::None, (UINT)model.indices.size()));

	InitializeTasks();
}
