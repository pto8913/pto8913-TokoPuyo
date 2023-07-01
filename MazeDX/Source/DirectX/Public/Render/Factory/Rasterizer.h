#pragma once

#include "Bindable.h"

class DirectX11;

class Rasterizer : public Bindable
{
public:
	enum RasterizerType
	{
		None,
		Wire,
		Transparent,
	};
	Rasterizer(DirectX11& dx, RasterizerType inType, UINT inSize);
	~Rasterizer();

	static std::shared_ptr<Rasterizer> Make(DirectX11& dx, RasterizerType inType, UINT inSize);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(RasterizerType inType, UINT inSize)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (inType ? "2s": "1s") + "#"s + std::to_string(inSize);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(type, size);
	}
	void Get(ID3D11RasterizerState& Out1, ID3D11RasterizerState& Out2);
protected:
	ID3D11RasterizerState* m_pRasterizer_1;
	ID3D11RasterizerState* m_pRasterizer_2;
	RasterizerType type;
	UINT size;
};