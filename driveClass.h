#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <string>
#include <iomanip>

class driveClass {
private:
	HANDLE fileHandle;				// ���������� ��������� ���������� (������� �����)

public:
	driveClass(const WCHAR *fileName);                           		// �����������

	HANDLE getFileHandle();

	BYTE *readRecords(LARGE_INTEGER sectorOffset,DWORD bufferSize);

	void printHexBuffer(BYTE * buffer, DWORD bufferSize);     // ����������� ������� � HEX ����

	~driveClass();                                  // ����������
};


