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
	DWORD sectorsPerCluster;
	DWORD totalSectors;

	DWORD numOfClustersToRead;
	DWORD firstClusterToRead;

	DWORD getBytesPerCluster();

public:
	fsClass(driveClass* driveObj);

	DWORD getTotalClusters();

	void setNumOfClustersToRead(DWORD &numOfClusters);
	void setFirstClusterToRead(DWORD &firstCluster);
	void getAttributes();
	void readClusters(HANDLE fileHandle);

	virtual string getFsName() = 0;

	~fsClass();
};