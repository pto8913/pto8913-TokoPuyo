
#include "Graphics/Render/Vertex.h"

namespace DX
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> Layout::GetVertexDesc(const DX::Layout::VertexType& type)
	{
		using F3 = DirectX::XMFLOAT3;
		using F2 = DirectX::XMFLOAT2;

		std::vector<D3D11_INPUT_ELEMENT_DESC> Out;
		switch (type)
		{
		case V2D:
			Out = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                       D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(F3),              D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			//MessageBox(NULL, Util::s2WString(std::string("Size :") + std::to_string(sizeof(F3))).c_str(), L"Desc", MB_OK);

			break;
		default:
			Out = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                       D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(F3),              D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(F3) + sizeof(F2), D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			//MessageBox(NULL, Util::s2WString(std::string("Size F3 :") + std::to_string(sizeof(F3)) + std::string(" F2 :") + std::to_string(sizeof(F2))).c_str(), L"Desc", MB_OK);

			break;
		}
		return Out;
	}
}