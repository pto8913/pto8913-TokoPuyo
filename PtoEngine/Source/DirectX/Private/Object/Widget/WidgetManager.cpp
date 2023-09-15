
#include "Object/Widget/WidgetManager.h"

#include "Render/IndexBuffer.h"

#include "Render/VertexShader.h"
#include "Render/PixelShader.h"

#include "Render/InputLayout.h"
#include "Render/Topology.h"

#include "Render/ScreenText.h"
#include "Render/BlendState.h"
#include "Render/SamplerState.h"

#include "Render/Rasterizer.h"
#include "Render/TransformConstantBuffer.h"

#include "EngineSettings.h"

WidgetManager::~WidgetManager()
{

}

WidgetManager& WidgetManager::Get() {
	static WidgetManager instance;
	return instance;
}

void WidgetManager::Init(DirectX11& dx)
{
	if (!bIsInitialized)
	{
		pBlendState = BlendState::Make(dx, 0);

		//Compile Shaders from shader file
		pVertexShader = VertexShader::Make(dx, L"Shader/DrawScreenText.hlsl", "VS");
		pPixelShader = PixelShader::Make(dx, L"Shader/DrawScreenText.hlsl", "PS");

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

		pInputLayout = InputLayout::Make(dx, DX::Layout::VertexType::V2D, pVertexShader.get());

		pTCB = std::make_shared<TransformConstantBuffer>(dx);
		//AddTask(pTCB);

		const auto windowSize = EngineSettings::GetWindowSize();
		pScreenTextOnlyOutput = ScreenTextOnlyOutput::Make(dx, windowSize.x, windowSize.y);
		pRt2D = pScreenTextOnlyOutput->GetRT2D();
		pMutex11 = pScreenTextOnlyOutput->GetMutex11();
		pMutex10 = pScreenTextOnlyOutput->GetMutex10();

		pSamplerState = SamplerState::Make(dx, 0);

		pRasterizer = Rasterizer::Make(dx, Rasterizer::RasterizerType::Transparent2, pIndexBuffer.get()->GetCount());

		bIsInitialized = true;
	}
}
bool WidgetManager::IsInitialized() const
{
	return bIsInitialized;
}