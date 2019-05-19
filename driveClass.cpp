#include "driveClass.h"

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

BYTE *driveClass::readRecords(LARGE_INTEGER sectorOffset,DWORD bufferSize){
	BYTE *buffer;
	buffer = new BYTE[bufferSize];	// Выделение памяти для буфера указанного размера
	// Устанавливаем указатель на начало файла.
	if (!SetFilePointerEx(fileHandle, sectorOffset, NULL, FILE_BEGIN)) {
			close();
			exit(GetLastError());
	}

	BOOL readResult = false;		// Инициализируем результат чтения файловой записи
	DWORD bytesReturned = 0;

    // Чтение данных в dataBuffer
	readResult = ReadFile(fileHandle, buffer, bufferSize, &bytesReturned, NULL);

	if(!readResult || bytesReturned != bufferSize) { // Обработка нарушения считывания диска
		cout << "Read boot record error: " << GetLastError() << endl;
		delete[] buffer;
		close();
		exit(GetLastError());
	}
	return buffer;
}

// Метод придания свойств объекту
void driveClass::setAttributes(){
	fsName = (unsigned char*)currentRecord->OEM_Name;
	bytesPerSector = *(WORD*)currentRecord->BytesPerSector;
	sectorsPerCluster = currentRecord->SectorsPerCluster;
	totalSectors = currentRecord->TotalSectors;
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

		DWORD bytesToRead = getBytesPerCluster() * numOfClustersToRead;
		LARGE_INTEGER sectorOffset;
		sectorOffset.QuadPart = firstClusterToRead * getBytesPerCluster();  // Задаем смещение

		BYTE *dataBuffer = readRecords(sectorOffset,bytesToRead);

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
