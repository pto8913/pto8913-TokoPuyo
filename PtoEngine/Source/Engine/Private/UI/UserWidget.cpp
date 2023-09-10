
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
	SetLayer(Layer::EActorLayer::UI);
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
	if (!IsPendingKill())
	{
		pRootSlate->Draw();
	}
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void UserWidget::Tick(DirectX11& dx, float deltaTime)
{
	if (!IsPendingKill())
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
World* UserWidget::GetWorld()
{
	if (IsValid(pOwner))
	{
		return pOwner->GetWorld();
	}
	return nullptr;
}

ID2D1RenderTarget* UserWidget::GetRt2D()
{
	return pRt2D;
}

void UserWidget::AddSlate(std::shared_ptr<SlateBase> inSlate)
{
	if (!IsPendingKill())
	{
		pRootSlate->AddChild(inSlate);
		pRootSlate->UpdateWidget();
	}
}
void UserWidget::RemoveSlate(std::shared_ptr<SlateBase> inSlate)
{
	if (!IsPendingKill())
	{
		pRootSlate->RemoveChild(inSlate);
		pRootSlate->UpdateWidget();
	}
}

// --------------------------
// Main : Animation
// --------------------------
void UserWidget::AddAnimation(WidgetAnimation in)
{
	if (!IsPendingKill())
	{
		mAnimations.Add(in);
	}
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
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnMouseMove(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnMouseButtonDown(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnMouseButtonHeld(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnMouseButtonUp(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnMouseEnter(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnMouseEnter(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnMouseLeave(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnMouseLeave(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnKeyDown(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnKeyDown(inMouseEvent);
		}
	}
	return false;
};
bool UserWidget::NativeOnKeyUp(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill())
	{
		if (pRootSlate != nullptr)
		{
			return pRootSlate->OnKeyUp(inMouseEvent);
		}
	}
	return false;
};

// --------------------------
// 
// --------------------------
DirectX::XMMATRIX UserWidget::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity() * DirectX::XMMatrixTranslation(0, 0, (float)ZOrder);
}