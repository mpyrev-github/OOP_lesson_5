#include "fsClass.h"
#include "driveClass.h"

using namespace std;

fsClass::fsClass(driveClass* driveObj){
	driveObj = driveObj;
}

// Отображение атрибутивной информации пользователю
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

// Метод чтения заданных пользователем кластеров
void fsClass::readClusters(HANDLE fileHandle) {
	if (numOfClustersToRead == 0) {
		cout << "Nothing to print." << endl;
		return;
	}
	if ((firstClusterToRead <= getTotalClusters())) { // Проверка на корректность ввода

		DWORD bytesToRead = getBytesPerCluster() * numOfClustersToRead;
		LARGE_INTEGER sectorOffset;
		sectorOffset.QuadPart = firstClusterToRead * getBytesPerCluster();  // Задаем смещение
		BYTE *dataBuffer = driveObj->readRecords(sectorOffset,bytesToRead,fileHandle);
		driveObj->printHexBuffer(dataBuffer, bytesToRead);     // Вывод в виде HEX значений
		delete[] dataBuffer;
	} else {
		cout << "Choosen sector unavailable.";
	}
}

fsClass::~fsClass(){

}
