#pragma once

#include "Object/DrawableObject.h"

class Camera;

class IndexBuffer;
class VertexBuffer;
class Topology;

class VertexShader;
class PixelShader;
class ConstantBufferPerFrame;

class SkySphere : public DrawableObject
{
public:
	SkySphere(DirectX11& dx, int latDiv, int longDiv);
	~SkySphere();

	void Update(Camera* pCamera);
	//void Draw(
	//	ID3D11DeviceContext* pID3DContext, 
	//	Camera* pCamera, 
	//	cbPerObject& cbPerObj, 
	//	ID3D11Buffer** pConstantBuffer,
	//	ID3D11SamplerState** pSamplerState
	//);
private:
	std::shared_ptr<IndexBuffer> m_pIndexBuffer;
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;
	std::shared_ptr<ConstantBufferPerFrame> m_pConstantBuffer;
	std::shared_ptr<Topology> m_pTopology;

	VertexShader* m_pVS_SkyMap;
	PixelShader* m_pPS_SkyMap;

	ID3D11Resource* m_pSkyMapTexture;
	ID3D11ShaderResourceView* m_pShaderResourceView;
	
	ID3D11DepthStencilState* m_pDSLessEqual;
	ID3D11RasterizerState* m_pRSCullNone;

	ID3D11SamplerState* m_pSamplerState;

	int NumSphereVertices;
	int NumSphereFaces;

	DirectX::XMMATRIX m_sphereWorld;

	DirectX::XMMATRIX Scale;
	DirectX::XMMATRIX Translation;
};