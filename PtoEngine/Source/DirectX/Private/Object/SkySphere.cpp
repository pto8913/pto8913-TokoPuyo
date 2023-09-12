
#include "Object/SkySphere.h"

#include "Core/DirectX.h"

#include "Geometry/Sphere.h"

#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/ConstantBuffer.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"
#include "Render/InputLayout.h"
#include "Render/Texture.h"
#include "Render/CubeTexture.h"
#include "Render/Topology.h"
#include "Render/Rasterizer.h"
#include "Render/DepthstencilState.h"
#include "Render/SamplerState.h"
#include "Render/TransformConstantBuffer.h"

using namespace DirectX;

const std::wstring SHADERPATH = L"Shader/Shader.hlsl";

SkySphere::SkySphere(DirectX11& dx, float radius)
{
	auto model = CreateSphere(10, 10);

	//auto model = Sphere::Make();
	//model.SetTransform(DirectX::XMMatrixScaling(radius, radius, radius));

	pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pIndexBuffer = IndexBuffer::Make(dx, "SkySphere", model.indices);
	pVertexBuffer = VertexBuffer<DX::FVertex>::Make(dx, "SkySphere", model.vertices);

	pTCB = std::make_shared<TransformConstantBuffer>(dx, 0);

	AddTask(CubeTexture::Make(dx, L"Content/Textures/skymap.dds"));
	AddTask(SamplerState::Make(dx));

	auto pVS = VertexShader::Make(dx, SHADERPATH, "SKYMAP_VS");
	AddTask(InputLayout::Make(dx, DX::Layout::VertexType::V3D, pVS.get()));
	AddTask(std::move(pVS));
	AddTask(PixelShader::Make(dx, SHADERPATH, "SKYMAP_PS"));

	AddTask(DepthStencilState::Make(dx, DepthStencilState::DSSType::SkyMap));

	AddTask(pTCB);

	AddTask(Rasterizer::Make(dx, Rasterizer::RasterizerType::None, (UINT)model.indices.size()));

	InitializeTasks();
}
DirectX::XMMATRIX SkySphere::GetTransformXM(DirectX11& dx) const noexcept
{
	return XMMatrixIdentity() * 
		XMMatrixScaling(5.0f, 5.0f, 5.0f) *
		DirectX::XMMatrixTranslation(
			DirectX::XMVectorGetX(dx.GetCameraLocation()),
			DirectX::XMVectorGetY(dx.GetCameraLocation()),
			DirectX::XMVectorGetZ(dx.GetCameraLocation())
		);
}
void SkySphere::ExecuteTasks(DirectX11& dx)
{
	DrawableObject3D::ExecuteTasks(dx);

	dx.GetContext()->OMSetDepthStencilState(NULL, 0);
}

TriangleList SkySphere::CreateSphere(UINT LatLines, UINT LongLines)
{
	UINT NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	UINT NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	TriangleList Out;
	Out.vertices.resize(NumSphereVertices);
	Out.indices.resize(NumSphereFaces * 3);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	Out.vertices[0].Pos.x = 0.0f;
	Out.vertices[0].Pos.y = 0.0f;
	Out.vertices[0].Pos.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14f / (LatLines - 1));
		DirectX::XMMATRIX Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28f / (LongLines));
			DirectX::XMMATRIX Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			Out.vertices[i * LongLines + j + 1].Pos.x = XMVectorGetX(currVertPos);
			Out.vertices[i * LongLines + j + 1].Pos.y = XMVectorGetY(currVertPos);
			Out.vertices[i * LongLines + j + 1].Pos.z = XMVectorGetZ(currVertPos);
		}
	}

	Out.vertices[NumSphereVertices - 1].Pos.x = 0.0f;
	Out.vertices[NumSphereVertices - 1].Pos.y = 0.0f;
	Out.vertices[NumSphereVertices - 1].Pos.z = -1.0f;

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		Out.indices[k] = 0;
		Out.indices[k + 1] = l + 1;
		Out.indices[k + 2] = l + 2;
		k += 3;
	}

	Out.indices[k] = 0;
	Out.indices[k + 1] = LongLines;
	Out.indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			Out.indices[k] = i * LongLines + j + 1;
			Out.indices[k + 1] = i * LongLines + j + 2;
			Out.indices[k + 2] = (i + 1) * LongLines + j + 1;

			Out.indices[k + 3] = (i + 1) * LongLines + j + 1;
			Out.indices[k + 4] = i * LongLines + j + 2;
			Out.indices[k + 5] = (i + 1) * LongLines + j + 2;

			k += 6; // next quad
		}

		Out.indices[k] = (i * LongLines) + LongLines;
		Out.indices[k + 1] = (i * LongLines) + 1;
		Out.indices[k + 2] = ((i + 1) * LongLines) + LongLines;

		Out.indices[k + 3] = ((i + 1) * LongLines) + LongLines;
		Out.indices[k + 4] = (i * LongLines) + 1;
		Out.indices[k + 5] = ((i + 1) * LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		Out.indices[k] = NumSphereVertices - 1;
		Out.indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		Out.indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	Out.indices[k] = NumSphereVertices - 1;
	Out.indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	Out.indices[k + 2] = NumSphereVertices - 2;

	return Out;
}