#pragma once

#include "fsClass.h"
#include "driveClass.h"

using namespace std;

class ntfsClass : public fsClass
{
public:
	ntfsClass(driveClass* driveObj);

	virtual DWORD getFirstClusterNum() override;
	virtual DWORD getTotalClusters() override;
	virtual FLOAT getFsClustersOffset() override;
	virtual string getFsName() override;

	~ntfsClass();
};
