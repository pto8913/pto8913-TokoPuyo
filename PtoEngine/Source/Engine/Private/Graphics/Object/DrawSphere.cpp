
#include "Graphics/Object/DrawSphere.h"
#include "Graphics/Core/DirectX.h"

#include "Graphics/Geometry/Sphere.h"

#include "Graphics/Render/VertexBuffer.h"
#include "Graphics/Render/IndexBuffer.h"
#include "Graphics/Render/ConstantBuffer.h"
#include "Graphics/Render/VertexShader.h"
#include "Graphics/Render/PixelShader.h"
#include "Graphics/Render/InputLayout.h"
#include "Graphics/Render/Texture.h"
#include "Graphics/Render/Topology.h"
#include "Graphics/Render/Rasterizer.h"
#include "Graphics/Render/SamplerState.h"
#include "Graphics/Render/TransformConstantBuffer.h"

DrawSphere::DrawSphere(DirectX11& dx, float radius)
{
	//SetWorldLocation(location);
	//SetWorldRotation(rotation);
	//SetWorldScale(scale);
	//SetWorldTransform();

	auto model = Sphere::Make();
	model.SetTransform(DirectX::XMMatrixScaling(radius, radius, radius));

	pIndexBuffer = IndexBuffer::Make(dx, "Sphere", model.indices);
	pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "Sphere", model.vertices);
	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
