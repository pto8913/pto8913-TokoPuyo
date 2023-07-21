#pragma once

#include "DirectX/DirectXHead.h"

class RenderTargetView;
class DepthStencilView;

class DirectX11
{
	friend class DirectXResource;
public:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------

	DirectX11(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height);
	~DirectX11();

	DirectX11(const DirectX11& CopyC) = delete;
	DirectX11& operator=(const DirectX11& CopyC) = delete;

	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
	// ------------------------------------------------------
	// Main : Device3D
	// ------------------------------------------------------
	ID3D11Device* GetDevice() { return m_pID3DDevice; }
	ID3D11DeviceContext* GetContext() { return m_pID3DContext; }
	ID3D11DeviceContext* GetDeferredContext() { return m_pID3DDeferredContext_1; }

	// ------------------------------------------------------
	// Main : Device2D
	// ------------------------------------------------------
	ID2D1Device* GetDevice2D() { return m_pID2DDevice; }
	ID2D1DeviceContext* GetContext2D() { return m_pID2DContext; }

	// ------------------------------------------------------
	// Main : Frame
	// ------------------------------------------------------
	void BeginFrame();
	HRESULT EndFrame();

	void DrawIndexed(UINT count);

	// ------------------------------------------------------
	// Main : Camera
	// ------------------------------------------------------
	void SetCameraView(const DirectX::XMMATRIX& InCameraView);
	DirectX::XMMATRIX GetCameraView() const;
	void SetCameraProjection(const DirectX::XMMATRIX& InCameraProjection);
	DirectX::XMMATRIX GetCameraProjection() const;
	void SetCameraLocation(const DirectX::XMVECTOR& InCameraLocation);
	DirectX::XMVECTOR GetCameraLocation() const;
	void SetCameraRotation(const DirectX::XMVECTOR& InCameraRotation);
	DirectX::XMVECTOR GetCameraRotation() const;

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	bool bInitialized = false;

	std::shared_ptr<RenderTargetView> m_pRenderTargetView;
	std::shared_ptr<DepthStencilView> m_pDepthStencilView;
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	IDXGISwapChain* m_pSwapChain;

	// ------------------------------------------------------
	// State : Device3D
	// ------------------------------------------------------
	ID3D11Device* m_pID3DDevice;
	ID3D11DeviceContext* m_pID3DContext;
	ID3D11DeviceContext* m_pID3DDeferredContext_1;

	// ------------------------------------------------------
	// State : Device2D
	// ------------------------------------------------------
	ID2D1Device* m_pID2DDevice;
	ID2D1DeviceContext* m_pID2DContext;

	// ------------------------------------------------------
	// State : Camera
	// ------------------------------------------------------
	DirectX::XMMATRIX CameraProjection;
	DirectX::XMMATRIX CameraView;
	DirectX::XMVECTOR CameraLocation;
	DirectX::XMVECTOR CameraRotation;
protected:

	template<typename VertexType>
	void FreshPic(
		ID3D11Buffer* pIVertexBuffer,
		float picWidth, float picHeight,
		int rowNum, int colNum, int currentRow, int currentCol
	);
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