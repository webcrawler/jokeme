
#include "Mutithread_Test.h"

Mutithread_Test* Mutithread_Test::create()
{
	Mutithread_Test* ptr = new Mutithread_Test();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

string Mutithread_Test::getWidgetFilePath()
{
	return "layer_test";
}

string Mutithread_Test::getLayerName()
{
	return "UI_Mutithread_Test";
}

Mutithread_Test::~Mutithread_Test()
{
}

void Mutithread_Test::updateUI()
{
	num = 1000;

	//pid1 = new (std::nothrow)(&Mutithread_Test::threadA, this);
}

void Mutithread_Test::threadA()
{
}

void Mutithread_Test::threadB()
{
}


