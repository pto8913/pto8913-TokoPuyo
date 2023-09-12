
#include "Object/DrawCube.h"

#include "Geometry/TriangleList.h"
#include "Geometry/Cube.h"

#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/Topology.h"

DrawCube::DrawCube(DirectX11& dx, float size)
{
	dx;
	size;
	//TriangleList model = Cube::Make(size);
	//
	//pVertexBuffer = VertexBuffer::Make(dx, "Cube", model.vertices);
	//pIndexBuffer = IndexBuffer::Make(dx, "Cube", model.indices);
	//pTopology = Topology::Make(dx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}