
#include "Object/Sprite.h"
#include "Core/DirectX.h"

#include "Geometry/Plane.h"

#include "Render/Factory/VertexBuffer.h"
#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/ConstantBuffer.h"
#include "Render/Factory/VertexShader.h"
#include "Render/Factory/PixelShader.h"
#include "Render/Factory/InputLayout.h"
#include "Render/Factory/ScreenTexture.h"
#include "Render/Factory/Topology.h"
#include "Render/Factory/Rasterizer.h"
#include "Render/Factory/BlendState.h"
#include "Render/Factory/SamplerState.h"

Sprite::Sprite(DirectX11& dx, const wchar_t* inFileName, std::wstring Tag, DirectX::XMFLOAT2 inSize, DirectX::XMFLOAT2 inOffset)
	: offset(inOffset)
{
	//SetWorldLocation(location);
	//SetWorldRotation(rotation);
	//SetWorldScale(scale);
	//SetWorldTransform();
	AddTask(BlendState::Make(dx, 0));

	auto model = Plane::Make2D();

	m_pIndexBuffer = IndexBuffer::Make(dx, Util::w2String(Tag), model.indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, Util::w2String(Tag), model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pScreenTexture = ScreenTexture::Make(dx, inFileName, inSize.x, inSize.y);
	//AddTask(m_pScreenTexture);
	AddTask(SamplerState::Make(dx));

	auto pVS = VertexShader::Make(dx, L"Shader/Sprite.hlsl", "VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, L"Shader/Sprite.hlsl", "PS"));

	AddTask(Rasterizer::Make(dx, Rasterizer::Transparent2, (UINT)model.indices.size()));
	
	m_pTCB = std::make_shared<TransformConstantBuffer>(dx, 0);
	m_pTCB->InitParentRefrence(*this);
	
	InitializeTasks();
}

void Sprite::ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	pContext->OMSetBlendState(0, 0, 0xffffffff);

	m_pTCB->Bind(dx, pContext, tf);

	m_pScreenTexture->Bind(offset);

	DrawableObject::ExecuteTasks(dx, pContext);
}

DirectX::XMMATRIX Sprite::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity();
}

void Sprite::SetOffset(DirectX::XMFLOAT2 inOffset)
{
	offset = inOffset;
}