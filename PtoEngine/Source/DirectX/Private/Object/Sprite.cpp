
#include "Object/Sprite.h"
#include "Core/DirectX.h"

#include "Render/ScreenTexture.h"

Sprite::Sprite(DirectX11& dx, const std::wstring& inFileName, std::wstring Tag, DirectX::XMFLOAT2 inSize)
{
	pScreenTexture = ScreenTexture::Make(dx, inFileName, inSize.x, inSize.y);
}
Sprite::~Sprite()
{
	pScreenTexture.reset();
	pScreenTexture = nullptr;
}

void Sprite::ExecuteTasks(DirectX11& dx)
{
	pScreenTexture->Bind(GetLocation(), DirectX::XMVectorGetY(GetRotation()));
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
	ObjectBase::SetRotation(inRotation);

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
	ObjectBase::SetScale(inScale);

	const auto x = DirectX::XMVectorGetX(inScale);
	const auto y = DirectX::XMVectorGetY(inScale);
	pScreenTexture->UpdateSize(x, y);
}

DirectX::XMMATRIX Sprite::GetTransformXM(DirectX11& dx) const noexcept
{
	return ObjectBase::GetTransformXM(dx);
	return DirectX::XMMatrixIdentity();
}