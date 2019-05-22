#pragma once

#include "fsClass.h"
#include "driveClass.h"

using namespace std;

class fatClass : public fsClass
{
private:

	double firstDataSector;
	DWORD reservedSectorsCount;
	DWORD fatSize16;
	long fatSize32;
	long fatSize;
	DWORD fatCopiesCount;
	DWORD totalFatSectors;
	DWORD rootDirSize;
	DWORD rootSectorsSize;
	DWORD totalSectors16;
	long totalSectors32;
	long totalSectors;

public:
	fatClass(driveClass* driveObj);

	virtual DWORD getFirstClusterNum() override;
	virtual DWORD getTotalClusters() override;
	virtual FLOAT getFsClustersOffset() override;
	virtual string getFsName() override;

	~fatClass();
};
