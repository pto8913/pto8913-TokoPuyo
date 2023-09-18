
#include "UI/UserWidget.h"

#include "Math/Math.h"

#include "Framework/World.h"

UserWidget::UserWidget(Object* inOwner, ID2D1RenderTarget* inRt2D, DirectX11& dx, DX::IMouseInterface* mouse)
	: pOwner(inOwner), pRt2D(inRt2D), pDX(&dx), pMouse(mouse)
{
	SetTickEnabled(true);

	//NativeOnInitialized();

	SetLayer(Layer::EActorLayer::UI);
}
UserWidget::~UserWidget()
{
	{
		auto iter = mAnimations.begin();
		while (iter != mAnimations.end())
		{
			auto& obj = *iter;
			obj.Deactivate();
			obj.Clear();
			iter = mAnimations.erase(iter);
		}
		mAnimations.clear();
	}
	{
		auto iter = mAnimationProps.begin();
		while (iter != mAnimationProps.end())
		{
			auto& obj = *iter;
			delete obj;
			obj = nullptr;
			iter = mAnimationProps.end();
		}
		mAnimationProps.clear();
	}

	if (pMouse)
	{
		pMouse->GetClickedLeftPressed().Unbind(GetName() + "_Left");
		pMouse->GetClickedLeftReleased().Unbind(GetName() + "_Left");
		pMouse->GetClickedLeftHeld().Unbind(GetName() + "_Left");
		pMouse->GetClickedRightPressed().Unbind(GetName() + "_Right");
		pMouse->GetClickedRightReleased().Unbind(GetName() + "_Right");
		pMouse->GetClickedRightHeld().Unbind(GetName() + "_Right");
		pMouse->GetClickedWheelPressed().Unbind(GetName() + "_Wheel");
		pMouse->GetClickedWheelReleased().Unbind(GetName() + "_Wheel");
		pMouse->GetClickedWheelHeld().Unbind(GetName() + "_Wheel");
		pMouse->GetMouseMove().Unbind(GetName() + "_Move");
	}
	pMouse = nullptr;
	if (pRootSlate != nullptr)
	{
		pRootSlate->ClearChildren();
	}
	pRootSlate.reset();
	pRootSlate = nullptr;

	pRt2D = nullptr;
	pDX = nullptr;

	pOwner = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void UserWidget::NativeOnInitialized()
{
	if (pMouse)
	{
		pMouse->GetClickedLeftPressed().Bind<&UserWidget::OnMouseButtonDown>(*this, GetName() + "_Left");
		pMouse->GetClickedLeftReleased().Bind<&UserWidget::OnMouseButtonUp>(*this, GetName() + "_Left");
		pMouse->GetClickedLeftHeld().Bind<&UserWidget::OnMouseButtonHeld>(*this, GetName() + "_Left");

		pMouse->GetClickedRightPressed().Bind<&UserWidget::OnMouseButtonDown>(*this, GetName() + "_Right");
		pMouse->GetClickedRightReleased().Bind<&UserWidget::OnMouseButtonUp>(*this, GetName() + "_Right");
		pMouse->GetClickedRightHeld().Bind<&UserWidget::OnMouseButtonHeld>(*this, GetName() + "_Right");

		pMouse->GetClickedWheelPressed().Bind<&UserWidget::OnMouseButtonDown>(*this, GetName() + "_Wheel");
		pMouse->GetClickedWheelReleased().Bind<&UserWidget::OnMouseButtonUp>(*this, GetName() + "_Wheel");
		pMouse->GetClickedWheelHeld().Bind<&UserWidget::OnMouseButtonHeld>(*this, GetName() + "_Wheel");

		pMouse->GetMouseMove().Bind<&UserWidget::OnMouseMove>(*this, GetName() + "_Move");
	}
}
void UserWidget::NativeConstruct()
{

}
void UserWidget::NativeDestruct()
{

}
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
	NativeConstruct();
}
void UserWidget::RemoveFromParent()
{
	NativeDestruct();
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
		mAnimations.push_back(in);
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