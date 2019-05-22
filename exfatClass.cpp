#include <iostream>
#include <windows.h>
#include <cmath>

#include "exfatClass.h"

typedef struct
{
	BYTE Offset_OEM[3];           	// Смещение до имени файловой системы
	BYTE OEM_Name[8];             	// Имя файловой системы
	BYTE Offset_TotalSectors[61]; 	// Смещение до значения количества секторов в ФС
	ULONGLONG TotalSectors; 		// Количество секторов в файловой системе
	DWORD FATStartSector; 			// Сектор в котором начинается exFAT
	DWORD FATSizeInSectors; 		// Размер exFAT в секторах
	DWORD FirstDataSector; 			// Сектор в котором начинается битовая карта кластеров
	DWORD TotalClusters; 			// Количество кластеров в файловой системе
	DWORD RootDirCluster; 			// Кластер, в котором находится корневой каталог
	BYTE Offset_SectorFactor[8];	// Смещение до значения cтепени размера сектора в байтах
	BYTE SectorFactor; 				// Степень размера сектора в байтах
	BYTE ClusterFactor; 			// Степень размера кластера в байтах

} exfatBootRecord;

using namespace std;

exfatClass::exfatClass(driveClass* driveObj) : fsClass(driveObj)
{
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = 0;
	exfatBootRecord *currentRecord = (exfatBootRecord*)driveObj->readRecords(sectorOffset, 1024);

	// Придаем свойства объекту
	bytesPerSector = (DWORD) currentRecord->SectorFactor;
	bytesPerSector = pow(2, bytesPerSector);
	sectorsPerCluster = currentRecord->ClusterFactor;
	sectorsPerCluster = pow(2, sectorsPerCluster);
	firstDataSector = currentRecord->FirstDataSector;
	totalClusters = currentRecord->TotalClusters;
	totalSectors = currentRecord->TotalSectors;
}

DWORD exfatClass::getFirstClusterNum(){
	return 2;
}

DWORD exfatClass::getTotalClusters(){
	return totalClusters;
}

double exfatClass::getFsClustersOffset(){
	return firstDataSector / sectorsPerCluster;
}

string exfatClass::getFsName()
{
	return "EXFAT";
}

exfatClass::~exfatClass() { }

