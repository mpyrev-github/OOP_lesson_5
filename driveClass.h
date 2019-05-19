#include <windows.h>
#include <string>

#pragma pack(push, 1)

typedef struct {
  BYTE Offset_OEM[3];           // �������� �� ����� �������� �������
  BYTE OEM_Name[8];             // ��� �������� �������
  BYTE BytesPerSector[2];       // ���������� ���� � �������
  BYTE SectorsPerCluster;       // ���������� �������� � ��������
  BYTE Offset_TotalSectors[26]; // �������� �� �������� ���������� �������� � ��
  ULONGLONG TotalSectors;       // ���������� �������� � �������� �������
} ntfsBootRecord;

#pragma pack(pop)

class driveClass {
protected:
	HANDLE fileHandle;				// ���������� ��������� ���������� (������� �����)

	BYTE *fsName;					// �������� �������� �������

	DWORD bytesPerSector;
	DWORD sectorsPerCluster;
	DWORD totalSectors;
	DWORD numOfClustersToRead;
	DWORD firstClusterToRead;

	ntfsBootRecord *currentRecord;	// ������� ����������� ������

    DWORD getBytesPerCluster();

    BYTE *readRecords(LARGE_INTEGER sectorOffset,DWORD bufferSize);

	void setAttributes();                 // ������ ��������� � ��������
	void printHexBuffer(BYTE * buffer);     // ����������� ������� � HEX ����
	void close();                           // �������� �����

public:
	driveClass(const WCHAR *fileName);                           		// �����������

	void setNumOfClustersToRead(DWORD &numOfClusters);
	void setFirstClusterToRead(DWORD &firstCluster);
	void readClusters();                            // ����� ������������ ���������
	void getAttributes();

	DWORD getTotalClusters();

	bool checkBootRecord(const WCHAR *fileName);    // �������� �� ������������ �� � NTFS

	~driveClass();                                  // ����������
};


