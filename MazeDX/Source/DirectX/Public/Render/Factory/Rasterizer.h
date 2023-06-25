#pragma once

#include "Bindable.h"

class DirectX11;

class Rasterizer : public Bindable
{
public:
	Rasterizer(DirectX11& dx, bool inTwoSide);
	~Rasterizer();

	static std::shared_ptr<Rasterizer> Make(DirectX11& dx, bool inTwoSide);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(bool inTwoSide)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (inTwoSide ? "2s": "1s");
	}
	std::string GetID() const noexcept
	{
		return GenerateID(bTwoSide);
	}
protected:
	ID3D11RasterizerState* m_pRasterizer;
	bool bTwoSide;
};