
#include "Object/DrawScreenText.h"

#include "Core/DirectX.h"

#include "Math/Vertex.h"

#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/VertexBuffer.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/InputLayout.h"
#include "Render/Factory/Topology.h"

#include "Render/Factory/BlendState.h"
#include "Render/Factory/SamplerState.h"

#include "Render/Factory/ScreenText.h"

#include "Render/Factory/Rasterizer.h"

DrawScreenText::DrawScreenText(DirectX11& dx, UINT inWidth, UINT inHeight)
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
	m_pIndexBuffer = IndexBuffer::Make(dx, "DrawScreenText", indices);
	m_pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, "DrawScreenText", vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));

	AddTask(m_pIndexBuffer);
	AddTask(m_pVertexBuffer);

	m_pScreenText = ScreenText::Make(dx, inWidth, inHeight);
	AddTask(m_pScreenText);

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, m_pIndexBuffer.get()->GetCount()));

	m_pTCB = std::make_shared<TransformConstantBuffer>(dx);
	m_pTCB->InitParentRefrence(*this);

	InitializeTasks();
}
void DrawScreenText::UpdateText(std::wstring inText)
{
	text = inText;
}
void DrawScreenText::ExecuteTasks(DirectX11& dx)
{
	dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);

	m_pTCB->Bind(dx, tf);
	m_pScreenText->Bind(text);

	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX DrawScreenText::GetTransformXM(DirectX11&) const noexcept
{
    return DirectX::XMMatrixIdentity();
}