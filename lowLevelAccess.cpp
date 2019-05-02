#pragma hdrstop
#pragma argsused

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string.h>

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;

    // Дескриптор файлового устройства (раздела диска).
	HANDLE partition = INVALID_HANDLE_VALUE;
	// Сведения о разделе.
	PARTITION_INFORMATION partitionInfo = {0};
	// Сведения о геометрии диска, на котором расположен раздел.
	DISK_GEOMETRY diskGeometry = {0};

	// Дескриптор файла для сохранения образа раздела.
	//HANDLE file = INVALID_HANDLE_VALUE;

	// Буфер для чтения.
	BYTE* buffer = NULL;
	// Размер буфера.
	DWORD bufferSize = 0;

	// Количество возвращенных байт.
	DWORD bytesReturned = 0;
	// Количество записанных байт.
	DWORD bytesWritten = 0;

	// Результат выполнения.
	BOOL result = FALSE;

	// Узнаем у пользователя имя диска
	wstring diskLetter = L"";
	cout << "Enter disk letter:\n";
	diskLetter += L"\\\\.\\";
	diskLetter += cin.get();
	diskLetter += L":";

	// Открываем раздел диска.
	if ((partition = CreateFileW(diskLetter.c_str(),
		GENERIC_READ,       // Режим доступа
		FILE_SHARE_READ | FILE_SHARE_WRITE,     // Режим совместной работы
		NULL,       // Атрибуты безопасности
		OPEN_EXISTING,      // Способ открытия
		0,      // Флаги и атрибуты
		NULL)) == INVALID_HANDLE_VALUE)     // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ
	{
		wcout << "Read disk (" << diskLetter.c_str()[4] <<")... can`t open!" << endl;
		cout << "Error: " << GetLastError() << endl;        // Код ошибки
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
		return (-1);
	}

	// Запрашиваем сведения о геометрии диска, на котором расположен раздел.
	if (!DeviceIoControl(partition,     // запрошенное устройство
		IOCTL_DISK_GET_DRIVE_GEOMETRY,      // выполняемая операция
		NULL,
		0,
		&diskGeometry,      // буфер вывода
		sizeof (DISK_GEOMETRY),
		&bytesReturned,     // возвращено байтов
		(LPOVERLAPPED) NULL))       // синхронизация ввода/вывода (I/O)
	{
		cout << "Error get drive geometry: " << GetLastError() << endl;
		CloseHandle(partition);
		system("pause");
		return (-2);
	}

	// Запрашиваем сведения о разделе.
	if (!DeviceIoControl(partition,     // запрошенное устройство
		IOCTL_DISK_GET_PARTITION_INFO,      // выполняемая операция
		NULL,
		0,
		&partitionInfo,      // буфер вывода
		sizeof (PARTITION_INFORMATION),
		&bytesReturned,     // возвращено байтов
		(LPOVERLAPPED)NULL))       // синхронизация ввода/вывода (I/O)
	{
		cout << "Error get partition info: " << GetLastError() << endl;
		CloseHandle(partition);
		system("pause");
		return (-3);
	}

	// Размер раздела
	cout << "Partition's size (in bytes): " << partitionInfo.PartitionLength.QuadPart << endl;
	// Размер диска
	cout << "Disk's size (in bytes):      " << (diskGeometry.Cylinders.QuadPart) *
		diskGeometry.TracksPerCylinder * diskGeometry.SectorsPerTrack *
		diskGeometry.BytesPerSector << endl
		 << "Num of cylinders:    " << diskGeometry.Cylinders.QuadPart << endl
		 << "Tracks per cylinder: " << diskGeometry.TracksPerCylinder << endl
		 << "Sectors per track:   " << diskGeometry.SectorsPerTrack << endl
		 << "Bytes per sector:    " << diskGeometry.BytesPerSector << endl;

	// Запрашиваем сведения о файловой системе
	WCHAR szVolumeName[100]    = L"";       // имя тома
	WCHAR szFileSystemName[10] = L"";       // имя файловой системы
	DWORD dwSerialNumber       = 0;         // серийный номер
	DWORD dwMaxFileNameLength  = 0;         // максимальная длина имени в ФС
	DWORD dwFileSystemFlags    = 0;         // системные флаги

	wstring diskLetterShort = L"";
		diskLetterShort = diskLetter[4];
		diskLetterShort += L"://";      // короткое имя типа *://

	if(GetVolumeInformationW(diskLetterShort.c_str(),
							szVolumeName,
							sizeof(szVolumeName),
							&dwSerialNumber,
							&dwMaxFileNameLength,
							&dwFileSystemFlags,
							szFileSystemName,
							sizeof(szFileSystemName)))
	{
		wcout << L"Volume name = " << szVolumeName << endl
			 << L"Serial number = " << dwSerialNumber << endl
			 << L"Max. filename length = " << dwMaxFileNameLength
			 << endl
			 << L"File system flags = $" << hex << dwFileSystemFlags
			 << endl
			 << L"File system name = " << szFileSystemName << endl;
	}
	/*
	// Создание файла для сохранения образа.
	if ((file = CreateFileA("D:\\partition.img",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL)) == INVALID_HANDLE_VALUE)
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(partition);
		system("pause");
		return (-4);
	}

	// Резервируем место на диске для сохранения образа раздела. Пока у нас файл 0 байт.
	//
	// Здесь мы передвинем указатель в файле за границу его конца на расстояние равное
	// длине нашего раздела. Тем самым мы увеличили размер файла (но это пока теоретически).
	if (!SetFilePointerEx(file, partitionInfo.PartitionLength, NULL, FILE_BEGIN))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(file);
		CloseHandle(partition);
		system("pause");
		return (-5);
	}
	// А вот здесь мы фиксируем новый размер файла физически.
	if (!SetEndOfFile(file))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(file);
		CloseHandle(partition);
		system("pause");
		return (-6);
	}


	// Устанавливаем размер буфера.
	bufferSize = diskGeometry.BytesPerSector * diskGeometry.SectorsPerTrack;
	// Выделение памяти для буфера указанного размера.
	buffer = new BYTE[bufferSize];

	// Переменные используемые для отображения процесса копирования.
	int p = 0;
	__int64 s = 0;
	__int64 t = (partitionInfo.PartitionLength.QuadPart / bufferSize) / 100;


	// Устанавливаем указатель на начало файла.
	SetFilePointer(file, 0, NULL, FILE_BEGIN);

	// Копирование раздела в файл.
	do
	{
		// Чтение секторов раздела.
		result = ReadFile(partition, buffer, bufferSize, &bytesReturned, NULL);
		if (!result)
		{
			cout << "Error: " << GetLastError() << endl;
			delete[] buffer;
			CloseHandle(file);
			CloseHandle(partition);
			system("pause");
			return (-7);
		}

		// Запись секторов в файл образа.
		result = WriteFile(file, buffer, bytesReturned, &bytesWritten, NULL);
		if (!result)
		{
			cout << "Error: " << GetLastError() << endl;
			delete[] buffer;
			CloseHandle(file);
			CloseHandle(partition);
			system("pause");
			return (-8);
		}

		if (!(s++ % (t)))
		{
			system("CLS");
			cout << "Copied " << p++ << " %" << endl;
		}
	}
	while (result && bytesReturned);
	*/
	cout << "Partition copied successfully!\n";

	delete[] buffer;
	//CloseHandle(file);
	CloseHandle(partition);

	system("pause");
	return 0;
}
