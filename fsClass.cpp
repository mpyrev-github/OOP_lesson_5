#include "fsClass.h"
#include "driveClass.h"

using namespace std;

fsClass::fsClass(driveClass* driveObj){
	this->driveObj = driveObj;
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

// Узнаем у пользователя количество интересующих кластеров
void fsClass::setNumOfClustersToRead(){
	tryAgainEnterNumOfClusters:
	cout << "How many clusters do you want to read?" << endl;
	cin >> numOfClustersToRead;
	if (numOfClustersToRead == 0) {
		cout << "Nothing to print." << endl;
		goto tryAgainEnterNumOfClusters;
	}
}

void fsClass::setFirstClusterToRead(){
	tryAgainEnterFirstCluster:
	cout <<	"Where to start? Available clusters: [" << getFirstClusterNum() << ", "
		 << getTotalClusters() - 1 + getFirstClusterNum() << "]" << endl;
	cin >> firstClusterToRead;
	if ((firstClusterToRead < getFirstClusterNum() || firstClusterToRead >= (getTotalClusters() + getFirstClusterNum()))) { // Проверка на корректность ввода
		cout << "Choosen sector unavailable." << endl;
		goto tryAgainEnterFirstCluster;
	}
	firstClusterToRead += getFsClustersOffset() - getFirstClusterNum();
}

// Метод чтения заданных пользователем кластеров
void fsClass::readClusters(){
		DWORD bytesToRead = getBytesPerCluster() * numOfClustersToRead;
		LARGE_INTEGER sectorOffset;
		sectorOffset.QuadPart = firstClusterToRead * getBytesPerCluster();  // Задаем смещение
		numOfClustersToRead == 1 ?
		cout << endl << "Sector by offset:" << sectorOffset.QuadPart << endl:
		cout << endl << "Sectors by offset:" << sectorOffset.QuadPart << endl;
		BYTE *dataBuffer = driveObj->readRecords(sectorOffset, bytesToRead);
		driveObj->printHexBuffer(dataBuffer, bytesToRead);     // Вывод в виде HEX значений
		delete[] dataBuffer;
}

fsClass::~fsClass(){

}
