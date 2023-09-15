#pragma once

#include "Object/DrawableObject2D.h"

class DirectX11;
class UserWidget;
class Object;

class WidgetManager : private DrawableObject2D
{
public:
	WidgetManager(DirectX11& dx);
	virtual ~WidgetManager();

protected:
	virtual void RemovePendingObjects();
public:
	void Tick(DirectX11& dx, float deltaTime);

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

	template<class TClass, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<UserWidget, TClass>>>
	TClass* CreateWidget(Object* inOwner, Args&& ...args)
	{
		auto out = std::make_shared<TClass>(inOwner, pRt2D, std::forward<Args>(args)...);
		pWidgets.push_back(inWidget);
		return std::move(out.get());
	};
protected:
	const TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};

	ID2D1RenderTarget* pRt2D = nullptr;
	IDXGIKeyedMutex* pMutex11 = nullptr;
	IDXGIKeyedMutex* pMutex10 = nullptr;

	std::vector<std::shared_ptr<UserWidget>> pWidgets;
};