#include "driveClass.h"
#include <iostream>
#include <string>
#include <mbstring.h>

using namespace std;

// ������������� ������
driveClass::driveClass() {
	fileHandle = INVALID_HANDLE_VALUE;
	currentRecord = NULL;
	fsName = (unsigned char*)"";
	bytesPerSector = 0;
	sectorsPerCluster = 0;
	totalClusters = 0;
}

// ����� �������� �����
HANDLE driveClass::open(const WCHAR *fileName) {
	// ��������� ������ �����.
	if ((fileHandle = CreateFileW(fileName,
		GENERIC_READ,       					// ����� �������
		FILE_SHARE_READ | FILE_SHARE_WRITE,     // ����� ���������� ������
		NULL,       							// �������� ������������
		OPEN_EXISTING,      					// ������ ��������
		0,      								// ����� � ��������
		NULL)) == INVALID_HANDLE_VALUE)	{

		wcout << "Read disk (" << fileName[4] <<")... can`t open!" << endl;

		//cout << "Error: " << GetLastError() << endl;        // ��� ������

		switch (GetLastError()) {       // �������
			case 2:
				cout << "Disk doesn`t exists!" << endl;
				break;
			case 5:
				cout << "Open with admin rights!" << endl;
				break;
			default:
				cout << "Unknown error." << endl;
				break;
		}

		system("pause");
		exit(-1);
	}
	return fileHandle;
}

// ����� �������� �� ���������� ����� �� ���������� � NTFS
bool driveClass::checkBootRecord(const WCHAR *fileName) {
	DWORD bufferSize;
	BYTE *buffer;       			// ��������� ����� ��� �������� ����������� ������
	bufferSize = 1024;				// ������������� ������ ������
	buffer = new BYTE[bufferSize];	// ��������� ������ ��� ������ ���������� �������
	fileHandle = open(fileName);    // ������������� ������������� �������

	// ������������� ��������� �� ������ �����.
	SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN);

	BOOL readResult = false;		// �������������� ��������� ������ �������� ������
	DWORD bytesReturned = 0;

	// ������ �������� ����������� ������.
	readResult = ReadFile(fileHandle, buffer, bufferSize, &bytesReturned, NULL);
	if (!readResult) {
		cout << "Read boot record error: " << GetLastError() << endl;
		delete[] buffer;
		close();
		exit (GetLastError());
	} else {
		currentRecord = (ntfsBootRecord *) buffer;
		if (_mbscmp(currentRecord->OEM_Name,(BYTE*) "NTFS    ")) {      // ��������� � ��������� ����������
			cout << "File system on this drive is not NTFS!" << endl    // ��������� ��������������
				 << "This file system doesn`t support!" << endl;
			close();
			system("pause");
			return false;
		} else {
			setFsAttributes();          // ��� ���������� ������ �������� �������
			return true;
		}
	}

	delete[] buffer;
}

// ����� �������� ������� �������
void driveClass::setFsAttributes(){
	fsName = (unsigned char*)currentRecord->OEM_Name;
	bytesPerSector = *(WORD*)currentRecord->BytesPerSector;
	sectorsPerCluster = currentRecord->SectorsPerCluster;
	totalClusters = currentRecord->TotalSectors;
}

BYTE *driveClass::getFsName(){
	return fsName;
}

DWORD driveClass::getBytesPerSector(){
	return bytesPerSector;
}

DWORD driveClass::getSectorsPerCluster(){
	return sectorsPerCluster;
}

DWORD driveClass::getBytesPerCluster(){
	return bytesPerSector * sectorsPerCluster;
}

DWORD driveClass::getTotalClusters(){
	return totalClusters;
}

// ����� ������ �������� ������������� ���������
void driveClass::readClusters() {

	if (!(firstClusterToRead < 1 || firstClusterToRead >= totalClusters)) { // �������� �� ������������ �����
		DWORD bytesReturned = 0;
		DWORD bytesToRead = bytesPerSector * numOfClustersToRead;
		BYTE *dataBuffer = new BYTE[bytesToRead];       					// �������� ������
		LARGE_INTEGER sectorOffset;
		sectorOffset.QuadPart = firstClusterToRead * getBytesPerCluster();  // ������ ��������

		// ��� ������ ������ � ����������� ���������� �������� ������ ��������������� ������� �������!
		// ������������� ��������
		if (!SetFilePointer(fileHandle, sectorOffset.LowPart, &sectorOffset.HighPart, FILE_BEGIN)) {
			close();
			exit(GetLastError());
		}

		// ������ ������ � dataBuffer
		bool readResult = ReadFile(fileHandle, dataBuffer, bytesToRead, &bytesReturned, NULL);

		if(!readResult || bytesReturned != bytesToRead) { // ��������� ��������� ���������� �����
			close();
			exit(GetLastError());
		}

		printHexBuffer(dataBuffer);     // ����� � ���� HEX ��������
	} else {
		cout << "Choosen sector unavailable.";
	}

	close();

}

// ����� ������ ������� � HEX ����
void driveClass::printHexBuffer(BYTE * buffer)
{
	for (int i = 1; i < getBytesPerCluster() + 1; i++) {
		printf("%02x ", buffer[i - 1]);

		if (i % 16 == 0) {
			cout << endl;
		}
		else if (i % 8 == 0)
		{
			cout << "  ";
		}
	}
}

void driveClass::close() {
	CloseHandle(fileHandle);
}

driveClass::~driveClass() {
	CloseHandle(fileHandle);
}
