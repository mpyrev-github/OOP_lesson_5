#include "iteratorClass.h"

using namespace std;

clusterIterator::clusterIterator(HANDLE fileHandle, LARGE_INTEGER sectorOffset, DWORD bytesPerCluster, DWORD numOfClustersToRead){
	this->fileHandle = fileHandle;
	sectorOffsetFirst = sectorOffset;
	this->bytesPerCluster = bytesPerCluster;
	this->numOfClustersToRead = numOfClustersToRead;
	clustersReaded = 0;
}

void clusterIterator::First(){
	sectorOffset = sectorOffsetFirst;
	sectorOffset.QuadPart += bytesPerCluster * (numOfClustersToRead - 1);
}

void clusterIterator::Next(){
	sectorOffset.QuadPart -= bytesPerCluster;
	clustersReaded += 1;
}

bool clusterIterator::IsDone(){

	return clustersReaded == numOfClustersToRead;
}

BYTE* clusterIterator::GetCurrent(){

	BYTE* buffer;
	buffer = new BYTE[bytesPerCluster];

	// Устанавливаем указатель на позицию по смещению.
	if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			cout << "Set File Pointer Error: " << GetLastError() << endl;
			system("pause");
			exit(GetLastError());
	}
	BOOL readResult = false;		// Инициализируем результат чтения файловой записи
	DWORD bytesReturned = 0;

	// Чтение данных в buffer
	readResult = ReadFile(fileHandle, buffer, bytesPerCluster, &bytesReturned, NULL);

	if(!readResult || bytesReturned != bytesPerCluster) { // Обработка нарушения считывания диска
		cout << "Read boot record error: " << GetLastError() << endl;
		delete[] buffer;
		system("pause");
		exit(GetLastError());
	}

	return buffer;
}