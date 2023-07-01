
#include "Render/Factory/TransformConstantBuffer.h"
#include "Object/DrawableObject.h"
#include "Render/Manager/BindableManager.h"

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
	m_pVCBuffer->Update(dx, GetTransform(dx));
	m_pVCBuffer->Bind(dx);
}
TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransform(DirectX11& dx)
{
	const DirectX::XMMATRIX model = m_pParent->GetTransformXM();
	return {
		DirectX::XMMatrixTranspose(model * dx.GetCameraView() * dx.GetCameraProjection()),
		DirectX::XMMatrixTranspose(model)
	};
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::m_pVCBuffer;
