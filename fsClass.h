#pragma once

#include <windows.h>
#include <string>
#include <iostream>

#include "driveClass.h"

using namespace std;

class fsClass {
protected:
	driveClass* driveObj;

	DWORD bytesPerSector;
	FLOAT sectorsPerCluster;
	DWORD totalSectors;

	DWORD numOfClustersToRead = 0;
	FLOAT firstClusterToRead = 0;

	DWORD getBytesPerCluster();

	virtual DWORD getTotalClusters() = 0;
	virtual DWORD getFirstClusterNum() = 0;
	virtual FLOAT getFsClustersOffset() = 0;

public:
	fsClass(driveClass* driveObj);

	void setNumOfClustersToRead();
	void setFirstClusterToRead();
	void getAttributes();
	void readClusters();

	virtual string getFsName() = 0;

	~fsClass();
};