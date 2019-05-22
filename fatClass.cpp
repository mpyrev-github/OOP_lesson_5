#include <iostream>
#include <windows.h>
#include <cmath>

#include "fatClass.h"

#pragma pack(push, 1)
typedef struct
{
	BYTE Offset_OEM[3];           			// �������� �� ����� �������� �������
	BYTE OEM_Name[8];             			// ��� �������� �������
	BYTE BytesPerSector[2];         		// ���������� ���� � �������
	BYTE SectorsPerCluster;         		// ���������� �������� � ��������
	BYTE ReservedSectorsCount[2];   		// ����������������� �������
	BYTE FatCopiesCount;            		// ���������� ����� FAT
	BYTE RootDirSize[2];                    // ������ ��������� ��������� FAT12/16
	BYTE TotalSectors16[2];         		// ���������� �������� � �������� ������� FAT12/16
	BYTE DevType;                           // ��� �������
	BYTE FatSize16[2];                      // ������ FAT � �������� FAT12/16
	BYTE Offset_StartSectors[4];    		// �������� �� �������� ���������� �������
	BYTE StartSectors[4];           		// ���������� �������� ����� ������� �������
	BYTE TotalSectors32[4];         		// ���������� �������� � �������� ������� FAT32
	BYTE FatSize32[4];                    	// ������ FAT � �������� FAT32
	BYTE Offset_RootDirCluster[4];          // �������� �� �������� ��������, � ������� ��������� �������� �������
	BYTE RootDirCluster[4];					// �������, � ������� ��������� �������� ������� FAT32
	BYTE Offset_BootCopySectorNumber[2];    // �������� �� ��������
	BYTE BootCopySectorNumber[2];           // ������, � ������� ��������� ��������� ����� ������������ �������

} fatBootRecord;
#pragma pack(pop)

using namespace std;

fatClass::fatClass(driveClass* driveObj) : fsClass(driveObj)
{
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = 0;
	fatBootRecord *currentRecord = (fatBootRecord*)driveObj->readRecords(sectorOffset, 1024, 1);

	// ������� �������� �������
	bytesPerSector = *(WORD *)currentRecord->BytesPerSector;
	sectorsPerCluster = currentRecord->SectorsPerCluster;
	reservedSectorsCount = *(WORD *)currentRecord->ReservedSectorsCount;
	fatSize16 = *(WORD *)currentRecord->FatSize16;
	fatSize32 = *(long *)currentRecord->FatSize32;
	fatSize = (fatSize16 != 0 ? fatSize16 : fatSize32);
	fatCopiesCount = currentRecord->FatCopiesCount;
	totalFatSectors = fatCopiesCount * fatSize;
	rootDirSize = *(WORD *) currentRecord->RootDirSize;
	rootSectorsSize = (rootDirSize * 32) / bytesPerSector;
	firstDataSector = reservedSectorsCount + totalFatSectors + rootSectorsSize;
	totalSectors16 = *(WORD *)currentRecord->TotalSectors16;
	totalSectors32 = *(long *)currentRecord->TotalSectors32;
	totalSectors = (totalSectors16 != 0 ? totalSectors16 : totalSectors32) - firstDataSector;
}

DWORD fatClass::getFirstClusterNum(){
	return 2;
}

DWORD fatClass::getTotalClusters(){
	return totalSectors / sectorsPerCluster;
}

FLOAT fatClass::getFsClustersOffset(){
	return firstDataSector / sectorsPerCluster;
}

string fatClass::getFsName()
{
	string fatID = (totalSectors16 != 0 ? (string)"12/16" : (string)"32");
	string fsName = (string)"FAT" + fatID;
	return fsName;
}

fatClass::~fatClass() { }

