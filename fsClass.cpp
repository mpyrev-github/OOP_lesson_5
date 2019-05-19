#include "fsClass.h"
#include "driveClass.h"

using namespace std;

fsClass::fsClass(driveClass* driveObj){
	driveObj = driveObj;
}

// ����������� ������������ ���������� ������������
void fsClass::getAttributes(){
	cout << "File system name:" << getFsName() << endl
		 << "Bytes per sector:" << bytesPerSector << endl
		 << "Sectors per cluster:" << sectorsPerCluster << endl
		 << "Bytes per cluster:" << getBytesPerCluster() << endl
		 << "Total clusters:" << getTotalClusters() << endl;
}

DWORD fsClass::getBytesPerCluster(){
	return bytesPerSector * sectorsPerCluster;
}

DWORD fsClass::getTotalClusters(){
	return totalSectors / sectorsPerCluster;
}

void fsClass::setNumOfClustersToRead(DWORD &numOfClusters){
	numOfClustersToRead = numOfClusters;
}

void fsClass::setFirstClusterToRead(DWORD &firstCluster){
	firstClusterToRead = firstCluster;
}

// ����� ������ �������� ������������� ���������
void fsClass::readClusters(HANDLE fileHandle) {
	if (numOfClustersToRead == 0) {
		cout << "Nothing to print." << endl;
		return;
	}
	if ((firstClusterToRead <= getTotalClusters())) { // �������� �� ������������ �����

		DWORD bytesToRead = getBytesPerCluster() * numOfClustersToRead;
		LARGE_INTEGER sectorOffset;
		sectorOffset.QuadPart = firstClusterToRead * getBytesPerCluster();  // ������ ��������
		BYTE *dataBuffer = driveObj->readRecords(sectorOffset,bytesToRead,fileHandle);
		driveObj->printHexBuffer(dataBuffer, bytesToRead);     // ����� � ���� HEX ��������
		delete[] dataBuffer;
	} else {
		cout << "Choosen sector unavailable.";
	}
}

fsClass::~fsClass(){

}
