#include <iostream>
#include <locale>
#include <Windows.h>

using namespace std;

void disksList();

int bin(DWORD logic, int driverCount);

boolean iHopeDiskExists(char driverName);

void diskInfo();

void crDelDirectory();

void crFile();

void copyFile();

void moveFile();

void fileAttribute();

void setAttribute();

void fileInfo();

void modifyDate();

string validPath();

int main() {
	setlocale(LC_ALL, "Russian");
	int pickme;
	do
	{
		system("cls");
		cout << "����� ���������� � ���� ���������� ��!\n\n";
		cout << "1 - ����� ������ ������, ������������ � �������\n2 - ����� ���������� � ����� � ��������� �����\n";
		cout << "3 - �������� � �������� ���������\n4 - �������� �����\n";
		cout << "5 - ����������� �����\n6 - ����������� �����\n";
		cout << "7 - �������� �����\n8 - ��������� ��������� �����\n";
		cout << "9 - ���������� � �����\n10 - ��������� ���� � ������� �����\n";
		cout << "0 - ����� �� ���������\n";
		cout << "\n������� �����: ";
		cin >> pickme;

		switch (pickme)
		{
		case 1:
			system("cls");
			disksList();
			system("pause");
			break;
		case 2:
			system("cls");
			diskInfo();
			system("pause");
			break;
		case 3:
			system("cls");
			crDelDirectory();
			system("pause");
			break;
		case 4:
			system("cls");
			crFile();
			system("pause");
			break;
		case 5:
			system("cls");
			copyFile();
			system("pause");
			break;
		case 6:
			system("cls");
			moveFile();
			system("pause");
			break;
		case 7:
			system("cls");
			fileAttribute();
			system("pause");
			break;
		case 8:
			system("cls");
			setAttribute();
			system("pause");
			break;
		case 9:
			system("cls");
			fileInfo();
			system("pause");
		case 10:
			system("cls");
			modifyDate();
			system("pause");
		}

	} while (pickme != 0);
}

int bin(DWORD logic,int driverCount) {
	while(logic / 2 != 0) {
		logic /= 2;
		driverCount += logic % 2;
	}
	return driverCount;
}

void disks(DWORD trueBuf, int driverCount, char *lpBuffer) {
	cout << "����� �� ����������: ";
	for (int i = 0; i < driverCount * trueBuf; i++) {
		printf("%c", lpBuffer[i]);
	}
	printf("\n");
}

void disksList() {
	int driverCount = 0;

	DWORD logic = GetLogicalDrives();
	//cout << "��������� ������ ������� GetLogicalDrives: " << logic << "\n";
	//������� ���������� ������
	driverCount = bin(logic, driverCount);
	//cchBuffer - ������������ ����� ������
	//MAX_PATH ������������ ����� ����, �������� ������ 260
	DWORD cchBuffer = MAX_PATH;
	//���� ����� �������� ���������� ������ �
	//�������� �������� ������� ���. ����� �������
	char lpBuffer[520] = { 0 };
	DWORD trueBuf = GetLogicalDriveStrings(cchBuffer, (LPWSTR)lpBuffer);
	//cout << "��������� ������ ������� GetLogicalDriveStrings: " << trueBuf << "\n";
	disks(trueBuf, driverCount, lpBuffer);
}

void diskInfo() {
	char driverName;
	boolean flag;
	disksList();
	cout << "�������� ����, ���������� � ������� ������ ��������. (������ �����: D)\n";
	cin >> driverName;
	flag = iHopeDiskExists(driverName);
	if (flag) {
		//�������������� ��������� ������
		//��� ������ ������� GetDriveType()
		char disk[4] = {' ', ':', '\\'};
		UINT id;
		disk[0] = driverName;

		cout << '\n';
		id = GetDriveTypeA(disk);
		switch (id) {
		case 0:
			cout << "��� ����� " << disk << " ����������!\n";
			break;
		case 1:
			cout << "�� ������� ����� ���� " << disk <<" !\n";
			break;
		case DRIVE_REMOVABLE:
			cout << "���� " << disk <<" ������.\n";
			break;
		case DRIVE_FIXED:
			cout << "���� " << disk << " �������.\n";
			break;
		case DRIVE_REMOTE:
			cout << "���� " << disk << " �������.\n";
			break;
		case DRIVE_CDROM:
			cout << disk << " ������� ����.\n";
			break;
		case DRIVE_RAMDISK:
			cout << disk << " RAM ����.\n";
			break;
		}

		//��������� ��������� ���� � ����
		
		//���� disk NULL
		//������������ ������ �������� ��������

		//��������� �� �����, ���������� �������� ����
		char VNameBuffer[MAX_PATH];
		
		char SysNameBuffer[MAX_PATH];
		//�������� ����� �����
		DWORD VSNumber;
		DWORD MCLength;
		//��������� �� ����������, ������� �������� �����, ��������� � ��������� �������� ��������
		DWORD FileSF;

		if (GetVolumeInformationA(disk, VNameBuffer, sizeof(VNameBuffer),
			&VSNumber, &MCLength, &FileSF, SysNameBuffer, sizeof(SysNameBuffer)))
		{
			cout << "��� ����:  " << VNameBuffer << "\n";
			cout << "�������� �������� ���������: " << SysNameBuffer << "\n";
			cout << "�������� ����� �����: " << VSNumber << "\n";
			cout << "������������ ����� ����� �����: " << MCLength << "\n";
			if (FileSF & FS_CASE_IS_PRESERVED)
				cout << "��� ������ �� ���� ����������� ������� ���� � ����� �����\n";
			if (FileSF & FS_CASE_SENSITIVE)
				cout << "�C ������������ ����� ������ � ������ �������� ���� � ������\n";
			if (FileSF & FS_UNICODE_STORED_ON_DISK)
				cout << "�C ������������ �������� �� ����� ���� ������ � UNICODE\n";
			if (FileSF & FILE_PERSISTENT_ACLS)
				cout << "�C �������� ����������� � ACL\n";
			if (FileSF & FILE_FILE_COMPRESSION)
				cout << "�� ������������ ������ ������\n";
			if (FileSF & FILE_NAMED_STREAMS)
				cout << "�� ������������ ����������� ������\n";
			if (FileSF & FILE_SUPPORTS_ENCRYPTION)
				cout << "�� ������������ Encrypted File System (EPS)\n";
			if (FileSF & FILE_SUPPORTS_OBJECT_IDS)
				cout << "�� ������������ �������������� ��������\n";
			if (FileSF & FILE_SUPPORTS_REPARSE_POINTS)
				cout << "�� ������������ ����� ���������� �������\n";
			if (FileSF & FILE_SUPPORTS_SPARSE_FILES)
				cout << "�� ������������ ����������� �����\n";
			if (FileSF & FILE_VOLUME_QUOTAS)
				cout << "�� ������������ VOLUME QUOTAS\n";
		}
		else
			cout << "�� ������� �������� ���������� � �����\n";
		DWORD SectorsPerCluster;
		DWORD BytesPerSector;
		DWORD NumberOfFreeClusters;
		DWORD TotalNumberOfClusters;

		if (GetDiskFreeSpaceA(disk, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters))
		{
			cout << "�������� � ��������: " << SectorsPerCluster << "\n";
			cout << "���� � �������: " << BytesPerSector << "\n";
			cout << "��������� ��������� �� �����: " << NumberOfFreeClusters << "\n";
			cout << "��������� �� �����: " << TotalNumberOfClusters << "\n";
			//cout << "����� ����� ����� �� �����: " << double(SectorsPerCluster) * double(BytesPerSector) * double(TotalNumberOfClusters) / 1024 / 1024 / 1024 << " ��" << endl;
			//cout << "���������� ���������� ����� �� �����: \n";
			//cout << double(SectorsPerCluster) * double(BytesPerSector) * double(NumberOfFreeClusters) / 1024 / 1024 / 1024 << " ��" << endl;
		}
		else
			cout << "�� ������� �������� ���������� � ��������� ������������ �� �����\n";
	}
	else {
		cout << "���� �� ����������!\n";
	}
}

boolean iHopeDiskExists(char driverName) {
	int driverCount = 0;

	DWORD logic = GetLogicalDrives();
	driverCount = bin(logic, driverCount);
	DWORD cchBuffer = MAX_PATH;
	char lpBuffer[520] = { 0 };
	DWORD trueBuf = GetLogicalDriveStrings(cchBuffer, (LPWSTR)lpBuffer);

	//�������� ���������� ����� �� ������������� � �������
	if (driverName >= 65 && driverName <= 90) {
		for (int i = 0; i < driverCount * trueBuf; i++) {
			if (lpBuffer[i] == driverName) {
				return true;
			}
			i++;
		}
	}
	return false;
}

void copyFile() {
	string pathToFile, pathToNewFile;

	system("cls");
	cout << "������� ���� � ����������� �����: ";
	pathToFile = validPath();
	cout << "������� ���� � ����� ��� ��� �������������� �����: ";
	cin >> pathToNewFile;
	if (CopyFileA(pathToFile.c_str(), pathToNewFile.c_str(), TRUE)) {
		cout << "���� ������� ���������� � " << pathToNewFile << "\n";
	}
	else cout << "����������� �� �������.\n��������, ������������� ���� �� ���������� ���� ��� ����������..\n";
};

void moveFile() {
	string pathToFile, newLocation;

	system("cls");
	cout << "������� ���� � �����, ������� ������� �����������: ";
	pathToFile = validPath();
	cout << "������� ����� ���� � �����, � ��������� �����: ";
	cin >> newLocation;
	//MOVEFILE_COPY_ALLOWED ��������� ��������� ���� � ������ ���
	//���� MOVEFILE_WRITE_THROUGH ��������� ������� �� ���������� �������� ���� ���� �� �����
	//��������� �� ����
	if (MoveFileExA(pathToFile.c_str(), newLocation.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
		cout << "���� ������� ���������.\n";
	else cout << "����������� ���� �� �������. ��������, �� ��������� ���� ��� ���������� ����.";
};

void crFile() {
	string pathCr;
	system("cls");
	cout << "������� ���� ������ � ������ ����� ��� ��� ��������: ";
	cin >> pathCr;
	//1 �������� - ��������� �� ��� �����, 2 - ������ ������� �� ������ � ������
	//3 - ���������� �� ���������� ������������� ����� ����������� ����������
	//6 - ���� ������ ��������� ��� ������������� 7 - �������� �������� ����� (readonly)
	//� ������ ������ �������� ���������� ����� 
	//������ - �������� �������� INVALID_HANDLE_VALUE
	CreateFileA(pathCr.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	cout << "���� ������!" << "\n";
}

void crDelDirectory() {
	char pickme;

	do {
		system("cls");
		cout << "1 - �������� ��������\n2 - �������� ��������\n0 - ����������� � ����\n";
		cout << "������� �����: ";
		cin >> pickme;
		switch (pickme)
		{
		case '1': {
			string dirName;
			string pathCr;
			system("cls");
			cout << "������� ���� ��� �������� ��������: ";
			pathCr = validPath();
			cout << "������� �������� ��������: ";
			cin >> dirName;
			pathCr += dirName;
			if (CreateDirectoryA(pathCr.c_str(), NULL)) {
				cout << "������� " << pathCr << " ������!\n";
			}
			else {
				cout << "������� " << pathCr << " ��� ����������!\n";
			}
			system("pause");
			break;
		}
		case '2': {
			string pathDel;
			system("cls");
			cout << "������� ���� � ��������: ";
			pathDel = validPath();
			pathDel += "\\\\";
			//��� �������� ���������� ����� �������� != 0
			//�������� �������� ������ ��� ������ ����������
			if (RemoveDirectoryA(pathDel.c_str())) cout << "\n������� �����!\n";
			else cout << "\n������� �� ������� �������...\n";
			system("pause");
			break;
		}
		}
	} while (pickme != '0');
}

void fileAttribute() {
	string path;
	DWORD attributeAnalysis;
	cout << "������� ���� � �����: ";
	cin >> path;
	attributeAnalysis = GetFileAttributesA(path.c_str());
	if (attributeAnalysis != INVALID_FILE_ATTRIBUTES) {
		if (attributeAnalysis & FILE_ATTRIBUTE_NORMAL)
			cout << "���� �� ����� ������ ������������� ���������\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_COMPRESSED)
			cout << "���� ������\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_DIRECTORY)
			cout << "���� ��������������� ��� �������\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_ENCRYPTED)
			cout << "���� ����������\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_HIDDEN)
			cout << "���� �����\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_READONLY)
			cout << "���� ������ ��� ������\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_SPARSE_FILE)
			cout << "���� - �����������\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_SYSTEM)
			cout << "���� �������� ��� ��������� ������������ ������������ ��������.\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_TEMPORARY)
			cout << "���� ������������ ��� ���������� ��������\n";
	}
};

void setAttribute() {
	char flag;
	int error = 0;
	HANDLE fHandle;
	string path;
	USHORT my = COMPRESSION_FORMAT_DEFAULT;
	LPDWORD BytesReturned = new DWORD;
	DWORD fAttr = 0;
	DWORD fA;
	cout << "������� ���� � ����� �������� �������� ������ ��������: ";
	path = validPath();
	cout << "\n���������� ������� '����������'\n+ - ��\n����� ������ ������ - ���\n";
	cin >> flag;
	if (flag == '+')
	{
		fA = GetFileAttributesA(path.c_str());
		if (fA & FILE_ATTRIBUTE_COMPRESSED)
			cout << "\n���� ����, ����������� ��� ����������..\n";
		else
		{
			//���������� ����� ��� ��������
			if (EncryptFileA(path.c_str()))
				cout << "\n���������� ����� ���������.\n";
			else
				cout << "\n�� ������� ����������� ����!\n";
		}
	}
	else
	{
		fA = GetFileAttributesA(path.c_str());
		if (fA & FILE_ATTRIBUTE_ENCRYPTED)
			//����������� ����� ��� ��������
			DecryptFileA(path.c_str(), fA);
	}
	cout << "\n���������� ������� '�������'?\n+ - ��\n";
	cin >> flag;
	if (flag == '+')
		fAttr |= FILE_ATTRIBUTE_HIDDEN;
	cout << "\n���������� ������� '������ ��� ������'?\n+ - ��\n";
	cin >> flag;
	if (flag == '1') {
		SetFileAttributesA(path.c_str(), fAttr);
		fAttr |= FILE_ATTRIBUTE_READONLY;
	}
	cout << "\n���������� ������� '��������� ����'?\n+ - ��\n";
	cin >> flag;
	if (flag == '1')
		fAttr |= FILE_ATTRIBUTE_SYSTEM;
	cout << "\n���������� ������� '��� ���������� ��������'?\n+ - ��\n";
	cin >> flag;
	if (flag == '1')
		fAttr |= FILE_ATTRIBUTE_TEMPORARY;
	//��������� ��������� ���������
	//��� �������� ���������� �������
	//���������� ��������� ��������
	if (SetFileAttributesA(path.c_str(), fAttr))
		cout << "\n�������� ��� ����� ������� �����������.\n";
};

void fileInfo() {
	HANDLE fHandle;
	BY_HANDLE_FILE_INFORMATION FileInfo;
	SYSTEMTIME sTime, lTime;
	string path;
	cout << "������� ���� � �����: ";
	path = validPath();
	fHandle = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileInformationByHandle(fHandle, &FileInfo);
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		cout << "���� �� ����� ������ ������������� ���������\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
		cout << "���� ������\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		cout << "���� ��������������� ��� �������\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
		cout << "���� ����������\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		cout << "���� �����\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		cout << "���� ������ ��� ������\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
		cout << "���� - �����������\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		cout << "���� �������� ��� ������������� ������������ ������������ �������� (��������� ����)\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
		cout << "���� ������������ ��� ���������� ��������\n";
	FileTimeToSystemTime(&FileInfo.ftCreationTime, &sTime);
	SystemTimeToTzSpecificLocalTime(NULL, &sTime, &lTime);
	cout << "\n����� �������� �����: \n" << sTime.wDay << ".";
	cout << lTime.wMonth << "." << lTime.wYear << " " << lTime.wHour << ":" << lTime.wMinute << "\n";
	FileTimeToSystemTime(&(FileInfo.ftLastWriteTime), &sTime);
	SystemTimeToTzSpecificLocalTimeEx(NULL, &sTime, &lTime);
	cout << "����� ��������� ������ � ����: \n" << lTime.wDay << ".";
	cout << lTime.wMonth << "." << lTime.wYear << " " << lTime.wHour << ":" << lTime.wMinute << "\n";
	cout << "���������� ����� ����, ����������� ����: " << FileInfo.dwVolumeSerialNumber << "\n";
	cout << "������� ����� ������� �����: " << FileInfo.nFileSizeHigh << "\n";
	cout << "������� ����� ������� �����: " << FileInfo.nFileSizeLow << "\n";
	cout << "����� ������ � ���� ������: " << FileInfo.nNumberOfLinks << "\n";
	cout << "������� ����� ����������� �������������� ����� � ������: " << FileInfo.nFileIndexHigh << "\n";
	cout << "������� ����� ����������� �������������� ����� � ������: " << FileInfo.nFileIndexLow << "\n";
	CloseHandle(fHandle);
};

void modifyDate() {
	HANDLE fHandle;
	TIME_ZONE_INFORMATION Tz;
	int Error;
	FILETIME fTimeCr, fTimeAc, fTimeWr;
	SYSTEMTIME sTimeCr = {}, sTimeAc = {}, sTimeWr = {}, UTimeCr = {}, UTimeAc = {}, UTimeWr = {};
	string path;
	cout << "������� ���� � ����� ���� � ����� �������� ������ ��������: ";
	path = validPath();
	fHandle = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WORD Year, Month, Day, Hour, Minute;
	cout << "������� ����� �������� �����.\n";
	cout << "\n������� ���: ";
	cin >> Year;
	sTimeCr.wYear = Year;
	cout << "\n������� �����: ";
	cin >> Month;
	sTimeCr.wMonth = Month;
	cout << "\n������� ����: ";
	cin >> Day;
	sTimeCr.wDay = Day;
	cout << "\n������� ���: ";
	cin >> Hour;
	sTimeCr.wHour = Hour;
	cout << "\n������� ������: ";
	cin >> Minute;
	sTimeCr.wMinute = Minute;
	GetTimeZoneInformation(&Tz);
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeCr, &UTimeCr);
	SystemTimeToFileTime(&UTimeCr, &fTimeCr);
	cout << "\n������� ����� ������ ��� ������ � ����. ";
	cout << "\n������� ���: ";
	cin >> Year;
	sTimeAc.wYear = Year;
	cout << "\n������� �����: ";
	cin >> Month;
	sTimeAc.wMonth = Month;
	cout << "\n������� ����: ";
	cin >> Day;
	sTimeAc.wDay = Day;
	cout << "\n������� ���: ";
	cin >> Hour;
	sTimeAc.wHour = Hour;
	cout << "\n������� ������: ";
	cin >> Minute;
	sTimeAc.wMinute = Minute;
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeAc, &UTimeAc);
	SystemTimeToFileTime(&UTimeAc, &fTimeAc);
	cout << "������� ����� ������ � ����.";
	cout << "\n������� ���: ";
	cin >> Year;
	sTimeWr.wYear = Year;
	cout << "\n������� �����: ";
	cin >> Month;
	sTimeWr.wMonth = Month;
	cout << "\n������� ����: ";
	cin >> Day;
	sTimeWr.wDay = Day;
	cout << "\n������� ��� \: ";
	cin >> Hour;
	sTimeWr.wHour = Hour;
	cout << "\n������� ������: ";
	cin >> Minute;
	sTimeWr.wMinute = Minute;
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeWr, &UTimeWr);
	SystemTimeToFileTime(&UTimeWr, &fTimeWr);
	if (SetFileTime(fHandle, &fTimeCr, &fTimeAc, &fTimeWr))
		cout << "\n���� � ����� ������� ��������!";
	CloseHandle(fHandle);
};

string validPath()
{
	int error;
	string path;
	do
	{
		error = 0;
		path.clear();
		cin >> path;
		//��� �������� ���������� ������������ �������� - �������� ����� ��� ��������
		if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES)
			error = GetLastError();
		if (error == 3 || error == 2) {
			cout << "�� ������� ����� ���� ��� ������� �� ���������� ����.\n";
			cout << "������� ���� ��� ���: ";
		}
	} while (error != 0);
	return path;
}
