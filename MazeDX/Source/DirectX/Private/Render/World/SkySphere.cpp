
#include "Render/World/SkySphere.h"

#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/VertexBuffer.h"
#include "Render/Factory/Topology.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/ConstantBuffer.h"
#include "Render/Factory/InputLayout.h"

#include "Render/Factory/SamplerState.h"

#include "Render/Factory/Rasterizer.h"

#include "Render/Factory/CubeTexture.h"

#include "Math/Vertex.h"

#include "DirectX/WICTextureLoader/WICTextureLoader11.h"
#include "DirectX/DDSTextureLoader/DDSTextureLoader11.h"

#include "Input/Camera.h"

#include "Geometry/Sphere.h"

using namespace DirectX;

const std::wstring SHADERPATH = L"Shader/SkyMap.hlsl";

SkySphere::SkySphere(DirectX11& dx, int latDiv, int longDiv)
{
	NumSphereVertices = (latDiv - 2) * longDiv + 2;
	NumSphereFaces = ((latDiv - 3) * longDiv * 2) + (longDiv * 2);

	TriangleList model = Sphere::Make();

	m_pIndexBuffer = IndexBuffer::Make(dx, "Sky", model.indices);
	m_pVertexBuffer = VertexBuffer::Make(dx, "Sky", model.vertices);
	m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto pVS = VertexShader::Make(dx, SHADERPATH, "SKYMAP_VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, SHADERPATH, "SKYMAP_PS"));

	//struct PSColorConstant
	//{
	//	DirectX::XMFLOAT3 color = { 1,1,1 };
	//	float padding;
	//} colorConst;
	//AddTask(PixelConstantBuffer<PSColorConstant>::Makedx, colorConst, 1));
	AddTask(ConstantBufferEx::Make(dx, sizeof(cbPerObject), 0));

	AddTask(Rasterizer::Make(dx, Rasterizer::None, m_pIndexBuffer->GetCount()));

	const wchar_t* TexturePaths[6] = {
		L"directx.ico",
		L"directx.ico",
		L"directx.ico",
		L"directx.ico",
		L"directx.ico",
		L"directx.ico",
	};

	std::vector<ScratchImage> pTextures(6);
	std::vector<ID3D11ShaderResourceView*> pTexResourceViews(6);
	HRESULT result;
	for (int i = 0; i < 6; ++i)
	{
		const wchar_t* path = TexturePaths[i];
		result = DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, pTextures[i]);
		if (FAILED(result))
		{
			MessageBox(NULL, L"Can not Load Texture", L"Failed Texture", MB_OK);
			assert(false);
		}
	}

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = (UINT)pTextures[0].GetMetadata().width;
	texDesc.Height = (UINT)pTextures[0].GetMetadata().height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA data[6];
	for (int i = 0; i < 6; ++i)
	{
		data[i].pSysMem = pTextures[i].GetPixels();
		data[i].SysMemPitch = (UINT)pTextures[i].GetImage(0, 0, 0)->rowPitch;
		data[i].SysMemSlicePitch = 0;
	}

	//ID3D11Texture2D* pTexture;
	//pID3DDevice->CreateTexture2D(&texDesc, data, &pTexture);

	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//ZeroMemory(&srvDesc, sizeof(srvDesc));
	//srvDesc.Format = texDesc.Format;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.Texture2D.MipLevels = 1;

	//pID3DDevice->CreateShaderResourceView(pTexture, &srvDesc, &m_pShaderResourceView);

	//CreateDDSTextureFromFileEx(
	//	pID3DDevice, 
	//	pID3DContext, 
	//	L"skymap.dds", 
	//	0, 
	//	D3D11_USAGE_DEFAULT,
	//	D3D11_BIND_SHADER_RESOURCE, 
	//	D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
	//	D3D11_RESOURCE_MISC_TEXTURECUBE, 
	//	DDS_LOADER_FORCE_SRGB, 
	//	&m_pSkyMapTexture, 
	//	&m_pShaderResourceView
	//);

	//D3D11_SAMPLER_DESC desc;
	//ZeroMemory(&desc, sizeof(desc));
	//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//desc.MinLOD = 0;
	//desc.MaxLOD = D3D11_FLOAT32_MAX;

	//result = pID3DDevice->CreateSamplerState(
	//	&desc,
	//	&m_pSamplerState
	//);
	//if (FAILED(result))
	//{
	//	MessageBox(NULL, L"Can not create SampleState.", L"Create SampleState Error", MB_OK);
	//	assert(false);
	//}

	//D3D11_RASTERIZER_DESC cmdesc;
	//cmdesc.FillMode = D3D11_FILL_SOLID;
	//cmdesc.CullMode = D3D11_CULL_NONE;
	//cmdesc.FrontCounterClockwise = false;
	//pID3DDevice->CreateRasterizerState(&cmdesc, &m_pRSCullNone);

	//D3D11_DEPTH_STENCIL_DESC dssdesc;
	//ZeroMemory(&dssdesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//dssdesc.DepthEnable = true;
	//dssdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dssdesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	//pID3DDevice->CreateDepthStencilState(&dssdesc, &m_pDSLessEqual);
}
SkySphere::~SkySphere()
{
	Util::SafeDelete(m_pIndexBuffer.get());
	Util::SafeDelete(m_pVertexBuffer.get());

	Util::SafeDelete(m_pVS_SkyMap);
	Util::SafeDelete(m_pPS_SkyMap);

	Util::SafeRelease(m_pSkyMapTexture);
	Util::SafeRelease(m_pShaderResourceView);

	Util::SafeRelease(m_pDSLessEqual);
	Util::SafeRelease(m_pRSCullNone);
}

void SkySphere::Update(Camera* pCamera)
{
	m_sphereWorld = XMMatrixIdentity();
	Scale = XMMatrixScaling(5.f, 5.f, 5.f);
	Translation = XMMatrixTranslationFromVector(
		pCamera->GetLocation()
	);
	m_sphereWorld = Scale * Translation;
}
//
//void SkySphere::Draw(ID3D11DeviceContext* pID3DContext, Camera* pCamera, cbPerObject& cbPerObj, ID3D11Buffer** pConstantBuffer, ID3D11SamplerState** pSamplerState)
//{
//	pID3DContext; pCamera; cbPerObj; pConstantBuffer; pSamplerState;
//	//m_pIndexBuffer->Bind(dx);
//	//pID3DContext->IASetIndexBuffer(m_pIndexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
//
//	//m_pVertexBuffer->Bind(*dxInstance);
//
//	//UINT stride = sizeof(FVertex);
//	//UINT offset = 0;
//	//pID3DContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer->GetBuffer(), &stride, &offset);
//
//	//XMMATRIX WVP = m_sphereWorld * pCamera->GetView() + pCamera->GetProjection();
//	//cbPerObj.WVP = XMMatrixTranspose(WVP);
//	//cbPerObj.World = XMMatrixTranspose(m_sphereWorld);
//
//	//pID3DContext->UpdateSubresource(*pConstantBuffer, 0, NULL, &cbPerObj, 0, 0);
//	//pID3DContext->VSSetConstantBuffers(0, 1, &*pConstantBuffer);
//
//	//pID3DContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
//	//pID3DContext->PSSetSamplers(0, 1, &*pSamplerState);
//
//	//pID3DContext->VSSetShader(m_pVS_SkyMap->GetShader(), 0, 0);
//	//pID3DContext->PSSetShader(m_pPS_SkyMap->GetShader(), 0, 0);
//
//	//pID3DContext->OMSetDepthStencilState(m_pDSLessEqual, 0);
//	//pID3DContext->RSSetState(m_pRSCullNone);
//	//pID3DContext->DrawIndexed(NumSphereFaces * 3, 0, 0);
//}
