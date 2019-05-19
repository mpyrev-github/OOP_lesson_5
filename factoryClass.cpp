#include <iostream>
#include "factoryClass.h"
#include "ntfsClass.h"
#include "exfatClass.h"
#include "string.h"

using namespace std;

typedef struct
{
	BYTE Offset_OEM[3];           // Смещение до имени файловой системы
	char OEM_Name[8];             // Имя файловой системы
} fsOEMname;

factoryClass::factoryClass() { }

fsClass* factoryClass::objCreator(driveClass* driveObj)
{
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = 0;
	fsOEMname *currentOEM = (fsOEMname*)driveObj->readRecords(sectorOffset, 1024,driveObj->getFileHandle());

	if (strcmp(currentOEM->OEM_Name, "NTFS    ") == 0) {
		return new ntfsClass(driveObj);
	} else if (strcmp(currentOEM->OEM_Name, "EXFAT   ") == 0) {
				return new exfatClass(driveObj);
		} else {
				cout << "This file system doesn`t support!" << endl;   // Обработка несоответствия
				system("pause");
                return 0;
		}

}

factoryClass::~factoryClass() { }
