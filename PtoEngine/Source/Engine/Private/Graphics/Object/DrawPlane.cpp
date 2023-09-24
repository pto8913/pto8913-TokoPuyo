
#include "Graphics/Object/DrawPlane.h"
#include "Graphics/Core/DirectX.h"

#include "Graphics/Geometry/Plane.h"

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

DrawPlane::DrawPlane(DirectX11& dx, float sizeX, float sizeZ, std::wstring inFileName, std::wstring inVShaderName, std::wstring inPShaderName)
{
	auto model = Plane::Make();
	model.SetTransform(DirectX::XMMatrixScaling(sizeX, 1, sizeZ));

	pIndexBuffer = IndexBuffer::Make(dx, "Plane", model.indices);
	pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "Plane", model.vertices);
	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
