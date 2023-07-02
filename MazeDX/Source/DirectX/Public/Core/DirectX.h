#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectXTex.lib")

#include "DirectX/DirectXHead.h"
#include "Geometry/TriangleList.h"

#include "StepTimer.h"

class ViewPort;

class DrawPlane;
class DrawSphere;
class ScreenText;

class Camera;

class DirectXInput;

class RenderTargetView;
class DepthStencilView;

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
	DirectX11(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height);
	~DirectX11();

	DirectX11(const DirectX11& CopyC) = delete;
	DirectX11& operator=(const DirectX11& CopyC) = delete;

	HINSTANCE const& GetHInstance() const;
	HWND const& GetHWnd() const;

	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
	ID3D11Device* GetDevice() { return m_pID3DDevice; }
	ID3D11DeviceContext* GetContext() { return m_pID3DContext; }

	void CleanUp();

	// Basic game loop
	void Tick();

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
	UINT m_outputWidth;
	UINT m_outputHeight;

	DX::StepTimer m_timer;

	IDXGISwapChain* m_pSwapChain;
	ID3D11Device* m_pID3DDevice;
	ID3D11DeviceContext* m_pID3DContext;

	std::shared_ptr<RenderTargetView> m_pRenderTargetView;
	std::shared_ptr<DepthStencilView> m_pDepthStencilView;

	cbPerFrame constbPerFrame;
	Light light;

	DrawPlane* m_pDrawPlane;
	DrawSphere* m_pDrawSphere;
	ScreenText* m_pScreenText;

	DirectXInput* m_pDirectXInput;
	
	Camera* m_pCamera;

	ViewPort* m_pViewPort;
	std::shared_ptr<class PixelShader> lightShader;

	// ---------------------------------------
	std::shared_ptr<class PixelShader> pixelShader;
	std::shared_ptr<class VertexShader> vertexShader;
	ID3D11Buffer* m_pConstantBufferEx;
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