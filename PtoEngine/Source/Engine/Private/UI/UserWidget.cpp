
#include "UI/UserWidget.h"

#include "Math/Math.h"

#include "Framework/World.h"

UserWidget::UserWidget(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse)
	: pOwner(inOwner), pMouse(mouse)
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
	if (pRootSlate != nullptr)
	{
		pRootSlate->ClearChildren();
	}
	pRootSlate.reset();
	pRootSlate = nullptr;

	pOwner = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void UserWidget::Tick(DirectX11& dx, float deltaTime)
{
	if (!IsPendingKill() && IsInViewport())
	{
		if (IsInViewport())
		{
			Draw();

			for (auto&& animation : mAnimations)
			{
				animation.Update(deltaTime);
			}
		}
	}
}
void UserWidget::Draw()
{
	if (pRootSlate != nullptr)
	{
		pRootSlate->Draw();
	}
}

void UserWidget::SetTickEnabled(bool inState) noexcept
{
	bIsInViewport = inState;
}
bool UserWidget::GetTickEnabled() const noexcept
{
	return bIsInViewport;
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
	if (!IsPendingKill() && IsInViewport())
	{
		mAnimations.Add(in);
	}
}

// --------------------------
// Main : Input
// --------------------------
void UserWidget::OnMouseMove(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnMouseMove(inMouseEvent);
	}
};
void UserWidget::OnMouseButtonDown(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnMouseButtonDown(inMouseEvent);
	}
};
void UserWidget::OnMouseButtonHeld(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnMouseButtonHeld(inMouseEvent);
	}
};
void UserWidget::OnMouseButtonUp(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnMouseButtonUp(inMouseEvent);
	}
};
void UserWidget::OnMouseEnter(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnMouseEnter(inMouseEvent);
	}
};
void UserWidget::OnMouseLeave(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnMouseLeave(inMouseEvent);
	}
};
void UserWidget::OnKeyDown(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnKeyDown(inMouseEvent);
	}
};
void UserWidget::OnKeyUp(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
	{
		NativeOnKeyUp(inMouseEvent);
	}
};

bool UserWidget::NativeOnMouseMove(DX::MouseEvent inMouseEvent)
{
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
	if (!IsPendingKill() && IsInViewport())
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
//DirectX::XMMATRIX UserWidget::GetTransformXM(DirectX11&) const noexcept
//{
//	return DirectX::XMMatrixIdentity() * DirectX::XMMatrixTranslation(0, 0, (float)ZOrder);
//}