/*
#include "iteratorClass.h"

clusterIterator::clusterIterator(LARGE_INTEGER sectorOffset, DWORD numOfClustersToRead, HANDLE fileHandle){
	// Устанавливаем указатель на позицию по смещению.
	if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			cout << "Set File Pointer Error: " << GetLastError() << endl;
			system("pause");
			exit(GetLastError());
	}
}

BYTE* clusterIterator::First(){
    // Устанавливаем указатель на позицию по смещению.
	if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			cout << "Set File Pointer Error: " << GetLastError() << endl;
			system("pause");
			exit(GetLastError());
	}
}

BYTE* clusterIterator::GetCurrent(){

	BOOL readResult = false;		// Инициализируем результат чтения файловой записи
	DWORD bytesReturned = 0;

	// Чтение данных в buffer
	readResult = ReadFile(fileHandle, buffer, bufferSize, &bytesReturned, NULL);

	if(!readResult || bytesReturned != bufferSize) { // Обработка нарушения считывания диска
		cout << "Read boot record error: " << GetLastError() << endl;
		delete[] buffer;
		system("pause");
		exit(GetLastError());
	}

	return buffer;
}