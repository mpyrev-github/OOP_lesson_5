#include "iteratorClass.h"

using namespace std;

clusterIterator::clusterIterator(HANDLE fileHandle, LARGE_INTEGER sectorOffset, DWORD bytesPerCluster, DWORD numOfClustersToRead){
	clusterIterator(HANDLE fileHandle, LARGE_INTEGER sectorOffset, DWORD BytesPerCluster, DWORD numOfClustersToRead);
	clustersReaded = 0;
}

void clusterIterator::Next(){
	sectorOffset.QuadPart -= bytesPerCluster * 2;
	clustersReaded += 1;
}