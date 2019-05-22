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
	double sectorsPerCluster;
	DWORD totalSectors;

	DWORD numOfClustersToRead;
	FLOAT firstClusterToRead;

	DWORD getBytesPerCluster();

	virtual DWORD getTotalClusters() = 0;
	virtual DWORD getFirstClusterNum() = 0;
	virtual double getFsClustersOffset() = 0;

public:
	fsClass(driveClass* driveObj);

	void setNumOfClustersToRead();
	void setFirstClusterToRead();
	void getAttributes();
	void readClusters();


	virtual string getFsName() = 0;

	~fsClass();
};