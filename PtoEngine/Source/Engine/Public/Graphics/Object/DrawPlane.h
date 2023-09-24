#pragma once

#include "DrawableObject3D.h"

class DirectX11;

class DrawPlane : public DrawableObject3D
{
public:
	DrawPlane(
		DirectX11& dx, 
		float sizeX,
		float sizeZ,
		std::wstring inFileName = L"Content/Textures/grass.jpg", 
		std::wstring inVShaderName = L"Shader/Shader.hlsl",
		std::wstring inPShaderName = L"Shader/Shader.hlsl"
	);
};