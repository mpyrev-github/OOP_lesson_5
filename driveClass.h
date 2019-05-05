#include <windows.h>
#include <string>

#pragma pack(push, 1)

typedef struct {
  BYTE Offset_OEM[3];           // Смещение до имени файловой системы
  BYTE OEM_Name[8];             // Имя файловой системы
  BYTE BytesPerSector[2];       // Количество байт в секторе
  BYTE SectorsPerCluster;       // Количество секторов в кластере
  BYTE Offset_TotalSectors[26]; // Смещение до значения количества секторов в ФС
  ULONGLONG TotalSectors;       // Количество секторов в файловой системе
} ntfsBootRecord;

#pragma pack(pop)

class driveClass {
protected:
	HANDLE fileHandle;				// Дескриптор файлового устройства (раздела диска)
	BYTE *fsName;					// Название Файловой Системы
	DWORD bytesPerSector;
	DWORD sectorsPerCluster;
	DWORD totalSectors;
	ntfsBootRecord *currentRecord;	// Текущая загрузочная запись

	HANDLE open(const WCHAR *fileName);     // Открытие диска
	void setFsAttributes();                 // Запись атрибутов в свойства
	void printHexBuffer(BYTE * buffer);     // Отображение буффера в HEX виде
	void close();                           // Закрытие диска

public:
	driveClass();                           		// Конструктор

	DWORD numOfClustersToRead;
	DWORD firstClusterToRead;

	BYTE *getFsName();
	DWORD getBytesPerSector();
	DWORD getSectorsPerCluster();
	DWORD getBytesPerCluster();
	DWORD getTotalClusters();
	bool checkBootRecord(const WCHAR *fileName);    // Проверка на соответствие ФС к NTFS
	void readClusters();                            // Вывод интересующих кластеров

	~driveClass();                                  // Деструктор
};


