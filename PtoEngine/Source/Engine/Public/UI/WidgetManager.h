#pragma once

#include "Object/DrawableObject2D.h"
#include "UI/UserWidget.h"

class DirectX11;
class UserWidget;
class Object;

class WidgetManager : private DrawableObject2D
{
public:
	WidgetManager(DirectX11& dx);
	virtual ~WidgetManager();
	operator bool() const noexcept;

protected:
	virtual void RemovePendingObjects();
public:
	void Tick(DirectX11& dx, float deltaTime);

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

	template<class TClass, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<UserWidget, TClass>>>
	TClass* CreateWidget(int inID, Object* inOwner, Args&& ...args)
	{
		auto out = std::make_shared<TClass>(inOwner, pRt2D, std::forward<Args>(args)...);
		out->SetID(inID);
		out->NativeOnInitialized();
		pWidgets.push_back(out);
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