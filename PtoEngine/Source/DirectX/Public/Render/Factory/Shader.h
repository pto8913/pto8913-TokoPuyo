#pragma once

#include "DirectX/DirectXHead.h"

#include "Render/Factory/Bindable.h"

#pragma comment(lib, "d3dcompiler.lib")

class Shader : public Bindable
{
protected:
	bool Compile(
		std::wstring pFilename, 
		const char* pEntryPoint, 
		const char* pShaderModel, 
		ID3DBlob** ppBlob
	);
};
