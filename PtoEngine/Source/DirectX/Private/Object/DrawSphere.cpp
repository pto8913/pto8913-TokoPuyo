
#include "Object/DrawSphere.h"
#include "Core/DirectX.h"

#include "Geometry/Sphere.h"

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

DrawSphere::DrawSphere(DirectX11& dx, float radius)
{
	//SetWorldLocation(location);
	//SetWorldRotation(rotation);
	//SetWorldScale(scale);
	//SetWorldTransform();

	auto model = Sphere::Make();
	model.SetTransform(DirectX::XMMatrixScaling(radius, radius, radius));

	m_pIndexBuffer = IndexBuffer::Make(dx, "Sphere", model.indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "Sphere", model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto pTCB = std::make_shared<TransformConstantBuffer>(dx);

	AddTask(Texture::Make(dx, Texture::TextureType::WIC, L"Content/Textures/grass.jpg"));
	AddTask(SamplerState::Make(dx));

	auto pVS = VertexShader::Make(dx, L"Shader/Shader.hlsl", "VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, L"Shader/Shader.hlsl", "PS"));
	AddTask(pTCB);

	AddTask(Rasterizer::Make(dx, Rasterizer::None, (UINT)model.indices.size()));

	InitializeTasks();
}

DirectX::XMMATRIX DrawSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslationFromVector(location);
}
