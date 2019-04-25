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
	HANDLE file = INVALID_HANDLE_VALUE;

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
	string diskLetter = "";
	cout << "Enter disk letter:\n";
	diskLetter += "\\\\.\\";
	diskLetter += cin.get();
	diskLetter += ":";

	// Открываем раздел диска.
	if ((partition = CreateFileA(diskLetter.c_str(),
		GENERIC_READ,       // Режим доступа
		FILE_SHARE_READ | FILE_SHARE_WRITE,     // Режим совместной работы
		NULL,       // Атрибуты безопасности
		OPEN_EXISTING,      // Способ открытия
		0,      // Флаги и атрибуты
		NULL)) == INVALID_HANDLE_VALUE)     // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ
	{
		cout << "Read disk (" << diskLetter.c_str() <<")... can`t open!" << endl;
		cout << "Error: " << GetLastError() << endl;
		system("pause");
		return (-1);
	}

	// Запрашиваем сведения о геометрии диска, на котором расположен раздел.
	if (!DeviceIoControl(partition,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		&diskGeometry,
		sizeof (DISK_GEOMETRY),
		&bytesReturned,
		(LPOVERLAPPED)NULL))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(partition);
		system("pause");
		return (-2);
	}

	// Запрашиваем сведения о разделе.
	if (!DeviceIoControl(partition,
		IOCTL_DISK_GET_PARTITION_INFO,
		NULL,
		0,
		&partitionInfo,
		sizeof (PARTITION_INFORMATION),
		&bytesReturned,
		(LPOVERLAPPED)NULL))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(partition);
		system("pause");
		return (-3);
	}

	// Размер раздела.
	cout << "Partition's size (in bytes): " << partitionInfo.PartitionLength.QuadPart << endl;
	// Размер диска отличается от размера раздела (причем, может значительно отличаться).
	cout << "Disk's size (in bytes):      " << (diskGeometry.Cylinders.QuadPart) *
		diskGeometry.TracksPerCylinder * diskGeometry.SectorsPerTrack *
		diskGeometry.BytesPerSector << endl;
	cout << "Num of cylinders:    " << diskGeometry.Cylinders.QuadPart << endl;
	cout << "Tracks per cylinder: " << diskGeometry.TracksPerCylinder << endl;
	cout << "Sectors per track:   " << diskGeometry.SectorsPerTrack << endl;
	cout << "Bytes per sector:    " << diskGeometry.BytesPerSector << endl;
	system("PAUSE");
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
	CloseHandle(file);
	CloseHandle(partition);

	system("pause");
	return 0;
}
