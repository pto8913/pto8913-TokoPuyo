
#include "Render/Viewport/ScreenText.h"

#include "Core/DirectX.h"

#include "Math/Vertex.h"

#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/VertexBuffer.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/InputLayout.h"

#include "Render/Factory/BlendState.h"
#include "Render/Factory/SamplerState.h"

#include "Render/Factory/Texture2D.h"

#include "Render/Factory/Rasterizer.h"

ScreenText::ScreenText(DirectX11& dx, UINT inWidth, UINT inHeight)
{
	AddTask(BlendState::Make(dx, 0));

	//dx.GetContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	//Create the vertex buffer
	std::vector<DX::FVertex> vertices =
	{
		DX::FVertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		DX::FVertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		DX::FVertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		DX::FVertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
	};

	std::vector<DWORD> indices = {
		0,  1,  2,
		0,  2,  3,
	};
	AddTask(IndexBuffer::Make(dx, "ScreenText", indices));
	AddTask(VertexBuffer::Make(dx, "ScreenText", vertices));

	m_pTexture2D = Texture2D::Make(dx, inWidth, inHeight);
	AddTask(m_pTexture2D);

	AddTask(SamplerState::Make(dx, 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, (UINT)indices.size()));

	//Compile Shaders from shader file
	auto pD2DVS = VertexShader::Make(dx, L"Shader/ScreenText.hlsl", "VS");
	pD2DVS->Bind(dx);

	auto pD2DPS = PixelShader::Make(dx, L"Shader/ScreenText.hlsl", "PS");
	pD2DPS->Bind(dx);

	auto pD2DIL = InputLayout::Make(dx, DX::Layout::VertexType::V2D, pD2DVS.get());
	pD2DIL->Bind(dx);

	//Set Primitive Topology
	//dx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pTCB = std::make_shared<TransformConstantBuffer>(dx);
	m_pTCB->InitParentRefrence(*this);
}

void ScreenText::UpdateText(std::wstring inText)
{
    text = inText;
}
void ScreenText::ExecuteTasks(DirectX11& dx)
{
	dx.GetContext()->OMSetBlendState(0, 0, 0xffffffff);
	
	m_pTCB->Bind(dx, tf);
	m_pTexture2D->Bind(dx, text);

	//Use the shader resource representing the direct2d render target
	//to texture a square which is rendered in screen space so it
	//overlays on top of our entire scene. We use alpha blending so
	//that the entire background of the D2D render target is "invisible",
	//And only the stuff we draw with D2D will be visible (the text)

	//Set the blend state for D2D render target texture objects
	DrawableObject::ExecuteTasks(dx);
}

DirectX::XMMATRIX ScreenText::GetTransformXM() const noexcept
{
    return DirectX::XMMatrixIdentity();
}