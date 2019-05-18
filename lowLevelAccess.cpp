#pragma hdrstop
#pragma argsused

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>

#include "driveClass.h"

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;

	// Узнаем доступные диски и выводим их на экран
	DWORD driveType;
	WCHAR driveList[MAX_PATH];
	WCHAR * drive;
	GetLogicalDriveStringsW(MAX_PATH, (LPWSTR)driveList);
	drive = driveList;
	driveType = GetDriveTypeW(drive);

	cout << "Available drive letter [";
	// ----- Обработка первого диска -----
	if( *drive && (driveType == DRIVE_REMOVABLE || driveType == DRIVE_FIXED ) ) {
			wcout << drive[0];
	}
	drive += wcslen(drive) + 1;
	// -----------------------------------

	// Если дисков больше одного, то узнаем их буквы
	while (*drive) {
		driveType = GetDriveTypeW(drive);

		if(driveType == DRIVE_REMOVABLE or driveType == DRIVE_FIXED) {
			wcout << "," << drive[0];
		}
		drive += wcslen(drive) + 1;
	}

	cout << "].";

	// Узнаем у пользователя имя интересующего диска
	wstring diskLetter = L"";
	cout << endl << "Choose one: ";
	diskLetter += L"\\\\.\\";
	diskLetter += cin.get();
	diskLetter += L":";

	driveClass driveObj; // Создадим объект класса driveClass

	// Работа с свойствами и методами класса
	if (!driveObj.checkBootRecord(diskLetter.c_str())) {        // Передаем букву диска и проверяем NTFS ли
		 exit(-1);      // Если нет, то закрываем программу
	} else {
		cout << "File system name:" << driveObj.getFsName() << endl
			 << "Bytes per sector:" << driveObj.getBytesPerSector() << endl
			 << "Sectors per cluster:" << driveObj.getSectorsPerCluster() << endl
			 << "Bytes per cluster:" << driveObj.getBytesPerCluster() << endl
			 << "Total clusters:" << driveObj.getTotalClusters() << endl
			 << "How many clusters do you want to read?" << endl;
		DWORD numOfClustersBuf;
		cin >> numOfClustersBuf;
		driveObj.setNumOfClustersToRead(numOfClustersBuf);        // Узнаем у пользователя количество интересующих кластеров
		cout <<	"Where to start? Available clusters: [0, "
			 << driveObj.getTotalClusters()<<"]" << endl;
		DWORD firstClusterBuf;
		cin >> firstClusterBuf;
		driveObj.setFirstClusterToRead(firstClusterBuf);        // Узнаем у пользователя с какого кластера начать
		driveObj.readClusters();
	}

	system("pause");
	return 0;
}
