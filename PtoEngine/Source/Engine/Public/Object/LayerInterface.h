#pragma once

namespace DX
{
	enum class ELayer
	{
		Ground,
		Event,
		Item,
		Building,
		Character,
		Effect
	};

	const ELayer LayerOrder[6] = {
		ELayer::Ground,
		ELayer::Event,
		ELayer::Item,
		ELayer::Building,
		ELayer::Character,
		ELayer::Effect
	};

	/*
	* Default Layer Order : 
	* 
	* Ground Layer (e.g. ground, river, grass...etc.
	* Event Layer (e.g. exit, start, carpet like shop.
	* Item Layer (e.g. potion, weapon...etc.
	* Building Layer (e.g. house, wood...etc.
	* Character Layer (e.g. player, enemy...etc.
	* Effect Layer (e.g. hit, weather...etc.
	* 
	* If you are not sure where your working layer belongs, ask pto8913. (The only one working on it was pto8913 lol.
	* 
	* What is Layer2D
	*
	* The smaller the Layer, the further back the object is displayed.
	*
	* ex. the player Layer is 1, the house Layer is 0,
	*
	* Default, the player try to overlap the house, the player appear front of the house.
	*
	* Next, the player press up key, the player Layer is 0.
	* In the state, the player try to overlap the house, the player stop moving in side of the house.
	*
	* Next, the player press up key once more. the player Layer is -1.
	* In the state, the player try to overlap the house, the player behind the house.
	*/
	struct Layer2DInterface
	{
	public:
		Layer2DInterface() = default;
		Layer2DInterface(int inLayer);

		virtual int GetLayer2D() const noexcept;
		/* Default can not change the Layer. */
		virtual void SetLayer2D(int inLayer);
	protected:
		int Layer = -1;
	};
}