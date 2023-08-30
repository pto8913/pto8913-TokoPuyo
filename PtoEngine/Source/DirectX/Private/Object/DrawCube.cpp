
#include "Object/DrawCube.h"

#include "Geometry/TriangleList.h"
#include "Geometry/Cube.h"

#include "Render/Factory/VertexBuffer.h"
#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/Topology.h"

DrawCube::DrawCube(DirectX11& dx, float size)
{
	dx;
	size;
	//TriangleList model = Cube::Make(size);
	//
	//m_pVertexBuffer = VertexBuffer::Make(dx, "Cube", model.vertices);
	//m_pIndexBuffer = IndexBuffer::Make(dx, "Cube", model.indices);
	//m_pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}