#ifndef __3D_TEST_H__
#define __3D_TEST_H__

#include "../../Define_.h"

class Layer3DTest : public LayerBase
{
public:
	static Layer3DTest* create();

	virtual string getWidgetFilePath();
	virtual string getLayerName();
	virtual void updateUI();

public:
	void test3D();
	void afterCaptured(bool succeed, const std::string& outputFile);

private:
};

#endif 
