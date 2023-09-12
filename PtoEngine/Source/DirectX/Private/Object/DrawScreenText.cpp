
#include "Object/DrawScreenText.h"

#include "Core/DirectX.h"

#include "Math/Vertex.h"

#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"

#include "Render/PixelShader.h"
#include "Render/VertexShader.h"

#include "Render/InputLayout.h"
#include "Render/Topology.h"

#include "Render/BlendState.h"
#include "Render/SamplerState.h"

#include "Render/ScreenText.h"

#include "Render/Rasterizer.h"

DrawScreenText::DrawScreenText(DirectX11& dx, float inWidth, float inHeight)
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
	pIndexBuffer = IndexBuffer::Make(dx, "DrawScreenText", indices);
	pVertexBuffer = VertexBuffer<DX::FVertex2D>::Make(dx, "DrawScreenText", vertices);
	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVS.get()));

	AddTask(pIndexBuffer);
	AddTask(pVertexBuffer);

	pScreenText = ScreenText::Make(dx, inWidth, inHeight);
	AddTask(pScreenText);

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, pIndexBuffer.get()->GetCount()));

	pTCB = std::make_shared<TransformConstantBuffer>(dx);
	pTCB->InitParentRefrence(*this);

	InitializeTasks();
}
DrawScreenText::~DrawScreenText()
{
	pTCB.reset();
	pTCB = nullptr;

	pScreenText.reset();
	pScreenText = nullptr;
}

void DrawScreenText::UpdateText(std::wstring inText, D2D1_RECT_F inRect)
{
	text = inText;
	rect = inRect;
}
void DrawScreenText::ExecuteTasks(DirectX11& dx)
{
	dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);

	pTCB->Bind(dx, tf);
	pScreenText->Bind(text, rect);

	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX DrawScreenText::GetTransformXM(DirectX11&) const noexcept
{
    return DirectX::XMMatrixIdentity();
}