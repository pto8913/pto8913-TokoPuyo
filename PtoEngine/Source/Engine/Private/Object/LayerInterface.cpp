
#include "Object/LayerInterface.h"

namespace DX
{
	Layer2DInterface::Layer2DInterface(int inLayer)
		: Layer(inLayer)
	{
	}
	int Layer2DInterface::GetLayer2D() const noexcept
	{
		return Layer;
	}
	void Layer2DInterface::SetLayer2D(int inLayer)
	{
	}
}