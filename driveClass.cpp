#include "driveClass.h"

using namespace std;

// Инициализация класса
driveClass::driveClass(const WCHAR *fileName) {
	//fileHandle = INVALID_HANDLE_VALUE;

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

HANDLE driveClass::getFileHandle(){
	return fileHandle;
}

BYTE *driveClass::readRecords(LARGE_INTEGER sectorOffset, DWORD bytesPerCluster, DWORD numOfClustersToRead){

	BYTE* buffer = new BYTE;
	clusterIterator * iterator = new clusterIterator(fileHandle, sectorOffset, bytesPerCluster, numOfClustersToRead);
	for(iterator->First(); !iterator->IsDone(); iterator->Next())
	{
		buffer = iterator->GetCurrent();
	}

	return buffer;
}

// Метод вывода буффера в HEX виде
void driveClass::printHexBuffer(BYTE * buffer, DWORD bufferSize){
	cout << "[----------------------   ----------------------]" << endl;
	for (int i = 1; i < bufferSize + 1; i++) {
		cout << hex << setw(2) << setfill('0') << DWORD(buffer[i - 1]) << " ";

		if (i % 16 == 0) {
			cout << endl;
		}
		else if (i % 8 == 0)
		{
			cout << "  ";
		}
	}
	cout << "[----------------------   ----------------------]" << endl;
    delete[] buffer;
}

driveClass::~driveClass() {
	CloseHandle(fileHandle);
}
