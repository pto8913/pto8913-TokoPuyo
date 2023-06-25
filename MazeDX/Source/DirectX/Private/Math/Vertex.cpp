
#include "Math/Vertex.h"

namespace DX
{
	size_t VertexLayout::Size() const
	{
		return elements.empty() ? 0u : elements.back().GetOffset();
	}
	const VertexLayout::Element& VertexLayout::Get(size_t i) const
	{
		return elements[i];
	}
	VertexLayout& VertexLayout::Append(ElementType type)
	{
		if (!Has(type))
		{
			elements.emplace_back(VertexLayout::Element(type, Size()));
		}
		return *this;
	}
	bool VertexLayout::Has(ElementType type) const noexcept
	{
		for (auto& e : elements)
		{
			if (e.GetType() == type)
			{
				return true;
			}
		}
		return false;
	}
	size_t VertexLayout::GetElementCount() const noexcept
	{
		return elements.size();
	}
	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(GetElementCount());
		for (const auto& e : elements)
		{
			desc.push_back(e.GetDesc());
		}
		return desc;
	}
	std::string VertexLayout::GetCode() const
	{
		std::string code;
		for (const auto& e : elements)
		{
			code += e.GetCode();
		}
		return code;
	}

	VertexLayout::Element::Element(ElementType inType, size_t inOffset)
		: type(inType), offset(inOffset)
	{
	}
	VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
	{
		return type;
	}
	size_t VertexLayout::Element::GetOffset() const noexcept
	{
		return offset;
	}
	size_t VertexLayout::Element::Size() const
	{
		return sizeof(type);
	}
	size_t VertexLayout::Element::GetOffsetAfter() const
	{
		return offset + Size();
	}
	template<VertexLayout::ElementType type>
	struct GenerateDesc {
		static constexpr D3D11_INPUT_ELEMENT_DESC Exec(size_t offset)
		{
			return {
				VertexLayout::Map<type>::segmentic,
				0,
				VertexLayout::Map<type>::dxgiFormat,
				0,
				(UINT)offset,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			};
		}
	};
	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
	{
		return Bridge<GenerateDesc>(type, GetOffset());
	}
	
	template<VertexLayout::ElementType type>
	struct CodeLookup
	{
		static constexpr auto Exec() noexcept
		{
			return DX::VertexLayout::Map<type>::code;
		}
	};
	const char* VertexLayout::Element::GetCode() const noexcept
	{
		return Bridge<CodeLookup>(type);
	}

	Vertex::Vertex(char* inpData, VertexLayout& inLayot)
		: pData(inpData), layout(inLayot)
	{
	}

	VertexBuffer::VertexBuffer(VertexLayout inLayout, size_t size)
		: layout(std::move(inLayout))
	{
		Resize(size);
	}
	size_t VertexBuffer::Size() const
	{
		size_t layoutSize = layout.Size();
		if (layoutSize == 0)
		{
			return buffer.size();
		}
		return buffer.size() / layout.Size();;
	}
	size_t VertexBuffer::SizeBytes() const
	{
		return buffer.size();
	}
	void VertexBuffer::Resize(size_t newSize)
	{
		const size_t size = Size();
		if (size < newSize)
		{
			buffer.resize(buffer.size() + layout.Size() * (newSize - size));
		}
	}
	Vertex VertexBuffer::Back()
	{
		return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
	}
	const char* VertexBuffer::GetData() const
	{
		return buffer.data();
	}
	const VertexLayout& VertexBuffer::GetLayout() const noexcept
	{
		return layout;
	}
}