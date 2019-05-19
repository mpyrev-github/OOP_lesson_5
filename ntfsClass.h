#pragma once

#include "fsClass.h"
#include "driveClass.h"

using namespace std;

class ntfsClass : public fsClass
{
public:
	ntfsClass(driveClass* driveObj);

	string getFsName() override;

	~ntfsClass();
};
