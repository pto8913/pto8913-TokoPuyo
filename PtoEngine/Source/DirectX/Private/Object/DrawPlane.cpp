
#include "Object/DrawPlane.h"
#include "Core/DirectX.h"

#include "Geometry/Plane.h"

#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/ConstantBuffer.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"
#include "Render/InputLayout.h"
#include "Render/Texture.h"
#include "Render/Topology.h"
#include "Render/Rasterizer.h"
#include "Render/SamplerState.h"
#include "Render/TransformConstantBuffer.h"

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
