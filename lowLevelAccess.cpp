#pragma hdrstop
#pragma argsused

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>

#include "driveClass.h"
#include "fsClass.h"
#include "factoryClass.h"

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

	driveClass* driveObj = new driveClass(diskLetter.c_str()); // Создадим объект класса driveClass и передаем букву диска в конструктор
	fsClass* fsObj = factoryClass::objCreator(driveObj);    // Запустим фабрику factoryClass

	// Работа с методами объектов класса fsClass
	fsObj->getAttributes();

	cout << "How many clusters do you want to read?" << endl;
	DWORD numOfClustersBuf;
	cin >> numOfClustersBuf;
	fsObj->setNumOfClustersToRead(numOfClustersBuf);        // Узнаем у пользователя количество интересующих кластеров
	cout <<	"Where to start? Available clusters: [0, "
		 << fsObj->getTotalClusters()-1<<"]" << endl;
	DWORD firstClusterBuf;
	cin >> firstClusterBuf;
	fsObj->setFirstClusterToRead(firstClusterBuf);        // Узнаем у пользователя с какого кластера начать
	fsObj->readClusters(driveObj->getFileHandle());

	system("pause");
	return 0;
}
