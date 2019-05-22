#pragma once

#include "fsClass.h"
#include "driveClass.h"

using namespace std;

class exfatClass : public fsClass
{
private:
	DWORD totalClusters;
	DWORD firstDataSector;
public:
	exfatClass(driveClass* driveObj);

	virtual DWORD getFirstClusterNum() override;
	virtual DWORD getTotalClusters() override;
	virtual double getFsClustersOffset() override;
	virtual string getFsName() override;

	~exfatClass();
};

