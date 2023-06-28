#pragma once

#include "DirectX/DirectXHead.h"
#include "Geometry/TriangleList.h"

#include "StepTimer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectXTex.lib")

class ViewPort;

class DrawPlane;

class Camera;
class InputLayout;

class DirectXInput;

class RenderTargetView;
class DepthStencilView;

struct cbPerObject
{
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;
};
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	DirectX::XMFLOAT3 direction;
	float pad;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};
struct Light_Point : public Light
{
	Light_Point()
	{
		ZeroMemory(this, sizeof(Light_Point));
	}

	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 attenuation;
	float pad2;
};
struct cbPerFrame
{
	Light light;
};

class DirectX11
{
	friend class DirectXResource;
public:
	DirectX11(HINSTANCE hInstance, HWND hWnd, int width, int height);
	~DirectX11();

	DirectX11(const DirectX11& CopyC) = delete;
	DirectX11& operator=(const DirectX11& CopyC) = delete;

	HINSTANCE const& GetHInstance() const;
	HWND const& GetHWnd() const;

	void CleanUp();

	// Basic game loop
	void Tick();

	virtual void Clear();

	virtual void Update(DX::StepTimer const& timer);
	virtual void Present();

	void DrawIndexed(UINT count);

	bool bInitialized = false;

	DirectX::XMMATRIX GetCameraView() const;
	DirectX::XMMATRIX GetCameraProjection() const;
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	
	// Device resources.
	int m_outputWidth;
	int m_outputHeight;

	DX::StepTimer m_timer;

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pID3DDevice;
	ID3D11DeviceContext* m_pID3DContext;

	std::shared_ptr<RenderTargetView> m_pRenderTargetView;
	std::shared_ptr<DepthStencilView> m_pDepthStencilView;

	cbPerFrame constbPerFrame;
	Light light;

	DrawPlane* m_pDrawPlane;

	DirectXInput* m_pDirectXInput;
	
	Camera* m_pCamera;

	ViewPort* m_pViewPort;
	std::shared_ptr<class PixelShader> lightShader;

	// ---------------------------------------
	TriangleList modelPlane;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11ShaderResourceView* m_pTextureView;
	std::shared_ptr<class PixelShader> pixelShader;
	std::shared_ptr<class VertexShader> vertexShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pConstantBufferPerFrame;
	ID3D11SamplerState* m_pSamplerState;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;
	ID3D11BlendState* m_pBlendState;
	std::shared_ptr<class Topology> topology;
	cbPerObject cbPerObj;
	DirectX::XMMATRIX plane;
	// ---------------------------------------
public:
	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();

protected:
	virtual void OnDeviceLost();

	template<typename VertexType>
	void FreshPic(
		ID3D11Buffer* pIVertexBuffer,
		float picWidth, float picHeight,
		int rowNum, int colNum, int currentRow, int currentCol
	);

public:
	// ------------------------------------------------------
	// Window
	// ------------------------------------------------------
	void GetDefaultSize(int& width, int& height) const noexcept;
	virtual void OnWindowSizeChanged(int width, int height);

protected:

	// ------------------------------------------------------
	// Resource
	// ------------------------------------------------------
	void CreateResources();
};


template<typename VertexType>
void DirectX11::FreshPic(
	ID3D11Buffer* pIVertexBuffer,
	float picWidth, float picHeight,
	int rowNum, int colNum, int currentRow, int currentCol
)
{
	float width = picWidth / colNum;
	float height = picHeight / rowNum;

	float widthU = 1.f / colNum;
	float heightV = 1.f / rowNum;

	DirectX::XMFLOAT2A uvLeftTop = DirectX::XMFLOAT2A(widthU * currentCol, heightV * currentRow);
	DirectX::XMFLOAT2A uvRightBottom = DirectX::XMFLOAT2A(uvLeftTop.x + widthU, uvLeftTop.y + heightV);

	D3D11_MAPPED_SUBRESOURCE mapResource;
	HRESULT result = m_pID3DContext->Map(
		pIVertexBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapResource
	);

	if (FAILED(result))
	{
		assert(false);
	}

	VertexType* spritePtr = (VertexType*)mapResource.pData;
	spritePtr[0].pos = DirectX::XMFLOAT3(width, height, 1.0f);
	spritePtr[1].pos = DirectX::XMFLOAT3(width, 0, 1.0f);
	spritePtr[2].pos = DirectX::XMFLOAT3(0, 0, 1.0f);
	spritePtr[3].pos = DirectX::XMFLOAT3(0, 0, 1.0f);
	spritePtr[4].pos = DirectX::XMFLOAT3(0, height, 1.0f);
	spritePtr[5].pos = DirectX::XMFLOAT3(width, height, 1.0f);

	spritePtr[0].tex0 = DirectX::XMFLOAT2(uvRightBottom.x, uvLeftTop.y);
	spritePtr[1].tex0 = DirectX::XMFLOAT2(uvRightBottom.x, uvRightBottom.y);
	spritePtr[2].tex0 = DirectX::XMFLOAT2(uvLeftTop.x, uvRightBottom.y);
	spritePtr[3].tex0 = DirectX::XMFLOAT2(uvLeftTop.x, uvRightBottom.y);
	spritePtr[4].tex0 = DirectX::XMFLOAT2(uvLeftTop.x, uvLeftTop.y);
	spritePtr[5].tex0 = DirectX::XMFLOAT2(uvRightBottom.x, uvLeftTop.y);

	float halfWidth = picWidth / 2.0f;
	float halfHeight = picHeight / 2.0f;

	m_pID3DContext->Unmap(pIVertexBuffer, 0);
}