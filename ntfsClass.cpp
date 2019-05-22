#include <iostream>
#include <windows.h>
#include "ntfsClass.h"

typedef struct
{
	BYTE Offset_OEM[3];           // Смещение до имени файловой системы
	BYTE OEM_Name[8];             // Имя файловой системы
	BYTE BytesPerSector[2];       // Количество байт в секторе
	BYTE SectorsPerCluster;       // Количество секторов в кластере
	BYTE Offset_TotalSectors[26]; // Смещение до значения количества секторов в ФС
	ULONGLONG TotalSectors;       // Количество секторов в файловой системе

} ntfsBootRecord;

using namespace std;

ntfsClass::ntfsClass(driveClass* driveObj) : fsClass(driveObj)
{
    LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = 0;
	ntfsBootRecord *currentRecord = (ntfsBootRecord*)driveObj->readRecords(sectorOffset, 1024, 1);

    // Придаем свойства объекту
	bytesPerSector = *(WORD*)currentRecord->BytesPerSector;
	sectorsPerCluster = currentRecord->SectorsPerCluster;
	totalSectors = currentRecord->TotalSectors;
}

DWORD ntfsClass::getFirstClusterNum(){
	return 0;
}

DWORD ntfsClass::getTotalClusters(){
	return totalSectors / sectorsPerCluster;
}

FLOAT ntfsClass::getFsClustersOffset(){
	return 0;
}

string ntfsClass::getFsName()
{
	return "NTFS";
}

ntfsClass::~ntfsClass() { }
