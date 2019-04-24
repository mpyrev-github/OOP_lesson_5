#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>
#include <iostream>
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;

    // ���������� ��������� ���������� (������� �����).
	HANDLE partition = INVALID_HANDLE_VALUE;
	// �������� � �������.
	PARTITION_INFORMATION partitionInfo = {0};
	// �������� � ��������� �����, �� ������� ���������� ������.
	DISK_GEOMETRY diskGeometry = {0};

	// ���������� ����� ��� ���������� ������ �������.
	HANDLE file = INVALID_HANDLE_VALUE;

	// ����� ��� ������.
	BYTE* buffer = NULL;
	// ������ ������.
	DWORD bufferSize = 0;

	// ���������� ������������ ����.
	DWORD bytesReturned = 0;
	// ���������� ���������� ����.
	DWORD bytesWritten = 0;

	// ��������� ����������.
	BOOL result = FALSE;

	// ��������� ������ �����.
	if ((partition = CreateFileA("\\\\.\\H:",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL)) == INVALID_HANDLE_VALUE)
	{
		cout << "Error: " << GetLastError() << endl;
		system("pause");
		return (-1);
	}

	// ����������� �������� � ��������� �����, �� ������� ���������� ������.
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

	// ����������� �������� � �������.
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

	// ������ �������.
	cout << "Partition's size (in bytes): " << partitionInfo.PartitionLength.QuadPart << endl;
	// ������ ����� ���������� �� ������� ������� (������, ����� ����������� ����������).
	cout << "Disk's size (in bytes):      " << (diskGeometry.Cylinders.QuadPart) *
		diskGeometry.TracksPerCylinder * diskGeometry.SectorsPerTrack *
		diskGeometry.BytesPerSector << endl;
	cout << "Num of cylinders:    " << diskGeometry.Cylinders.QuadPart << endl;
	cout << "Tracks per cylinder: " << diskGeometry.TracksPerCylinder << endl;
	cout << "Sectors per track:   " << diskGeometry.SectorsPerTrack << endl;
	cout << "Bytes per sector:    " << diskGeometry.BytesPerSector << endl;
	system("PAUSE");

	// �������� ����� ��� ���������� ������.
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


	// ����������� ����� �� ����� ��� ���������� ������ �������. ���� � ��� ���� 0 ����.
	//
	// ����� �� ���������� ��������� � ����� �� ������� ��� ����� �� ���������� ������
	// ����� ������ �������. ��� ����� �� ��������� ������ ����� (�� ��� ���� ������������).
	if (!SetFilePointerEx(file, partitionInfo.PartitionLength, NULL, FILE_BEGIN))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(file);
		CloseHandle(partition);
		system("pause");
		return (-5);
	}
	// � ��� ����� �� ��������� ����� ������ ����� ���������.
	if (!SetEndOfFile(file))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(file);
		CloseHandle(partition);
		system("pause");
		return (-6);
	}


	// ������������� ������ ������.
	bufferSize = diskGeometry.BytesPerSector * diskGeometry.SectorsPerTrack;
	// ��������� ������ ��� ������ ���������� �������.
	buffer = new BYTE[bufferSize];

	// ���������� ������������ ��� ����������� �������� �����������.
	int p = 0;
	__int64 s = 0;
	__int64 t = (partitionInfo.PartitionLength.QuadPart / bufferSize) / 100;


	// ������������� ��������� �� ������ �����.
	SetFilePointer(file, 0, NULL, FILE_BEGIN);

	// ����������� ������� � ����.
	do
	{
		// ������ �������� �������.
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

		// ������ �������� � ���� ������.
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

	cout << "Partition copied successfully!\n";

	delete[] buffer;
	CloseHandle(file);
	CloseHandle(partition);

	system("pause");
	return 0;
}
