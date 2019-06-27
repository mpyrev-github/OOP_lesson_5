
#include <windows.h>
#include <iostream>

 class clusterIterator
{
private:
	HANDLE fileHandle;
	LARGE_INTEGER sectorOffset;
	LARGE_INTEGER sectorOffsetFirst;
	DWORD bytesPerCluster;
	DWORD numOfClustersToRead;
	DWORD clustersReaded;
public:
	clusterIterator(HANDLE fileHandle, LARGE_INTEGER sectorOffset, DWORD BytesPerCluster, DWORD numOfClustersToRead);
	virtual void First();
	virtual void Next();
	virtual bool IsDone();
	BYTE* GetCurrent();
	~clusterIterator();
};
