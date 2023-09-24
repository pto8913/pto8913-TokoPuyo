
#include "UI/WidgetManager.h"
#include "UI/UserWidget.h"

#include "Graphics/Core/DirectX.h"

#include "Graphics/Render/IndexBuffer.h"
#include "Graphics/Render/VertexBuffer.h"

#include "Graphics/Render/PixelShader.h"
#include "Graphics/Render/VertexShader.h"

#include "Graphics/Render/InputLayout.h"
#include "Graphics/Render/Topology.h"

#include "Graphics/Render/ScreenText.h"
#include "Graphics/Render/BlendState.h"
#include "Graphics/Render/SamplerState.h"

#include "Graphics/Render/Rasterizer.h"

#include "EngineSettings.h"

WidgetManager::WidgetManager(DirectX11& dx)
{
	AddTask(BlendState::Make(dx, 0));

	//Compile Shaders from shader file
	auto pVS = VertexShader::Make(dx, L"Shader/DrawScreenText.hlsl", "VS");
	AddTask(PixelShader::Make(dx, L"Shader/DrawScreenText.hlsl", "PS"));
	AddTask(pVS);

	std::vector<DX::FVertex2D> vertices =
	{
		DX::FVertex2D(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		DX::FVertex2D(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		DX::FVertex2D(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		DX::FVertex2D(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	};

	std::vector<DWORD> indices = {
		0,  1,  2,
		0,  2,  3,
	};
	pIndexBuffer = IndexBuffer::Make(dx, "WidgetManager", indices);
	pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, "WidgetManager", vertices);
	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));

	AddTask(pIndexBuffer);
	AddTask(pVertexBuffer);

	pTCB = std::make_shared<TransformConstantBuffer>(dx);
	pTCB->InitParentRefrence(*this);
	//AddTask(pTCB);

	const auto widgetSize = EngineSettings::GetWindowSize();
	auto pScreenTextOnlyOutput = ScreenTextOnlyOutput::Make(dx, widgetSize.x, widgetSize.y);
	pRt2D = pScreenTextOnlyOutput->GetRT2D();
	pMutex11 = pScreenTextOnlyOutput->GetMutex11();
	pMutex10 = pScreenTextOnlyOutput->GetMutex10();
	AddTask(pScreenTextOnlyOutput);

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, pIndexBuffer.get()->GetCount()));

	InitializeTasks();
}
WidgetManager::~WidgetManager()
{
	//pRt2D->Release();
	Util::SafeRelease(pMutex11);
	Util::SafeRelease(pMutex10);

	auto iter = pWidgets.begin();
	while (iter != pWidgets.end())
	{
		auto& obj = *iter;
		if (obj != nullptr)
		{
			obj.reset();
			obj = nullptr;
			iter = pWidgets.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	pWidgets.clear();

	pRt2D = nullptr;
	pMutex11 = nullptr;
	pMutex10 = nullptr;
}
WidgetManager::operator bool() const noexcept
{
	return pWidgets.size() > 0;
}

void WidgetManager::RemovePendingObjects()
{
	auto iter = pWidgets.begin();
	while (iter != pWidgets.end())
	{
		auto& obj = *iter;
		if (obj != nullptr)
		{
			if (obj->IsPendingKill())
			{
				obj->EndPlay();
				obj.reset();
				obj = nullptr;
				iter = pWidgets.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		else
		{
			++iter;
		}
	}
}
void WidgetManager::Tick(DirectX11& dx, float deltaTime)
{
	RemovePendingObjects();

	//dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);
	pTCB->Bind(dx, tf);

	pMutex11->ReleaseSync(0);
	pMutex10->AcquireSync(0, 5);

	pRt2D->BeginDraw();
	pRt2D->Clear();

	for (auto&& widget : pWidgets)
	{
		widget->Tick(dx, deltaTime);
	}

	pRt2D->EndDraw();

	pMutex10->ReleaseSync(1);
	pMutex11->AcquireSync(1, 5);

	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX WidgetManager::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity() * DirectX::XMMatrixTranslation(0, 0, 0);
}