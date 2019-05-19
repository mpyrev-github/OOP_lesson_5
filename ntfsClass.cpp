#include <iostream>
#include <windows.h>
#include "ntfsClass.h"

typedef struct
{
	BYTE Offset_OEM[3];           // �������� �� ����� �������� �������
	BYTE OEM_Name[8];             // ��� �������� �������
	BYTE BytesPerSector[2];       // ���������� ���� � �������
	BYTE SectorsPerCluster;       // ���������� �������� � ��������
	BYTE Offset_TotalSectors[26]; // �������� �� �������� ���������� �������� � ��
	ULONGLONG TotalSectors;       // ���������� �������� � �������� �������

} ntfsBootRecord;

using namespace std;

ntfsClass::ntfsClass(driveClass* driveObj) : fsClass(driveObj)
{
    LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = 0;
	ntfsBootRecord *currentRecord = (ntfsBootRecord*)driveObj->readRecords(sectorOffset, 1024, driveObj->getFileHandle());

    // ������� �������� �������
	bytesPerSector = *(WORD*)currentRecord->BytesPerSector;
	sectorsPerCluster = currentRecord->SectorsPerCluster;
	totalSectors = currentRecord->TotalSectors;
}

string ntfsClass::getFsName()
{
	return "NTFS";
}

ntfsClass::~ntfsClass() { }
