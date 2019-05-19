#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <string>
#include <iomanip>

#pragma pack(push, 1)

typedef struct {
  BYTE Offset_OEM[3];           // Смещение до имени файловой системы
  BYTE OEM_Name[8];             // Имя файловой системы
  BYTE BytesPerSector[2];       // Количество байт в секторе
  BYTE SectorsPerCluster;       // Количество секторов в кластере
  BYTE Offset_TotalSectors[26]; // Смещение до значения количества секторов в ФС
  ULONGLONG TotalSectors;       // Количество секторов в файловой системе
} ntfsBootRecord1;

#pragma pack(pop)

class driveClass {
private:
	HANDLE fileHandle;				// Дескриптор файлового устройства (раздела диска)

	BYTE *fsName;					// Название Файловой Системы

	DWORD bytesPerSector;
	DWORD sectorsPerCluster;
	DWORD totalSectors;
	DWORD numOfClustersToRead;
	DWORD firstClusterToRead;

	DWORD getBytesPerCluster();

	void printHexBuffer(BYTE * buffer);     // Отображение буффера в HEX виде

public:
	driveClass(const WCHAR *fileName);                           		// Конструктор

	ntfsBootRecord1 *currentRecord;	// Текущая загрузочная запись

	void setNumOfClustersToRead(DWORD &numOfClusters);
	void setFirstClusterToRead(DWORD &firstCluster);
	void readClusters();                            // Вывод интересующих кластеров
	void getAttributes();
	void close();                          // Закрытие диска
	void setAttributes();                 // Запись атрибутов в свойства

	BYTE *readRecords(LARGE_INTEGER sectorOffset,DWORD bufferSize);

	DWORD getTotalClusters();

	~driveClass();                                  // Деструктор
};


