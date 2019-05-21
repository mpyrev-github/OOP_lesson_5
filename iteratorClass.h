/*
 class clusterIterator
{
public:
	clusterIterator(LARGE_INTEGER sectorOffset, DWORD numOfClustersToRead, HANDLE fileHandle);
	void First() { currentPosition = 0; }
	void Next() { currentPosition++; }
	bool IsDone() { return currentPosition == arraySize; }
	BYTE* GetCurrent();
}
