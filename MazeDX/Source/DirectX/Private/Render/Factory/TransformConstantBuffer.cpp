
#include "Render/Factory/TransformConstantBuffer.h"
#include "Object/DrawableObject.h"

TransformConstantBuffer::TransformConstantBuffer(DirectX11& dx, UINT slot)
{
	if (!m_pVCBuffer)
	{
		m_pVCBuffer = std::make_unique<VertexConstantBuffer<TransformConstantBuffer::Transforms>>(dx, slot);
	}
}
void TransformConstantBuffer::InitParentRefrence(const DrawableObject& pParent)
{
	m_pParent = &pParent;
}
void TransformConstantBuffer::Bind(DirectX11& dx)
{
	Update(dx, GetTransform(dx));
}
void TransformConstantBuffer::Update(DirectX11& dx, const TransformConstantBuffer::Transforms& newTransform)
{
	m_pVCBuffer->Update(dx, newTransform);
	m_pVCBuffer->Bind(dx);
}
TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransform(DirectX11& dx)
{
	const auto model = m_pParent->GetWorldTransformXM();
	return {
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model * dx.GetCameraView() * dx.GetCameraProjection()
		)
	};
}
std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_pVCBuffer;
