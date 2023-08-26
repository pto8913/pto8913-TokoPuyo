
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

DrawPlane::DrawPlane(DirectX11& dx, float sizeX, float sizeZ, std::wstring inFileName, std::wstring inVShaderName, std::wstring inPShaderName)
{
	auto model = Plane::Make();
	model.SetTransform(DirectX::XMMatrixScaling(sizeX, 1, sizeZ));

	m_pIndexBuffer = IndexBuffer::Make(dx, "Plane", model.indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "Plane", model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto pTCB = std::make_shared<TransformConstantBuffer>(dx, 0);
	
	AddTask(Texture::Make(dx, Texture::TextureType::WIC, inFileName));
	AddTask(SamplerState::Make(dx));

	auto pVS = VertexShader::Make(dx, inVShaderName, "VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, inPShaderName, "PS"));

	AddTask(pTCB);

	AddTask(Rasterizer::Make(dx, Rasterizer::None, (UINT)model.indices.size()));

	InitializeTasks();
}
