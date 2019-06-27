
#include <windows.h>
#include <iostream>

 class clusterIteratorDecorator : public clusterIterator 
{
private:
	HANDLE fileHandle;
	LARGE_INTEGER sectorOffset;
	LARGE_INTEGER sectorOffsetFirst;
	DWORD bytesPerCluster;
	DWORD numOfClustersToRead;
	DWORD clustersReaded;
public:
	clusterIteratorDecorator(HANDLE fileHandle, LARGE_INTEGER sectorOffset, DWORD BytesPerCluster, DWORD numOfClustersToRead);
	virtual void Next() override;
	~clusterIteratorDecorator();
};
