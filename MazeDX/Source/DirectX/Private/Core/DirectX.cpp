
#include "Core/DirectX.h"

#include "Math/Vertex.h"

#include "DirectX/WICTextureLoader/WICTextureLoader11.h"

#include "Input/DirectXInput.h"

#include "Render/Factory/VertexBuffer.h"
#include "Render/Factory/IndexBuffer.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/InputLayout.h"

#include "Render/Factory/SamplerState.h"
#include "Render/Factory/BlendState.h"

#include "Render/Factory/DepthStencilView.h"
#include "Render/Factory/RenderTargetView.h"

#include "Render/Factory/ConstantBuffer.h"
#include "Render/Factory/TransformConstantBuffer.h"
#include "Render/Factory/Topology.h"

#include "Render/Viewport/ViewPort.h"

#include "Geometry/Plane.h"

#include "Render/World/SkySphere.h"

#include "Input/Camera.h"

#include "Object/DrawPlane.h"

#include <vector>

const std::wstring SHADERPATH = L"Shader/Shader.hlsl";

using namespace DirectX;

DirectX11::DirectX11(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	bInitialized = true;

	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_outputWidth = (int)max(width, 1);
	m_outputHeight = (int)max(height, 1);

	CreateResources();

	m_pDirectXInput = new DirectXInput(m_hInstance, m_hWnd);
	m_pCamera = new Camera((float)m_outputWidth, (float)m_outputHeight);

	m_pDrawPlane = new DrawPlane(*this, 100);
	m_pDrawPlane->SetWorldLocation(XMFLOAT3(0, 10, 0));
	m_pDrawPlane->SetWorldScale(XMMatrixScaling(500, 10, 500));

	/* Constant Buffer Per Frame */
	{
		light.direction = XMFLOAT3(0, -1, 0);
		light.ambient = XMFLOAT4(0.2, 0.2, 0.2, 1);
		light.diffuse = XMFLOAT4(1, 1, 1, 1);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(cbPerFrame);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		std::string str = std::string("ConstantBuffer Size :") + std::to_string(desc.ByteWidth);
		MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBuffer Error", MB_OK);

		HRESULT result = m_pID3DDevice->CreateBuffer(
			&desc,
			NULL,
			&m_pConstantBufferPerFrame
		);

		if (FAILED(result))
		{
			MessageBox(NULL, L"Can not create ConstantBufferPerFrame", L"Failed ConstantBufferPerFrame", MB_OK);
			assert(false);
		}
	}
 
	{
		modelPlane = Plane::Make();
		vertexShader = VertexShader::Make(*this, SHADERPATH, "VS");
		pixelShader = PixelShader::Make(*this, SHADERPATH, "PS");
		lightShader = PixelShader::Make(*this, SHADERPATH, "D2D_PS");

		vertexShader->Bind(*this);
		pixelShader->Bind(*this);
		lightShader->Bind(*this);

		/* Index Buffer */
		{
			std::vector<DWORD> indices = {
				0, 1, 2,
				0, 2, 3
			};

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ByteWidth = (UINT)indices.size() * sizeof(DWORD);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = sizeof(DWORD);

			std::string str = std::string("IndexBuffer Size :") + std::to_string(desc.ByteWidth);
			MessageBox(NULL, Util::s2WString(str).c_str(), L"IndexBuffer Error", MB_OK);

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = indices.data();

			if (FAILED(m_pID3DDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer)))
			{
				MessageBox(NULL, L"Can not create IndexBuffer", L"Failed IndexBuffer", MB_OK);
				assert(false);
			}
		}

		/* VertexBuffer */
		{
			std::vector<DX::FVertex> vertices = {
				DX::FVertex(-1.0f, -1.0f, -1.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f),
				DX::FVertex(1.0f, -1.0f, -1.0f,   0.0f, 100.0f, 0.0f, 1.0f, 0.0f),
				DX::FVertex(1.0f, -1.0f,  1.0f,   0.0f,   0.0f, 0.0f, 1.0f, 0.0f),
				DX::FVertex(-1.0f, -1.0f,  1.0f, 100.0f,   0.0f, 0.0f, 1.0f, 0.0f),
			};

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ByteWidth = (UINT)vertices.size() * sizeof(DX::FVertex);
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			std::string	str = std::string("VertexBuffer Size :") + std::to_string(desc.ByteWidth);
			MessageBox(NULL, Util::s2WString(str).c_str(), L"pVertexBuffer Error", MB_OK);

			D3D11_SUBRESOURCE_DATA vertexBufferData;
			ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
			vertexBufferData.pSysMem = vertices.data();

			HRESULT result = m_pID3DDevice->CreateBuffer(
				&desc,
				&vertexBufferData,
				&m_pVertexBuffer
			);

			if (FAILED(result))
			{
				MessageBox(NULL, L"can not create pVertexBuffer", L"pVertexBuffer Error", MB_OK);
				assert(false);
			}
		}

		/* Input Layout */
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			UINT numElements = ARRAYSIZE(layout);
			const auto pByteCode = vertexShader->GetBuffer();

			HRESULT result = m_pID3DDevice->CreateInputLayout(
				layout,
				numElements,
				pByteCode->GetBufferPointer(),
				pByteCode->GetBufferSize(),
				&m_pInputLayout
			);

			if (FAILED(result))
			{
				MessageBox(NULL, L"Can not create InputLayout.", L"Create InputLayout Error", MB_OK);
				assert(false);
			}
			m_pID3DContext->IASetInputLayout(m_pInputLayout);
		}

		/* Topology */
		m_pID3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		/* Viewport */
		{
			m_pViewPort = new ViewPort(*this, (float)m_outputWidth, (float)m_outputHeight);
			m_pViewPort->Bind(*this);
		}

		/* Texture */
		{
			ScratchImage scratch;
			std::wstring inFileName = L"grass.jpg";
			DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
			HRESULT result = DirectX::LoadFromWICFile(
				inFileName.c_str(),
				DirectX::WIC_FLAGS_IGNORE_SRGB,
				nullptr,
				scratch
			);
			if (FAILED(result))
			{
				MessageBox(NULL, L"Can not load Texture", L"Failed Texture constructor", MB_OK);
				assert(false);
			}

			if (scratch.GetImage(0, 0, 0)->format != format)
			{
				DirectX::ScratchImage converted;
				result = DirectX::Convert(
					*scratch.GetImage(0, 0, 0),
					format,
					DirectX::TEX_FILTER_DEFAULT,
					0.5,
					converted
				);
				if (FAILED(result))
				{
					MessageBox(NULL, L"Can not load Convert Texture", L"Failed Texture constructor", MB_OK);
					assert(false);
				}
				scratch = std::move(converted);
			}

			UINT t_width = (UINT)scratch.GetImage(0, 0, 0)->width;
			UINT t_height = (UINT)scratch.GetImage(0, 0, 0)->height;
			D3D11_TEXTURE2D_DESC texDesc;
			ZeroMemory(&texDesc, sizeof(texDesc));
			texDesc.Width = t_width;
			texDesc.Height = t_height;
			texDesc.MipLevels = 0;
			texDesc.ArraySize = 1;
			texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			ID3D11Texture2D* pTexture;
			m_pID3DDevice->CreateTexture2D(&texDesc, nullptr, &pTexture);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = texDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			m_pID3DDevice->CreateShaderResourceView(pTexture, &srvDesc, &m_pTextureView);
		}

		/* Constant Buffer */
		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.ByteWidth = sizeof(cbPerObject);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			std::string str = std::string("ConstantBuffer Size :") + std::to_string(desc.ByteWidth);
			MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBuffer Error", MB_OK);

			HRESULT result = m_pID3DDevice->CreateBuffer(
				&desc,
				NULL,
				&m_pConstantBuffer
			);

			if (FAILED(result))
			{
				MessageBox(NULL, L"Can not create ConstantBufferPerFrame", L"Failed ConstantBufferPerFrame", MB_OK);
				assert(false);
			}
		}

		/* Sampler State */
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			desc.MinLOD = 0;
			desc.MaxLOD = D3D11_FLOAT32_MAX;
			desc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

			HRESULT result = m_pID3DDevice->CreateSamplerState(
				&desc,
				&m_pSamplerState
			);
			if (FAILED(result))
			{
				MessageBox(NULL, L"Can not create SampleState.", L"Create SampleState Error", MB_OK);
				assert(false);
			}
		}

		/* Blend State */
		{
			D3D11_BLEND_DESC blendDesc;
			ZeroMemory(&blendDesc, sizeof(blendDesc));

			D3D11_RENDER_TARGET_BLEND_DESC rtbd;
			ZeroMemory(&rtbd, sizeof(rtbd));

			rtbd.BlendEnable = true;
			rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
			rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			rtbd.BlendOp = D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

			blendDesc.AlphaToCoverageEnable = false;
			blendDesc.RenderTarget[0] = rtbd;
			m_pID3DDevice->CreateBlendState(&blendDesc, &m_pBlendState);
		}

		/* Rasterizer */
		{
			D3D11_RASTERIZER_DESC cmdesc;

			ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
			cmdesc.FillMode = D3D11_FILL_SOLID;
			cmdesc.CullMode = D3D11_CULL_BACK;
			cmdesc.FrontCounterClockwise = true;
			m_pID3DDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

			cmdesc.FrontCounterClockwise = false;

			m_pID3DDevice->CreateRasterizerState(&cmdesc, &CWcullMode);

		}
		plane = XMMatrixScaling(500, 10, 500) * XMMatrixTranslation(0, 10, 0);
	}
}
DirectX11::~DirectX11()
{
	CleanUp();
}

HINSTANCE const& DirectX11::GetHInstance() const
{
	return m_hInstance;
}
HWND const& DirectX11::GetHWnd() const
{
	return m_hWnd;
}

void DirectX11::CleanUp()
{
	m_pSwapChain->SetFullscreenState(false, NULL);
	PostMessage(m_hWnd, WM_DESTROY, 0, 0);

	Util::SafeDelete(m_pDirectXInput);
	Util::SafeDelete(m_pCamera);
	Util::SafeDelete(m_pViewPort);

	Util::SafeRelease(m_pSwapChain);
	Util::SafeRelease(m_pID3DDevice);
	Util::SafeRelease(m_pID3DContext);
}

// Executes the basic game loop.
void DirectX11::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		}
	);
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() != 0)
	{
		Clear();
		// TODO: Add your rendering code here.

		Present();
	}
}

void DirectX11::Clear()
{
	// Clear the views.
	//float bgColor[4] = { 0.1, 0.1, 0.1, 1 };

	//m_pViewPort->CreateViewPort(m_pID3DContext, (float)m_outputWidth, (float)m_outputHeight);

	m_pRenderTargetView->Clear(*this);
	m_pDepthStencilView->Clear(*this);

	ID3D11ShaderResourceView* const pNullTexture = nullptr;
	m_pID3DContext->PSSetShaderResources(0, 1, &pNullTexture);
	m_pID3DContext->PSSetShaderResources(3, 1, &pNullTexture);
}

void DirectX11::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	m_pDirectXInput->Update(m_hWnd, elapsedTime, m_pCamera);
	m_pCamera->Update();
	plane = XMMatrixScaling(500, 10, 500) * XMMatrixTranslation(0, 10, 0);

	//m_pSkySphere->Update(m_pCamera);
}

void DirectX11::Present()
{
	// TODO: Add rendering logic here.

	m_pRenderTargetView->Clear(*this);
	m_pDepthStencilView->Clear(*this);

	/* Light */
	{
		constbPerFrame.light = light;
		m_pID3DContext->UpdateSubresource(m_pConstantBufferPerFrame, 0, NULL, &constbPerFrame, 0, 0);
		m_pID3DContext->PSSetConstantBuffers(0, 1, &m_pConstantBufferPerFrame);
	}

	m_pID3DContext->OMSetRenderTargets(1, &m_pRenderTargetView->Get(), m_pDepthStencilView->Get());

	//Set the default blend state (no blending) for opaque objects
	//m_pID3DContext->OMSetBlendState(m_pBlendState, 0, 0xffffffff);

	m_pDrawPlane->ExecuteTasks(*this);

	//{
	//	vertexShader->Bind(*this);
	//	pixelShader->Bind(*this);

	//	m_pID3DContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//	UINT stride = sizeof(FVertex);
	//	UINT offset = 0;
	//	m_pID3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//	DirectX::XMMATRIX WVP = plane * GetCameraView() * GetCameraProjection();
	//	cbPerObj.WVP = XMMatrixTranspose(WVP);
	//	cbPerObj.World = XMMatrixTranspose(plane);
	//	m_pID3DContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cbPerObj, 0, 0);
	//	m_pID3DContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//	m_pID3DContext->PSSetShaderResources(0, 1, &m_pTextureView);
	//	m_pID3DContext->PSSetSamplers(0, 1, &m_pSamplerState);
	//	m_pID3DContext->RSSetState(CCWcullMode);
	//	DrawIndexed(6);
	//}

	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT result = m_pSwapChain->Present(1, 0);
	
	//m_pViewPort->CreateViewPort(m_pID3DContext, (float)m_outputWidth, (float)m_outputHeight);

	// If the device was reset we must completely reinitialize the renderer.
	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		if (FAILED(result))
		{
			assert(false);
		}
	}
}

void DirectX11::DrawIndexed(UINT count)
{
	m_pID3DContext->DrawIndexed(count, 0, 0);
}

DirectX::XMMATRIX DirectX11::GetCameraView() const
{
	return m_pCamera->GetView();
}
DirectX::XMMATRIX DirectX11::GetCameraProjection() const
{
	return m_pCamera->GetProjection();
}

// Message handlers
void DirectX11::OnActivated()
{
	// TODO: DirectX11 is becoming active window.
}

void DirectX11::OnDeactivated()
{
	// TODO: DirectX11 is becoming background window.
}

void DirectX11::OnSuspending()
{
	// TODO: DirectX11 is being power-suspended (or minimized).
}

void DirectX11::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: DirectX11 is being power-resumed (or returning from minimize).
}

void DirectX11::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
	CleanUp();
	CreateResources();

	MessageBox(NULL, L"DeviceLost", L"Create DeviceLost Error", MB_OK);
}

// ------------------------------------------------------
// Window
// ------------------------------------------------------
void DirectX11::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}
void DirectX11::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);
}

// ------------------------------------------------------
// Resource
// ------------------------------------------------------
// Allocate all memory resources that change on a window SizeChanged event.
void DirectX11::CreateResources()
{
	//Describe our SwapChain Buffer
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = m_outputWidth;
	bufferDesc.Height = m_outputHeight;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};
	UINT totalFeatureLevels = ARRAYSIZE(featureLevels);

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		totalFeatureLevels,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pID3DDevice,
		nullptr,
		&m_pID3DContext
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not find Driver Type.", L"Create Swap Chain Error", MB_OK);
		assert(false);
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		&pBackBuffer
	);

	m_pRenderTargetView = std::shared_ptr<RenderTargetView>{ new RenderTargetView(*this, pBackBuffer.Get()) };
	m_pDepthStencilView = std::shared_ptr<DepthStencilView>{ new DepthStencilView(*this, m_outputWidth, m_outputHeight) };

	//m_pID3DContext->OMSetRenderTargets(1, &m_pRenderTargetView->Get(), m_pDepthStencilView->Get());

	// TODO: Initialize windows-size dependent objects here.
}