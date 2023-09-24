#pragma once

#include "Graphics/DirectXHead.h"
#include "Bindable.h"

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
