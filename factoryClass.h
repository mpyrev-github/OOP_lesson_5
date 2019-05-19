#pragma once

#include "driveClass.h"
#include "fsClass.h"

class factoryClass
{
private:
	factoryClass();
	~factoryClass();

public:
	static fsClass* objCreator(driveClass* driveObj);

};