#pragma once

#include "fsClass.h"
#include "driveClass.h"

using namespace std;

class exfatClass : public fsClass
{
public:
	exfatClass(driveClass* driveObj);

	string getFsName() override;

	~exfatClass();
};

