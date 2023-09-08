#pragma once

#include "Framework/Level/Level.h"

#include "Actor/Ground/GroundTypes.h"
#include "Actor/Event/EventTypes.h"
#include "Actor/Character/CharacterTypes.h"
#include "Actor/Effect/EffectTypes.h"

#include "Actor/Actor2DTypes.h"

class Actor2D;

class GroundBase;
class EventBase;
class ItemBase;
class CharacterBase;

// ------------------------------------------------------------------------------------------------------------
// Level 2D
// ------------------------------------------------------------------------------------------------------------
class Level2D : public Level
{
public:
	Level2D(DirectX11& dx);
	virtual ~Level2D();
	virtual void SetObjectCollection() override;

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void Generate(DirectX11& dx);
protected:
	virtual void GenerateGroundLayer() = 0;
	virtual void GenerateEventLayer() = 0;
	virtual void GenerateCharacterLayer() {}
	virtual void GenerateEffectLayer() {}

public:
	virtual void Init(const int& x, const int& y);
	virtual void Clear();
protected:
	template<typename T>
	void Clear(TArray<TArray<T>>& in)
	{
		for (auto&& elem : in)
		{
			for (auto&& layer : elem)
			{
				layer.reset();
			}
			elem.Clear();
		}
		in.Clear();
	}
public:
	// --------------------------
	// Main : Utils
	// --------------------------
	const int& GetWidth() const noexcept;
	const int& GetHeight() const noexcept;

	bool IsInWorld(const float& x, const float& y) const noexcept;

	DirectX::XMFLOAT2 WorldToScreen(const float& x, const float& y, const FVector& size);
protected:
	void SetSpriteLocation(std::shared_ptr<Actor2D> sprite, const float& worldX, const float& worldY);
	std::shared_ptr<Actor2D> GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const Layer::EActorLayer& inLayer) const;
public:
	// --------------------------
	// Main : Utils : Ground
	// --------------------------
	virtual std::shared_ptr<GroundBase> SetGroundLayerIDSpe(const EGroundId& groundType, const float& worldX, const float& worldY);
	void SetGroundLayerID(const EGroundId& groundType, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal = false, INT16 inConstantXY2 = -1);
	std::shared_ptr<GroundBase> GetGroundLayer(const int& worldX, const int& worldY) const;

	// --------------------------
	// Main : Utils : Event
	// --------------------------
	void SetEventLayerID(const EEventId& eventType, const float& worldX, const float& worldY);
	void SetEventLayerID(std::shared_ptr<EventBase>& inEventData, const float& worldX, const float& worldY);
	void SetEventLayerID(std::shared_ptr<EventBase>&& inEventData, const float& worldX, const float& worldY);
	std::shared_ptr<EventBase> GetEventLayer(const int& worldX, const int& worldY) const;

	// --------------------------
	// Main : Utils : Character
	// --------------------------
	void SetCharacterLayerID(const ECharacterId& characterType, const float& worldX, const float& worldY);
	std::shared_ptr<CharacterBase> GetCharacterLayer(const int& worldX, const int& worldY) const;

public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	int width = 0;
	int height = 0;
protected:
	bool bInitialized = false;

	// --------------------------
	// State : Display
	// --------------------------
};