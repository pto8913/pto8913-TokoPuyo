
#include "UI/UserWidget.h"

#include "Math/Math.h"

#include "Framework/World.h"

UserWidget::UserWidget(DirectX11& dx, DX::IMouseInterface* mouse, float windowSizeW, float windowSizeH)
	: WidgetBase(dx, windowSizeW, windowSizeH),
	pMouse(mouse)
{
	SetTickEnabled(true);

	if (pMouse)
	{
		pMouse->GetClickedLeftPressed().Bind<&UserWidget::OnMouseButtonDown>(*this, "UserWidget");
		pMouse->GetClickedLeftReleased().Bind<&UserWidget::OnMouseButtonUp>(*this, "UserWidget");
		pMouse->GetClickedLeftHeld().Bind<&UserWidget::OnMouseButtonHeld>(*this, "UserWidget");

		pMouse->GetClickedRightPressed().Bind<&UserWidget::OnMouseButtonDown>(*this, "UserWidget");
		pMouse->GetClickedRightReleased().Bind<&UserWidget::OnMouseButtonUp>(*this, "UserWidget");
		pMouse->GetClickedRightHeld().Bind<&UserWidget::OnMouseButtonHeld>(*this, "UserWidget");

		pMouse->GetClickedWheelPressed().Bind<&UserWidget::OnMouseButtonDown>(*this, "UserWidget");
		pMouse->GetClickedWheelReleased().Bind<&UserWidget::OnMouseButtonUp>(*this, "UserWidget");
		pMouse->GetClickedWheelHeld().Bind<&UserWidget::OnMouseButtonHeld>(*this, "UserWidget");

		pMouse->GetMouseMove().Bind<&UserWidget::OnMouseMove>(*this, "UserWidget");
	}
}
UserWidget::UserWidget(std::shared_ptr<Object> inOwner, DirectX11& dx, DX::IMouseInterface* mouse, float windowSizeW, float windowSizeH)
	: UserWidget(dx, mouse, windowSizeW, windowSizeH)
{
	if (inOwner != nullptr)
	{
		pOwner = inOwner;
	}
}

UserWidget::~UserWidget()
{
	if (pMouse)
	{
		pMouse->GetClickedLeftPressed().Unbind("UserWidget");
		pMouse->GetClickedLeftReleased().Unbind("UserWidget");
		pMouse->GetClickedLeftHeld().Unbind("UserWidget");
		pMouse->GetClickedRightPressed().Unbind("UserWidget");
		pMouse->GetClickedRightReleased().Unbind("UserWidget");
		pMouse->GetClickedRightHeld().Unbind("UserWidget");
		pMouse->GetClickedWheelPressed().Unbind("UserWidget");
		pMouse->GetClickedWheelReleased().Unbind("UserWidget");
		pMouse->GetClickedWheelHeld().Unbind("UserWidget");
		pMouse->GetMouseMove().Unbind("UserWidget");
	}
	pMouse = nullptr;

	pRootSlate->ClearChildren();
	pRootSlate.reset();
	pRootSlate = nullptr;

	pOwner.reset();
	pOwner = nullptr;
}

void UserWidget::Draw()
{
	pRootSlate->Draw();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void UserWidget::Tick(DirectX11& dx, float deltaTime)
{
	if (IsInViewport())
	{
		ExecuteTasks(dx);

		for (auto&& animation : mAnimations)
		{
			animation.Update(deltaTime);
		}
	}
}
void UserWidget::AddToViewport(double inZOrder)
{
	ZOrder = Math::MapRange<double>(inZOrder, 0.0, 100.0, 0.0, 1.0);

	bIsInViewport = true;
}
void UserWidget::RemoveFromParent()
{
	bIsInViewport = false;
}
bool UserWidget::IsInViewport() const noexcept
{
	return bIsInViewport;
}
double UserWidget::GetZOrder() const noexcept
{
	return ZOrder;
}
std::shared_ptr<World> UserWidget::GetWorld()
{
	if (pOwner)
	{
		return pOwner->GetWorld();
	}
}

ID2D1RenderTarget* UserWidget::GetRt2D()
{
	return pRt2D;
}

// --------------------------
// Main : Animation
// --------------------------
void UserWidget::AddAnimation(WidgetAnimation in)
{
	mAnimations.Add(in);
}

// --------------------------
// Main : Input
// --------------------------
void UserWidget::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	NativeOnMouseMove(inMouseEvent);
};
void UserWidget::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	NativeOnMouseButtonDown(inMouseEvent);
};
void UserWidget::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	NativeOnMouseButtonHeld(inMouseEvent);
};
void UserWidget::OnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	NativeOnMouseButtonUp(inMouseEvent);
};
void UserWidget::OnMouseEnter(DX::MouseEvent inMouseEvent)
{
	NativeOnMouseEnter(inMouseEvent);
};
void UserWidget::OnMouseLeave(DX::MouseEvent inMouseEvent)
{
	NativeOnMouseLeave(inMouseEvent);
};
void UserWidget::OnKeyDown(DX::MouseEvent inMouseEvent)
{
	NativeOnKeyDown(inMouseEvent);
};
void UserWidget::OnKeyUp(DX::MouseEvent inMouseEvent)
{
	NativeOnKeyUp(inMouseEvent);
};

bool UserWidget::NativeOnMouseMove(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnMouseMove(inMouseEvent);
};
bool UserWidget::NativeOnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnMouseButtonDown(inMouseEvent);
};
bool UserWidget::NativeOnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnMouseButtonHeld(inMouseEvent);
};
bool UserWidget::NativeOnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnMouseButtonUp(inMouseEvent);
};
bool UserWidget::NativeOnMouseEnter(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnMouseEnter(inMouseEvent);
};
bool UserWidget::NativeOnMouseLeave(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnMouseLeave(inMouseEvent);
};
bool UserWidget::NativeOnKeyDown(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnKeyDown(inMouseEvent);
};
bool UserWidget::NativeOnKeyUp(DX::MouseEvent inMouseEvent)
{
	return pRootSlate->OnKeyUp(inMouseEvent);
};

// --------------------------
// 
// --------------------------
DirectX::XMMATRIX UserWidget::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity() * DirectX::XMMatrixTranslation(0, 0, (float)ZOrder);
}