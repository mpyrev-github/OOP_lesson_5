#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <string>
#include <iomanip>

#pragma pack(push, 1)

typedef struct {
  BYTE Offset_OEM[3];           // �������� �� ����� �������� �������
  BYTE OEM_Name[8];             // ��� �������� �������
  BYTE BytesPerSector[2];       // ���������� ���� � �������
  BYTE SectorsPerCluster;       // ���������� �������� � ��������
  BYTE Offset_TotalSectors[26]; // �������� �� �������� ���������� �������� � ��
  ULONGLONG TotalSectors;       // ���������� �������� � �������� �������
} ntfsBootRecord1;

#pragma pack(pop)

class driveClass {
private:
	HANDLE fileHandle;				// ���������� ��������� ���������� (������� �����)

	BYTE *fsName;					// �������� �������� �������

	DWORD bytesPerSector;
	DWORD sectorsPerCluster;
	DWORD totalSectors;
	DWORD numOfClustersToRead;
	DWORD firstClusterToRead;

	DWORD getBytesPerCluster();

	void printHexBuffer(BYTE * buffer);     // ����������� ������� � HEX ����

public:
	driveClass(const WCHAR *fileName);                           		// �����������

	ntfsBootRecord1 *currentRecord;	// ������� ����������� ������

	void setNumOfClustersToRead(DWORD &numOfClusters);
	void setFirstClusterToRead(DWORD &firstCluster);
	void readClusters();                            // ����� ������������ ���������
	void getAttributes();
	void close();                          // �������� �����
	void setAttributes();                 // ������ ��������� � ��������

	BYTE *readRecords(LARGE_INTEGER sectorOffset,DWORD bufferSize);

	DWORD getTotalClusters();

	~driveClass();                                  // ����������
};


