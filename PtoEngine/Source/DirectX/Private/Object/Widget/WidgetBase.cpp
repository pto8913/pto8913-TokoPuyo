
#include "Object/Widget/WidgetBase.h"

#include "Core/DirectX.h"

#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"

#include "Render/PixelShader.h"
#include "Render/VertexShader.h"

#include "Render/InputLayout.h"
#include "Render/Topology.h"

#include "Render/ScreenText.h"
#include "Render/BlendState.h"
#include "Render/SamplerState.h"

#include "Render/Rasterizer.h"

#include "Object/Widget/WidgetManager.h"

WidgetBase::WidgetBase(DirectX11& dx, float windowSizeW, float windowSizeH)
{
	WidgetManager& widgetManager = WidgetManager::Get();

	widgetManager.Init(dx);
	AddTask(widgetManager.pBlendState);

	//Compile Shaders from shader file
	AddTask(widgetManager.pPixelShader);
	AddTask(widgetManager.pVertexShader);

	pIndexBuffer = widgetManager.pIndexBuffer;
	pVertexBuffer = widgetManager.pVertexBuffer;
	pTopology = widgetManager.pTopology;

	AddTask(widgetManager.pInputLayout);

	AddTask(pIndexBuffer);
	AddTask(pVertexBuffer);

	pTCB = widgetManager.pTCB;
	pTCB->InitParentRefrence(*this);
	//AddTask(pTCB);

	auto pScreenTextOnlyOutput = widgetManager.pScreenTextOnlyOutput;
	pRt2D = widgetManager.pRt2D;
	pMutex11 = widgetManager.pMutex11;
	pMutex10 = widgetManager.pMutex10;
	AddTask(pScreenTextOnlyOutput);

	AddTask(widgetManager.pSamplerState);

	AddTask(widgetManager.pRasterizer);

	InitializeTasks();
}
WidgetBase::~WidgetBase()
{
	//pRt2D->Release();
	Util::SafeRelease(pMutex11);
	Util::SafeRelease(pMutex10);
}

void WidgetBase::DrawInternal(DirectX11& dx)
{
	pMutex11->ReleaseSync(0);
	pMutex10->AcquireSync(0, 5);

	pRt2D->BeginDraw();
	pRt2D->Clear();

	Draw();

	pRt2D->EndDraw();

	pMutex10->ReleaseSync(1);
	pMutex11->AcquireSync(1, 5);
}

void WidgetBase::ExecuteTasks(DirectX11& dx)
{
	//dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);

	pTCB->Bind(dx, tf);

	DrawInternal(dx);

	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX WidgetBase::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity() * DirectX::XMMatrixTranslation(0, 0, 0);
}
