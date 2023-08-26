
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

Sprite::Sprite(DirectX11& dx, const std::wstring& inFileName, std::wstring Tag, DirectX::XMFLOAT2 inSize)
{
	AddTask(BlendState::Make(dx, 0));

	auto pVS = VertexShader::Make(dx, L"Shader/Sprite.hlsl", "VS");
	AddTask(PixelShader::Make(dx, L"Shader/Sprite.hlsl", "PS"));
	AddTask(pVS);

	auto model = Plane::Make2D();
	m_pIndexBuffer = IndexBuffer::Make(dx, Util::w2String(Tag), model.indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, Util::w2String(Tag), model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));

	AddTask(m_pIndexBuffer);
	AddTask(m_pVertexBuffer);

	m_pTCB = std::make_shared<TransformConstantBuffer>(dx, 0);
	m_pTCB->InitParentRefrence(*this);

	m_pScreenTexture = ScreenTexture::Make(dx, inFileName, inSize.x, inSize.y);
	AddTask(m_pScreenTexture);

	AddTask(SamplerState::Make(dx));

	AddTask(Rasterizer::Make(dx, Rasterizer::Transparent2, m_pIndexBuffer->GetCount()));

	InitializeTasks();
}

void Sprite::ExecuteTasks(DirectX11& dx)
{
	//dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);

	m_pTCB->Bind(dx, tf);

	m_pScreenTexture->Bind(GetLocation());

	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX Sprite::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity();
}

void Sprite::UpdateTexture(const std::wstring& inFileName)
{
	if (m_pScreenTexture)
	{
		m_pScreenTexture->UpdateTexture(inFileName);
	}
}