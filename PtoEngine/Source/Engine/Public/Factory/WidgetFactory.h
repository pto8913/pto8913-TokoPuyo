#pragma once

struct IWidgetCreator
{
	IWidgetBase Exec();
};

struct IWidgetBase
{
};

class WidgetFactory
{
public:
	WidgetFactory(IWidgetCreator inCreator)
	{
		creator = inCreator;
	}
	void Do()
	{
		creator.Exec();
	}
private:
	IWidgetCreator creator;
};