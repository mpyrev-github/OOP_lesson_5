#include <iostream>
#include <windows.h>
#include <cmath>

#include "exfatClass.h"

typedef struct
{
	BYTE Offset_OEM[3];           	// �������� �� ����� �������� �������
	BYTE OEM_Name[8];             	// ��� �������� �������
	BYTE Offset_TotalSectors[61]; 	// �������� �� �������� ���������� �������� � ��
	ULONGLONG TotalSectors; 		// ���������� �������� � �������� �������
	DWORD FATStartSector; 			// ������ � ������� ���������� exFAT
	DWORD FATSizeInSectors; 		// ������ exFAT � ��������
	DWORD FirstDataSector; 			// ������ � ������� ���������� ������� ����� ���������
	DWORD TotalClusters; 			// ���������� ��������� � �������� �������
	DWORD RootDirCluster; 			// �������, � ������� ��������� �������� �������
	BYTE Offset_SectorFactor[8];	// �������� �� �������� c������ ������� ������� � ������
	BYTE SectorFactor; 				// ������� ������� ������� � ������
	BYTE ClusterFactor; 			// ������� ������� �������� � ������

} exfatBootRecord;

using namespace std;

exfatClass::exfatClass(driveClass* driveObj) : fsClass(driveObj)
{
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = 0;
	exfatBootRecord *currentRecord = (exfatBootRecord*)driveObj->readRecords(sectorOffset, 1024);

	// ������� �������� �������
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

