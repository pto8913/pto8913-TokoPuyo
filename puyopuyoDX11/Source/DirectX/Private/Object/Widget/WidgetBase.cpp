
#include "Object/Widget/WidgetBase.h"

#include "Core/DirectX.h"

#include "Config.h"

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

WidgetBase::WidgetBase(DirectX11& dx)
{
	m_ClearColor = D2D1::ColorF(D2D1::ColorF::Black);

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

	m_pScreenTextOnlyOutput = ScreenTextOnlyOutput::Make(dx, (UINT)Config::windowSize.x, (UINT)Config::windowSize.y);
	AddTask(m_pScreenTextOnlyOutput);
	m_pRt2D = m_pScreenTextOnlyOutput->GetRT2D();
	m_pMutex11 = m_pScreenTextOnlyOutput->GetMutex11();
	m_pMutex10 = m_pScreenTextOnlyOutput->GetMutex10();

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, m_pIndexBuffer.get()->GetCount()));

	m_pTCB = std::make_shared<TransformConstantBuffer>(dx);
	m_pTCB->InitParentRefrence(*this);

	InitializeTasks();
}
void WidgetBase::DrawInternal()
{
	m_pMutex11->ReleaseSync(0);
	m_pMutex10->AcquireSync(0, 5);

	m_pRt2D->BeginDraw();
	m_pRt2D->Clear(m_ClearColor);

	Draw();

	m_pRt2D->EndDraw();

	m_pMutex10->ReleaseSync(1);
	m_pMutex11->AcquireSync(1, 5);
}

void WidgetBase::ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	pContext->OMSetBlendState(0, 0, 0xffffffff);

	m_pTCB->Bind(dx, pContext, tf);

	DrawInternal();

	DrawableObject::ExecuteTasks(dx, pContext);
}

void WidgetBase::SetVisibility(bool inVisible)
{
	DrawableObject2D::SetVisibility(inVisible);
}
bool WidgetBase::GetVisibility()
{
	return DrawableObject2D::GetVisibility();
}

DirectX::XMMATRIX WidgetBase::GetTransformXM(DirectX11&) const noexcept
{
	return DirectX::XMMatrixIdentity();
}

