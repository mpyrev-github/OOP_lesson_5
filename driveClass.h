#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <string>
#include <iomanip>

#include "iteratorClass.h"

class driveClass {
private:
	HANDLE fileHandle;				// ���������� ��������� ���������� (������� �����)

public:
	driveClass(const WCHAR *fileName);                           		// �����������

	HANDLE getFileHandle();

	BYTE *readRecords(LARGE_INTEGER sectorOffset, DWORD bytesPerCluster, DWORD numOfClustersToRead);

	void printHexBuffer(BYTE * buffer, DWORD bufferSize);     // ����������� ������� � HEX ����

	~driveClass();                                  // ����������
};


