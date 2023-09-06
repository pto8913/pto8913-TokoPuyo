
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
	//SetWorldLocation(location);
	//SetWorldRotation(rotation);
	//SetWorldScale(scale);
	//SetWorldTransform();
	AddTask(BlendState::Make(dx, 0));

	auto model = Plane::Make2D();

	m_pIndexBuffer = IndexBuffer::Make(dx, Util::w2String(Tag), model.indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, Util::w2String(Tag), model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pScreenTexture = ScreenTexture::Make(dx, inFileName, inSize.x, inSize.y);
	//AddTask(pScreenTexture);
	AddTask(SamplerState::Make(dx));

	auto pVS = VertexShader::Make(dx, L"Shader/Sprite.hlsl", "VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, L"Shader/Sprite.hlsl", "PS"));

	AddTask(Rasterizer::Make(dx, Rasterizer::Transparent2, (UINT)model.indices.size()));

	pTCB = std::make_shared<TransformConstantBuffer>(dx, 0);
	pTCB->InitParentRefrence(*this);

	InitializeTasks();
}

void Sprite::ExecuteTasks(DirectX11& dx)
{
	//dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);

	pTCB->Bind(dx, tf);

	pScreenTexture->Bind(GetLocation(), DirectX::XMVectorGetY(GetRotation()));
	//pScreenTexture->Bind(offset);// { DirectX::XMVectorGetX(GetLocation()), DirectX::XMVectorGetY(GetLocation()) });

	DrawableObject::ExecuteTasks(dx);
}
void Sprite::UpdateTexture(const std::wstring& inFileName)
{
	if (pScreenTexture)
	{
		pScreenTexture->UpdateTexture(inFileName);
	}
}

// ----------------------------------------------------
// Main: Transform
// ----------------------------------------------------
D2D_POINT_2F Sprite::GetCenter()
{
	const auto loc = GetLocation();

	const auto x = DirectX::XMVectorGetX(loc);
	const auto y = DirectX::XMVectorGetY(loc);

	const auto scale = GetScale();

	const auto sx = DirectX::XMVectorGetX(scale);
	const auto sy = DirectX::XMVectorGetY(scale);

	return D2D1::Point2F(
		x + sx / 2.f,
		y + sy / 2.f
	);
}

void Sprite::SetRotation(const DirectX::XMVECTOR& inRotation) noexcept
{
	DrawableObject2D::SetRotation(inRotation);

	//const auto angle = DirectX::XMVectorGetZ(inRotation);
	//auto rt2d = pScreenTexture->GetRt2D();
	//rt2d->SetTransform(
	//	D2D1::Matrix3x2F::Rotation(
	//		angle, 
	//		GetCenter()
	//	)
	//);
}
void Sprite::SetScale(const DirectX::XMVECTOR& inScale) noexcept
{
	DrawableObject2D::SetScale(inScale);

	const auto x = DirectX::XMVectorGetX(inScale);
	const auto y = DirectX::XMVectorGetY(inScale);
	pScreenTexture->UpdateSize(x, y);
}

DirectX::XMMATRIX Sprite::GetTransformXM(DirectX11& dx) const noexcept
{
	return DrawableObject2D::GetTransformXM(dx);
	return DirectX::XMMatrixIdentity();
}