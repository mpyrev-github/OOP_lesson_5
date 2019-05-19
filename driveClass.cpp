#include "driveClass.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Инициализация класса
driveClass::driveClass(const WCHAR *fileName) {
	fileHandle = INVALID_HANDLE_VALUE;
	currentRecord = NULL;
	fsName = (unsigned char*)"";
	bytesPerSector = 0;
	sectorsPerCluster = 0;
	totalSectors = 0;
	numOfClustersToRead = 0;
	firstClusterToRead = 0;

	// Открываем раздел диска.
	if ((fileHandle = CreateFileW(fileName,
		GENERIC_READ,       					// Режим доступа
		FILE_SHARE_READ | FILE_SHARE_WRITE,     // Режим совместной работы
		NULL,       							// Атрибуты безопасности
		OPEN_EXISTING,      					// Способ открытия
		0,      								// Флаги и атрибуты
		NULL)) == INVALID_HANDLE_VALUE)	{

		wcout << "Read disk (" << fileName[4] <<")... can`t open!" << endl;

		//cout << "Error: " << GetLastError() << endl;        // Код ошибки

		switch (GetLastError()) {       // Причины
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
}

// Метод проверки ФС выбранного диска на совпадение с NTFS
bool driveClass::checkBootRecord(const WCHAR *fileName) {
	DWORD bufferSize;
	BYTE *buffer;       			// Объявляем буфер для хранения загрузочной записи
	bufferSize = 1024;				// Устанавливаем размер буфера
	buffer = new BYTE[bufferSize];	// Выделение памяти для буфера указанного размера

	// Устанавливаем указатель на начало файла.
	SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN);

	BOOL readResult = false;		// Инициализируем результат чтения файловой записи
	DWORD bytesReturned = 0;

	// Чтение секторов загрузочной записи.
	readResult = ReadFile(fileHandle, buffer, bufferSize, &bytesReturned, NULL);
	if (!readResult) {
		cout << "Read boot record error: " << GetLastError() << endl;
		delete[] buffer;
		close();
		exit (GetLastError());
	} else {
		currentRecord = (ntfsBootRecord *) buffer;
		string ntfsName("NTFS    ");        // Сигнатура NTFS в записи OEM
		string fsName ((char *)currentRecord->OEM_Name);        // Запишем текущую сигнатуру в переменную типа string

		if (fsName != ntfsName) {      // Сравнение с известной сигнатурой
			cout << "File system on this drive is not NTFS!" << endl    // Обработка несоответствия
				 << "This file system doesn`t support!" << endl;
			close();
			system("pause");
			return false;
		} else {
			setAttributes();          // При совпадении задаем свойства объекту
			return true;
		}
	}

	delete[] buffer;
}

// Метод придания свойств объекту
void driveClass::setAttributes(){
	fsName = (unsigned char*)currentRecord->OEM_Name;
	bytesPerSector = *(WORD*)currentRecord->BytesPerSector;
	sectorsPerCluster = currentRecord->SectorsPerCluster;
	totalSectors = currentRecord->TotalSectors;
}

// Отображение атрибутивной информации пользователю
void driveClass::getAttributes(){
	cout << "File system name:" << fsName << endl
		 << "Bytes per sector:" << bytesPerSector << endl
		 << "Sectors per cluster:" << sectorsPerCluster << endl
		 << "Bytes per cluster:" << getBytesPerCluster() << endl
		 << "Total clusters:" << getTotalClusters() << endl;
}

DWORD driveClass::getBytesPerCluster(){
	return bytesPerSector * sectorsPerCluster;
}

DWORD driveClass::getTotalClusters(){
	return totalSectors / sectorsPerCluster;
}

void driveClass::setNumOfClustersToRead(DWORD &numOfClusters){
	numOfClustersToRead = numOfClusters;
}

void driveClass::setFirstClusterToRead(DWORD &firstCluster){
	firstClusterToRead = firstCluster;
}

// Метод чтения заданных пользователем кластеров
void driveClass::readClusters() {
	if (numOfClustersToRead == 0) {
		cout << "Nothing to print." << endl;
		return;
	}

	if ((firstClusterToRead <= getTotalClusters())) { // Проверка на корректность ввода
		DWORD bytesReturned = 0;
		DWORD bytesToRead = getBytesPerCluster() * numOfClustersToRead;
		BYTE *dataBuffer = new BYTE[bytesToRead];       					// Выделяем память
		LARGE_INTEGER sectorOffset;
		sectorOffset.QuadPart = firstClusterToRead * getBytesPerCluster();  // Задаем смещение

		// При чтении данных с физического устройства смещение должно соответствовать границе сектора!
		// Устанавливаем смещение
		if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			close();
			exit(GetLastError());
		}

		// Чтение данных в dataBuffer
		bool readResult = ReadFile(fileHandle, dataBuffer, bytesToRead, &bytesReturned, NULL);

		if(!readResult || bytesReturned != bytesToRead) { // Обработка нарушения считывания диска
			close();
			exit(GetLastError());
		}

		printHexBuffer(dataBuffer);     // Вывод в виде HEX значений
		delete[] dataBuffer;
	} else {
		cout << "Choosen sector unavailable.";
	}

	close();
}

// Метод вывода буффера в HEX виде
void driveClass::printHexBuffer(BYTE * buffer){
    for (int i = 1; i < getBytesPerCluster() * numOfClustersToRead + 1; i++) {
		cout << hex << setw(2) << setfill('0') << DWORD(buffer[i - 1]) << " ";

		if (i % 16 == 0) {
			cout << endl;
		}
		else if (i % 8 == 0)
		{
			cout << "  ";
		}
	}
    delete[] buffer;
}

void driveClass::close() {
	CloseHandle(fileHandle);
}

driveClass::~driveClass() {
	CloseHandle(fileHandle);
}
