/*
 class clusterIterator
{
public:
	clusterIterator(LARGE_INTEGER sectorOffset, DWORD numOfClustersToRead, HANDLE fileHandle);
	void First();
	void Next();
	bool IsDone();
	BYTE* GetCurrent();
    ~clusterIterator();
}
