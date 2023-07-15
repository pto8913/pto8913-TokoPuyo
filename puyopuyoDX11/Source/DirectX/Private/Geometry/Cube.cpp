
#include "Geometry/Cube.h"

//TriangleList Cube::Make(float size)
//{
//	TriangleList Out;
//	Out.vertices.resize(8);
//
//	Out.vertices[0].Pos = { -size,-size,-size };
//	Out.vertices[1].Pos = {  size,-size,-size };
//	Out.vertices[2].Pos = { -size, size,-size };
//	Out.vertices[3].Pos = {  size, size,-size };
//	Out.vertices[4].Pos = { -size,-size, size };
//	Out.vertices[5].Pos = {  size,-size, size };
//	Out.vertices[6].Pos = { -size, size, size };
//	Out.vertices[7].Pos = {  size, size, size };
//
//	Out.indices.clear();
//	Out.indices	= {
//		0,2,1,
//		2,3,1,
//
//		1,3,5,
//		3,7,5,
//
//		2,6,3,
//		3,6,7,
//
//		4,5,7,
//		4,7,6,
//
//		0,4,2,
//		2,4,6,
//
//		0,1,4,
//		1,5,4,
//	};
//
//	return Out;
//}