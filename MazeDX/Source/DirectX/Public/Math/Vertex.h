#pragma once

#include "DirectX/DirectXHead.h"

//struct FVertex 
//{
//public:
//	FVertex()
//		: Pos(0, 0, 0), Color(0, 0, 0, 1) {};
//	FVertex(float x, float y, float z)
//		: Pos(x, y, z), Color(0, 0, 0, 1) {};
//	FVertex(float x, float y, float z, float r, float g, float b, float a = 1)
//		: Pos(x, y, z), Color(r, g, b, a) {};
//
//	DirectX::XMFLOAT3 Pos;
//	DirectX::XMFLOAT4 Color;
//};

struct FVertex
{
public:
	FVertex()
		: Pos(0, 0, 0), texCoord(0, 0), Normal(0, 1, 0) {};
	FVertex(float x, float y, float z)
		: Pos(x, y, z), texCoord(0, 0), Normal(0, 1, 0) {};
	FVertex(float x, float y, float z, float u, float v)
		: Pos(x, y, z), texCoord(u, v), Normal(0, 1, 0) {};
	FVertex(float u, float v)
		: Pos(0, 0, 0), texCoord(u, v), Normal(0, 1, 0) {};
	FVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		: Pos(x, y, z), texCoord(u, v), Normal(nx, ny, nz) {};

	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 Normal;
};

struct Point 
{
public:
	float X, Y, Z, Rhw;
	DWORD Diffuse;
};

#define LAYOUT_ELEMENT_TYPES \
	X(Position3D) \
	X(Texture2D) \
	X(Normal) \
	X(Count)

namespace DX
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			#define X(el) el,
			LAYOUT_ELEMENT_TYPES
			#undef X
		};

		template<template<VertexLayout::ElementType> class F, typename... Args>
		static constexpr auto Bridge(VertexLayout::ElementType type, Args&&... args)
		{
			switch (type)
			{
				#define X(el) case VertexLayout::el: return F<VertexLayout::el>::Exec(std::forward<Args>(args)...);
				LAYOUT_ELEMENT_TYPES
				#undef X
			}
			return F<VertexLayout::Count>::Exec(std::forward<Args>(args)...);
		}

		template<ElementType> struct Map;
		template<> 
		struct Map<Position3D>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* segmentic = "Position";
			static constexpr const char* code = "P3";
		};
		template<>
		struct Map<Normal>
		{
			using SysType = DirectX::XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* segmentic = "Normal";
			static constexpr const char* code = "N";
		};
		template<>
		struct Map<Texture2D>
		{
			using SysType = DirectX::XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* segmentic = "TexCoord";
			static constexpr const char* code = "T2";
		};
		template<>
		struct Map<Count>
		{
			using SysType = long double;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;
			static constexpr const char* segmentic = "!INVALID!";
			static constexpr const char* code = "!INV!";
		};

		class Element
		{
		public:
			Element(ElementType inType, size_t inOffset);

			ElementType GetType() const noexcept;
			size_t GetOffset() const noexcept;
			size_t Size() const;
			size_t GetOffsetAfter() const;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const;
			const char* GetCode() const noexcept;
		private:
			ElementType type;
			size_t offset;
		};

		size_t Size() const;
		const Element& Get(size_t i) const;
		VertexLayout& Append(ElementType type);
		bool Has(ElementType type) const noexcept;
		size_t GetElementCount() const noexcept;
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const;
		std::string GetCode() const;

		template<ElementType Type>
		const Element& Make() const
		{
			for (auto& e : elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			return elements.front();
		}
	private:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexBuffer;
	public:
		Vertex(char* inpData, VertexLayout& inLayot);

		template<VertexLayout::ElementType Type>
		auto& Attr()
		{
			auto pAttribute = pData + layout.Make<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		template<typename T>
		void SetAttributeByIndex(size_t idx, T&& val)
		{
			const auto& element = layout.Get(idx);
			auto pAttribute = pData + element.GetOffset();
			VertexLayout::Bridge<AttributeSetting>(
				element.GetType(), this, pAttribute, std::forward<T>(val)
			);
		}
	private:
		template<VertexLayout::ElementType Type>
		struct AttributeSetting
		{
			template<typename T>
			static constexpr auto Exec(Vertex* pVertex, char* pAttribute, T&& args)
			{
				return pVertex->SetAttribute<Type>(pAttribute, std::forward<T>(args));
			}
		};
		template<typename F, typename... Args>
		void SetAttributeByIndex(size_t i, F&& first, Args&&... args)
		{
			SetAttributeByIndex(i, std::forward<F>(first));
			SetAttributeByIndex(i + 1, std::forward<Args>(args)...);
		}
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val)
		{
			if (pAttribute)
			{
				using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
				if constexpr (std::is_assignable<Dest, SrcType>::value)
				{
					*reinterpret_cast<Dest*>(pAttribute) = val;
				}
				else
				{
					MessageBox(NULL, L"Parameter Attribute type mismatch", L"Create VertexLayout Error", MB_OK);
					//assert(false);
				}
			}
			else
			{
				MessageBox(NULL, L"Parameter Attribute is not valid" , L"Create VertexLayout Error", MB_OK);
				//assert(false);
			}
		}

		char* pData;
		VertexLayout layout;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(VertexLayout inLayout, size_t size = 0);
		size_t Size() const;
		size_t SizeBytes() const;
		void Resize(size_t newSize);
		Vertex Back();
		const char* GetData() const;
		const VertexLayout& GetLayout() const noexcept;

		Vertex operator[](size_t i)
		{
			return Vertex{ buffer.data() + layout.Size() * i, layout };
		}

		template<typename ...Params>
		void EmplaceBack(Params&&... params)
		{
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
};