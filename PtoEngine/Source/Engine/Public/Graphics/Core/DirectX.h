#pragma once

#include "Graphics/DirectXHead.h"

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

	IDXGISwapChain* GetSwapChain() { return pSwapChain; }
	HINSTANCE GetHInstance() { return mhInstance; }
	HWND& GetHWnd() { return mhWnd; }

	// ------------------------------------------------------
	// Main : Device3D
	// ------------------------------------------------------
	ID3D11Device* GetDevice() { return pID3DDevice; }
	ID3D11DeviceContext* GetContext() { return pID3DContext; }

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

	std::shared_ptr<RenderTargetView> pRenderTargetView;
	std::shared_ptr<DepthStencilView> pDepthStencilView;
private:
	IDXGISwapChain* pSwapChain;
	HINSTANCE mhInstance;
	HWND mhWnd;

	// ------------------------------------------------------
	// State : Device3D
	// ------------------------------------------------------
	ID3D11Device* pID3DDevice;
	ID3D11DeviceContext* pID3DContext;

	// ------------------------------------------------------
	// State : Camera
	// ------------------------------------------------------
	DirectX::XMMATRIX CameraProjection;
	DirectX::XMMATRIX CameraView;
	DirectX::XMVECTOR CameraLocation;
	DirectX::XMVECTOR CameraRotation;
};