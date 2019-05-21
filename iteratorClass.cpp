/*
#include "iteratorClass.h"

clusterIterator::clusterIterator(LARGE_INTEGER sectorOffset, DWORD numOfClustersToRead, HANDLE fileHandle){
	// ������������� ��������� �� ������� �� ��������.
	if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			cout << "Set File Pointer Error: " << GetLastError() << endl;
			system("pause");
			exit(GetLastError());
	}
}

BYTE* clusterIterator::First(){
    // ������������� ��������� �� ������� �� ��������.
	if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			cout << "Set File Pointer Error: " << GetLastError() << endl;
			system("pause");
			exit(GetLastError());
	}
}

BYTE* clusterIterator::GetCurrent(){

	BOOL readResult = false;		// �������������� ��������� ������ �������� ������
	DWORD bytesReturned = 0;

	// ������ ������ � buffer
	readResult = ReadFile(fileHandle, buffer, bufferSize, &bytesReturned, NULL);

	if(!readResult || bytesReturned != bufferSize) { // ��������� ��������� ���������� �����
		cout << "Read boot record error: " << GetLastError() << endl;
		delete[] buffer;
		system("pause");
		exit(GetLastError());
	}

	return buffer;
}