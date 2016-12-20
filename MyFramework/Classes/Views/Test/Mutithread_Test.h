#ifndef __MUTITHREAD_TEST_H__
#define __MUTITHREAD_TEST_H__

#include "../../Define_.h"
#include <mutex>
#include <thread>

class Mutithread_Test: public LayerBase
{
public:
	static Mutithread_Test* create();
	Mutithread_Test() {};
	~Mutithread_Test();

public:
	virtual string getWidgetFilePath();
	virtual string getLayerName();
	virtual void updateUI();

private:
	std::thread* pid1;
	std::thread* pid2;
	std::mutex mutex;
	void threadA();
	void threadB();
	int num;
};

#endif 
