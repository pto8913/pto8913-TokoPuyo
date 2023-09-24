
#include "Graphics/Render/TransformConstantBuffer.h"
#include "Graphics/Object/DrawableObject.h"
#include "Graphics/Render/BindableManager.h"

TransformConstantBuffer::TransformConstantBuffer(DirectX11& dx, UINT inSlot)
	: slot(inSlot)
{
	if (!pVCBuffer)
	{
		pVCBuffer = std::make_unique<VertexConstantBuffer<TransformConstantBuffer::Transforms>>(dx, inSlot);
	}
}
TransformConstantBuffer::~TransformConstantBuffer()
{
	pVCBuffer.reset();
	pVCBuffer = nullptr;

	pParent = nullptr;
}
std::shared_ptr<TransformConstantBuffer> TransformConstantBuffer::Make(DirectX11& dx, UINT inSlot)
{
	return BindableManager::Make<TransformConstantBuffer>(dx, inSlot);
}
void TransformConstantBuffer::InitParentRefrence(const DrawableObject& inParent)
{
	pParent = &inParent;
}
void TransformConstantBuffer::Bind(DirectX11& dx)
{
	if (pVCBuffer != nullptr)
	{
		pVCBuffer->Update(GetContext(dx), GetTransform(dx));
		pVCBuffer->Bind(dx);
	}
}
void TransformConstantBuffer::Bind(DirectX11& dx, TransformConstantBuffer::Transforms transform)
{
	if (pVCBuffer != nullptr)
	{
		pVCBuffer->Update(GetContext(dx), transform);
		pVCBuffer->Bind(dx);
	}
}
TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransform(DirectX11& dx)
{
	const DirectX::XMMATRIX model = pParent->GetTransformXM(dx);
	return {
		DirectX::XMMatrixTranspose(model * dx.GetCameraView() * dx.GetCameraProjection()),
		DirectX::XMMatrixTranspose(model)
	};
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::pVCBuffer = nullptr;
