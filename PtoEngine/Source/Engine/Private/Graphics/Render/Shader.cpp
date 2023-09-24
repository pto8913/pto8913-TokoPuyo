
#include "Graphics/Render/Shader.h"

bool Shader::Compile(std::wstring pFilename, const char* pEntryPoint, const char* pShaderModel, ID3DBlob** ppBlob)
{
	WCHAR path[256];
	size_t len = 0;

	size_t outputSize = pFilename.length() + 1;
	char* outputStr = new char[outputSize];

	size_t charsConverted = 0;

	wcstombs_s(&charsConverted, outputStr, outputSize, pFilename.c_str(), pFilename.length());
	mbstowcs_s(&len, path, 256, outputStr, _TRUNCATE);

	ID3DBlob* pErrorMsg;
	HRESULT hr = D3DCompileFromFile(
		path,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		pEntryPoint,
		pShaderModel,
		0,
		0,
		ppBlob,
		&pErrorMsg
	);
	if (FAILED(hr))
	{
		static_cast<char*>(pErrorMsg->GetBufferPointer());
		MessageBox(NULL, L"Compile Failed", L"Shader Compile Error", MB_OK);
		Util::SafeRelease(pErrorMsg);
		assert(false);
		return false;
	}

	Util::SafeRelease(pErrorMsg);
	return true;
}