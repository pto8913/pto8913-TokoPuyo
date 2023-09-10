
#include "Render/TransformConstantBuffer.h"
#include "Object/DrawableObject.h"
#include "Render/BindableManager.h"

TransformConstantBuffer::TransformConstantBuffer(DirectX11& dx, UINT inSlot)
	: slot(inSlot)
{
	if (!m_pVCBuffer)
	{
		m_pVCBuffer = std::make_unique<VertexConstantBuffer<TransformConstantBuffer::Transforms>>(dx, inSlot);
	}
}
std::shared_ptr<TransformConstantBuffer> TransformConstantBuffer::Make(DirectX11& dx, UINT inSlot)
{
	return BindableManager::Make<TransformConstantBuffer>(dx, inSlot);
}
void TransformConstantBuffer::InitParentRefrence(const DrawableObject& pParent)
{
	m_pParent = &pParent;
}
void TransformConstantBuffer::Bind(DirectX11& dx)
{
	m_pVCBuffer->Update(GetContext(dx), GetTransform(dx));
	m_pVCBuffer->Bind(dx);
}
void TransformConstantBuffer::Bind(DirectX11& dx, TransformConstantBuffer::Transforms transform)
{
	m_pVCBuffer->Update(GetContext(dx), transform);
	m_pVCBuffer->Bind(dx);
}
TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransform(DirectX11& dx)
{
	const DirectX::XMMATRIX model = m_pParent->GetTransformXM(dx);
	return {
		DirectX::XMMatrixTranspose(model * dx.GetCameraView() * dx.GetCameraProjection()),
		DirectX::XMMatrixTranspose(model)
	};
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_pVCBuffer;
