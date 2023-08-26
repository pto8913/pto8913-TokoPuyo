
#include "Object/Widget/WidgetBase.h"

#include "Core/DirectX.h"

#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/VertexBuffer.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/InputLayout.h"
#include "Render/Factory/Topology.h"

#include "Render/Factory/ScreenText.h"
#include "Render/Factory/BlendState.h"
#include "Render/Factory/SamplerState.h"

#include "Render/Factory/Rasterizer.h"

WidgetBase::WidgetBase(DirectX11& dx, UINT windowSizeW, UINT windowSizeH)
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
	m_pIndexBuffer = IndexBuffer::Make(dx, "WidgetBase", indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, "WidgetBase", vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));

	AddTask(m_pIndexBuffer);
	AddTask(m_pVertexBuffer);

	m_pTCB = std::make_shared<TransformConstantBuffer>(dx);
	m_pTCB->InitParentRefrence(*this);
	//AddTask(m_pTCB);

	auto pScreenTextOnlyOutput = ScreenTextOnlyOutput::Make(dx, windowSizeW, windowSizeH);
	pRt2D = pScreenTextOnlyOutput->GetRT2D();
	pMutex11 = pScreenTextOnlyOutput->GetMutex11();
	pMutex10 = pScreenTextOnlyOutput->GetMutex10();
	AddTask(pScreenTextOnlyOutput);

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::Transparent2, m_pIndexBuffer->GetCount()));

	InitializeTasks();
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

	m_pTCB->Bind(dx, tf);

	DrawInternal(dx);

	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX WidgetBase::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity() * DirectX::XMMatrixTranslation(0, 0, 0);
}
