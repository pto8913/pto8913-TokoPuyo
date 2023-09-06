
#include "Component/SpriteComponent.h"

#include "EngineSettings.h"

#include "Helper/VectorHelper.h"

using namespace DirectX;

SpriteComponent::SpriteComponent(Actor* inOwner, DirectX11& dx, const std::wstring& inFileName, std::wstring inTag, DirectX::XMFLOAT2 inSize)
	: ActorComponent(inOwner), Sprite(
		dx,
		inFileName,
		inTag,
		inSize
	)
{
	auto c = EngineSettings::GETCELL(Vector::ConvertDXToVector2D(inSize));
	SetActorScale({ c.x, c.y, 0 });
}

SpriteComponent::~SpriteComponent()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void SpriteComponent::Tick(DirectX11& dx, float deltaTime)
{
	if (GetTickEnabled())
	{
		ExecuteTasks(dx);
		ActorComponent::Tick(dx, deltaTime);
	}
}

// -----------------------------------
// Main : Transform
// -----------------------------------
FVector SpriteComponent::GetActorLocation()
{
	const auto vec = ActorComponent::GetActorLocation();
	const auto ratio = EngineSettings::GetWindowAspectRatio();
	return {
		vec.x / ratio.x,
		vec.y / ratio.y,
		vec.z
	};
}
void SpriteComponent::SetActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.y, in.z, 0.f);
	SetLocation(vec);
	ActorComponent::SetActorLocation(in);
}
FRotator SpriteComponent::GetActorRotation()
{
	return mLocalRotation;
}
void SpriteComponent::AddActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.y, in.z, 0.f);
	vec = DirectX::XMVectorAdd(GetLocation(), vec);
	SetLocation(vec);
	ActorComponent::AddActorLocation(in);
}
void SpriteComponent::SetActorRotation(const FRotator& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.pitch, in.yaw, in.roll, 0.f);
	SetRotation(vec);
	ActorComponent::SetActorRotation(in);
}
FVector SpriteComponent::GetActorScale()
{
	return mLocalScale;
}
void SpriteComponent::SetActorScale(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.y, in.z, 0.f);
	SetScale(vec);
	ActorComponent::SetActorScale(in);
}