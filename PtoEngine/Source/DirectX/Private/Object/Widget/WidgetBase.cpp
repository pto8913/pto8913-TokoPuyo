
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

WidgetBase::WidgetBase(DirectX11& dx, float windowSizeW, float windowSizeH)
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
	pIndexBuffer = IndexBuffer::Make(dx, "WidgetBase", indices);
	pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, "WidgetBase", vertices);
	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));

	AddTask(pIndexBuffer);
	AddTask(pVertexBuffer);

	pTCB = std::make_shared<TransformConstantBuffer>(dx);
	pTCB->InitParentRefrence(*this);
	//AddTask(pTCB);

	auto pScreenTextOnlyOutput = ScreenTextOnlyOutput::Make(dx, windowSizeW, windowSizeH);
	pRt2D = pScreenTextOnlyOutput->GetRT2D();
	pMutex11 = pScreenTextOnlyOutput->GetMutex11();
	pMutex10 = pScreenTextOnlyOutput->GetMutex10();
	AddTask(pScreenTextOnlyOutput);

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, pIndexBuffer.get()->GetCount()));

	InitializeTasks();
}
WidgetBase::~WidgetBase()
{
	pRt2D->Release();
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
