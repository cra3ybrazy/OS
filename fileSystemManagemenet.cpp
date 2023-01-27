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
		cout << "Äîáðî ïîæàëîâàòü â ìåíþ óïðàâëåíèÿ ÔÑ!\n\n";
		cout << "1 - Âûâîä ñïèñêà äèñêîâ, ñóùåñòâóþùèõ â ñèñòåìå\n2 - Âûâîä èíôîðìàöèè î äèñêå è ñâîáîäíîì ìåñòå\n";
		cout << "3 - Ñîçäàíèå è óäàëåíèå êàòàëîãîâ\n4 - Ñîçäàíèå ôàéëà\n";
		cout << "5 - Êîïèðîâàíèå ôàéëà\n6 - Ïåðåìåùåíèå ôàéëà\n";
		cout << "7 - Àòðèáóòû ôàéëà\n8 - Èçìåíåíèå àòðèáóòîâ ôàéëà\n";
		cout << "9 - Èíôîðìàöèÿ î ôàéëå\n10 - Èçìåíåíèå äàòû è âðåìåíè ôàéëà\n";
		cout << "0 - Âûõîä èç ïðîãðàììû\n";
		cout << "\nÂâåäèòå ÷èñëî: ";
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
	cout << "Äèñêè íà óñòðîéñòâå: ";
	for (int i = 0; i < driverCount * trueBuf; i++) {
		printf("%c", lpBuffer[i]);
	}
	printf("\n");
}

void disksList() {
	int driverCount = 0;

	DWORD logic = GetLogicalDrives();
	//cout << "Ðåçóëüòàò âûçîâà ôóíêöèè GetLogicalDrives: " << logic << "\n";
	//ïîäñ÷åò êîëè÷åñòâà äèñêîâ
	driverCount = bin(logic, driverCount);
	//cchBuffer - ìàêñèìàëüíàÿ äëèíà áóôåðà
	//MAX_PATH ìàêñèìàëüíàÿ äëèíà ïóòè, ïàðàìåòð ðàâíûé 260
	DWORD cchBuffer = MAX_PATH;
	//ñþäà áóäóò ïîìåùåíû ñèìâîëüíûå äàííûå î
	//êîðíåâîì êàòàëîãå êàæäîãî ëîã. äèñêà ñèñòåìû
	char lpBuffer[520] = { 0 };
	DWORD trueBuf = GetLogicalDriveStrings(cchBuffer, (LPWSTR)lpBuffer);
	//cout << "Ðåçóëüòàò âûçîâà ôóíêöèè GetLogicalDriveStrings: " << trueBuf << "\n";
	disks(trueBuf, driverCount, lpBuffer);
}

void diskInfo() {
	char driverName;
	boolean flag;
	disksList();
	cout << "Âûáåðèòå äèñê, èíôîðìàöèþ î êîòîðîì õîòèòå ïîëó÷èòü. (Ïðèìåð ââîäà: D)\n";
	cin >> driverName;
	flag = iHopeDiskExists(driverName);
	if (flag) {
		//ïðåîáðàçîâàíèå ââåäåííûõ äàííûõ
		//äëÿ âûçîâà ôóíêöèè GetDriveType()
		char disk[4] = {' ', ':', '\\'};
		UINT id;
		disk[0] = driverName;

		cout << '\n';
		id = GetDriveTypeA(disk);
		switch (id) {
		case 0:
			cout << "Òèï äèñêà " << disk << " íåèçâåñòåí!\n";
			break;
		case 1:
			cout << "Íå óäàëîñü íàéòè äèñê " << disk <<" !\n";
			break;
		case DRIVE_REMOVABLE:
			cout << "Äèñê " << disk <<" ãèáêèé.\n";
			break;
		case DRIVE_FIXED:
			cout << "Äèñê " << disk << " æåñòêèé.\n";
			break;
		case DRIVE_REMOTE:
			cout << "Äèñê " << disk << " ñåòåâîé.\n";
			break;
		case DRIVE_CDROM:
			cout << disk << " êîìïàêò äèñê.\n";
			break;
		case DRIVE_RAMDISK:
			cout << disk << " RAM äèñê.\n";
			break;
		}

		//ïîëó÷åíèå ïîäðîáíîé èíôî î òîìå
		
		//åñëè disk NULL
		//èñïîëüçóåòñÿ êîðåíü òåêóùåãî êàòàëîãà

		//óêàçàòåëü íà áóôåð, ïîëó÷èâøèé íàçâàíèå òîìà
		char VNameBuffer[MAX_PATH];
		
		char SysNameBuffer[MAX_PATH];
		//ñåðèéíûé íîìåð äèñêà
		DWORD VSNumber;
		DWORD MCLength;
		//óêàçàòåëü íà ïåðåìåííóþ, êîòîðàÿ ïîëó÷àåò ôëàãè, ñâÿçàííûå ñ óêàçàííîé ôàéëîâîé ñèñòåìîé
		DWORD FileSF;

		if (GetVolumeInformationA(disk, VNameBuffer, sizeof(VNameBuffer),
			&VSNumber, &MCLength, &FileSF, SysNameBuffer, sizeof(SysNameBuffer)))
		{
			cout << "Èìÿ òîìà:  " << VNameBuffer << "\n";
			cout << "Íàçâàíèå ôàéëîâîé ñòðóêòóðû: " << SysNameBuffer << "\n";
			cout << "Ñåðèéíûé íîìåð äèñêà: " << VSNumber << "\n";
			cout << "Ìàêñèìàëüíàÿ äëèíà èìåíè ôàéëà: " << MCLength << "\n";
			if (FileSF & FS_CASE_IS_PRESERVED)
				cout << "Ïðè çàïèñè íà äèñê ñîõðàíÿåòñÿ ðåãèñòð áóêâ â èìåíè ôàéëà\n";
			if (FileSF & FS_CASE_SENSITIVE)
				cout << "ÔC ïîääåðæèâàåò ïîèñê ôàéëîâ ñ ó÷åòîì ðåãèñòðà áóêâ â èìåíàõ\n";
			if (FileSF & FS_UNICODE_STORED_ON_DISK)
				cout << "ÔC ïîääåðæèâàåò õðàíåíèå íà äèñêå èìåí ôàéëîâ â UNICODE\n";
			if (FileSF & FILE_PERSISTENT_ACLS)
				cout << "ÔC ñïîñîáíà îïåðèðîâàòü ñ ACL\n";
			if (FileSF & FILE_FILE_COMPRESSION)
				cout << "ÔÑ ïîääåðæèâàåò ñæàòèå ôàéëîâ\n";
			if (FileSF & FILE_NAMED_STREAMS)
				cout << "ÔÑ ïîääåðæèâàåò èìåíîâàííûå ïîòîêè\n";
			if (FileSF & FILE_SUPPORTS_ENCRYPTION)
				cout << "ÔÑ ïîääåðæèâàåò Encrypted File System (EPS)\n";
			if (FileSF & FILE_SUPPORTS_OBJECT_IDS)
				cout << "ÔÑ ïîääåðæèâàåò èäåíòèôèêàòîðû îáúåêòîâ\n";
			if (FileSF & FILE_SUPPORTS_REPARSE_POINTS)
				cout << "ÔÑ ïîääåðæèâàåò òî÷êè ïîâòîðíîãî ðàçáîðà\n";
			if (FileSF & FILE_SUPPORTS_SPARSE_FILES)
				cout << "ÔÑ ïîääåðæèâàåò ðàçðåæåííûå ôàéëû\n";
			if (FileSF & FILE_VOLUME_QUOTAS)
				cout << "ÔÑ ïîääåðæèâàåò VOLUME QUOTAS\n";
		}
		else
			cout << "Íå óäàëîñü ïîëó÷èòü èíôîðìàöèþ î äèñêå\n";
		DWORD SectorsPerCluster;
		DWORD BytesPerSector;
		DWORD NumberOfFreeClusters;
		DWORD TotalNumberOfClusters;

		if (GetDiskFreeSpaceA(disk, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters))
		{
			cout << "Ñåêòîðîâ â êëàñòåðå: " << SectorsPerCluster << "\n";
			cout << "Áàéò â ñåêòîðå: " << BytesPerSector << "\n";
			cout << "Ñâîáîäíûõ êëàñòåðîâ íà äèñêå: " << NumberOfFreeClusters << "\n";
			cout << "Êëàñòåðîâ íà äèñêå: " << TotalNumberOfClusters << "\n";
			//cout << "Îáùèé îáú¸ì ìåñòà íà äèñêå: " << double(SectorsPerCluster) * double(BytesPerSector) * double(TotalNumberOfClusters) / 1024 / 1024 / 1024 << " Ãá" << endl;
			//cout << "Êîëè÷åñòâî ñâîáîäíîãî ìåñòà íà äèñêå: \n";
			//cout << double(SectorsPerCluster) * double(BytesPerSector) * double(NumberOfFreeClusters) / 1024 / 1024 / 1024 << " Ãá" << endl;
		}
		else
			cout << "Íå óäàëîñü ïîëó÷èòü èíôîðìàöèþ î ñâîáîäíîì ïðîñòðàíñòâå íà äèñêå\n";
	}
	else {
		cout << "Äèñê íå ñóùåñòâóåò!\n";
	}
}

boolean iHopeDiskExists(char driverName) {
	int driverCount = 0;

	DWORD logic = GetLogicalDrives();
	driverCount = bin(logic, driverCount);
	DWORD cchBuffer = MAX_PATH;
	char lpBuffer[520] = { 0 };
	DWORD trueBuf = GetLogicalDriveStrings(cchBuffer, (LPWSTR)lpBuffer);

	//ïðîâåðêà ââåäåííîãî äèñêà íà ñóùåñòâîâàíèå â ñèñòåìå
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
	cout << "Óêàæèòå ïóòü ê êîïèðóåìîìó ôàéëó: ";
	pathToFile = validPath();
	cout << "Óêàæèòå ïóòü è íîâîå èìÿ äëÿ ñêîïèðîâàííîãî ôàéëà: ";
	cin >> pathToNewFile;
	if (CopyFileA(pathToFile.c_str(), pathToNewFile.c_str(), TRUE)) {
		cout << "Ôàéë óñïåøíî ñêîïèðîâàí â " << pathToNewFile << "\n";
	}
	else cout << "Êîïèðîâàíèå íå óäàëîñü.\nÂîçìîæíî, ñêîïèðîâàííûé ôàéë ïî óêàçàííîìó ïóòè óæå ñóùåñòâóåò..\n";
};

void moveFile() {
	string pathToFile, newLocation;

	system("cls");
	cout << "Óêàæèòå ïóòü ê ôàéëó, êîòîðûé æåëàåòå ïåðåìåñòèòü: ";
	pathToFile = validPath();
	cout << "Óêàæèòå íîâûé ïóòü ê ôàéëó, ñ óêàçàíèåì ôàéëà: ";
	cin >> newLocation;
	//MOVEFILE_COPY_ALLOWED ïîçâîëÿåò ïåðåíåñòè ôàéë â äðóãîé òîì
	//ôëàã MOVEFILE_WRITE_THROUGH ïðèçûâàåò ôóíêöèþ íå âîçâðàùàòü çíà÷åíèå ïîêà ôàéë íå áóäåò
	//ïåðåìåùåí íà äèñê
	if (MoveFileExA(pathToFile.c_str(), newLocation.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
		cout << "Ôàéë óñïåøíî ïåðåìåùåí.\n";
	else cout << "Ïåðåìåñòèòü ôàéë íå óäàëîñü. Âîçìîæíî, ïî óêàçííîìó ïóòè óæå ñóùåñòâóåò ôàéë.";
};

void crFile() {
	string pathCr;
	system("cls");
	cout << "Óêàæèòå ïóòü âìåñòå ñ èìåíåì ôàéëà äëÿ åãî ñîçäàíèÿ: ";
	cin >> pathCr;
	//1 ïàðàìåòð - óêàçàòåëü íà èìÿ ôàéëà, 2 - çàïðîñ äîñòóïà íà ÷òåíèå è çàïèñü
	//3 - ðàçðåøåíèå íà ñîâìåñòíîå èñïîëüçîâàíèå ôàéëà íåñêîëüêèìè ïðîöåññàìè
	//6 - ôàéë âñåãäà ñîçäàåòñÿ èëè ïåðåñîçäàåòñÿ 7 - àòðèáóòû ñîçäàíèÿ ôàéëà (readonly)
	//â ñëó÷àå óñïåõà âåðíåòñÿ äåñêðèïòîð ôàéëà 
	//îøèáêà - âåðíåòñÿ çíà÷åíèå INVALID_HANDLE_VALUE
	CreateFileA(pathCr.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	cout << "Ôàéë ñîçäàí!" << "\n";
}

void crDelDirectory() {
	char pickme;

	do {
		system("cls");
		cout << "1 - Ñîçäàíèå êàòàëîãà\n2 - Óäàëåíèå êàòàëîãà\n0 - Âîçâðàùåíèå â ìåíþ\n";
		cout << "Ââåäèòå ÷èñëî: ";
		cin >> pickme;
		switch (pickme)
		{
		case '1': {
			string dirName;
			string pathCr;
			system("cls");
			cout << "Ââåäèòå ïóòü äëÿ ñîçäàíèÿ êàòàëîãà: ";
			pathCr = validPath();
			cout << "Ââåäèòå íàçâàíèå êàòàëîãà: ";
			cin >> dirName;
			pathCr += dirName;
			if (CreateDirectoryA(pathCr.c_str(), NULL)) {
				cout << "Êàòàëîã " << pathCr << " ñîçäàí!\n";
			}
			else {
				cout << "Êàòàëîã " << pathCr << " óæå ñóùåñòâóåò!\n";
			}
			system("pause");
			break;
		}
		case '2': {
			string pathDel;
			system("cls");
			cout << "Ââåäèòå ïóòü ê êàòàëîãó: ";
			pathDel = validPath();
			pathDel += "\\\\";
			//ïðè óñïåøíîì âûïîëíåíèè âîçâð çíà÷åíèå != 0
			//óäàëåíèå âîçìîæíî òîëüêî äëÿ ïóñòûõ äèðåêòîðèé
			if (RemoveDirectoryA(pathDel.c_str())) cout << "\nÊàòàëîã óäàë¸í!\n";
			else cout << "\nÊàòàëîã íå óäàëîñü óäàëèòü...\n";
			system("pause");
			break;
		}
		}
	} while (pickme != '0');
}

void fileAttribute() {
	string path;
	DWORD attributeAnalysis;
	cout << "Óêàæèòå ïóòü ê ôàéëó: ";
	cin >> path;
	attributeAnalysis = GetFileAttributesA(path.c_str());
	if (attributeAnalysis != INVALID_FILE_ATTRIBUTES) {
		if (attributeAnalysis & FILE_ATTRIBUTE_NORMAL)
			cout << "Ôàéë íå èìååò äðóãèõ óñòàíîâëåííûõ àòðèáóòîâ\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_COMPRESSED)
			cout << "Ôàéë ñæàòûé\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_DIRECTORY)
			cout << "Ôàéë èäåíòèôèöèðîâàí êàê êàòàëîã\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_ENCRYPTED)
			cout << "Ôàéë çàøèôðîâàí\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_HIDDEN)
			cout << "Ôàéë ñêðûò\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_READONLY)
			cout << "Ôàéë òîëüêî äëÿ ÷òåíèÿ\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_SPARSE_FILE)
			cout << "Ôàéë - ðàçðåæåííûé\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_SYSTEM)
			cout << "Ôàéë ÷àñòè÷íî èëè ïîëíîñòüþ èñïîëüçóåòñÿ îïåðàöèîííîé ñèñòåìîé.\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_TEMPORARY)
			cout << "Ôàéë èñïîëüçóåòñÿ äëÿ âðåìåííîãî õðàíåíèÿ\n";
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
	cout << "Óêàæèòå ïóòü ê ôàéëó àòðèáóòû êîòîðîãî õîòèòå èçìåíèòü: ";
	path = validPath();
	cout << "\nÓñòàíîâèòü àòðèáóò 'çàøèôðîâàí'\n+ - Äà\nËþáîé äðóãîé ñèìâîë - íåò\n";
	cin >> flag;
	if (flag == '+')
	{
		fA = GetFileAttributesA(path.c_str());
		if (fA & FILE_ATTRIBUTE_COMPRESSED)
			cout << "\nÔàéë ñæàò, çàøèôðîâàòü åãî íåâîçìîæíî..\n";
		else
		{
			//øèôðîâàíèå ôàéëà èëè êàòàëîãà
			if (EncryptFileA(path.c_str()))
				cout << "\nØèôðîâàíèå ôàéëà çàâåðøåíî.\n";
			else
				cout << "\nÍå óäàëîñü çàøèôðîâàòü ôàéë!\n";
		}
	}
	else
	{
		fA = GetFileAttributesA(path.c_str());
		if (fA & FILE_ATTRIBUTE_ENCRYPTED)
			//ðàñøèôðîâêà ôàéëà èëè êàòàëîãà
			DecryptFileA(path.c_str(), fA);
	}
	cout << "\nÓñòàíîâèòü àòðèáóò 'ñêðûòûé'?\n+ - Äà\n";
	cin >> flag;
	if (flag == '+')
		fAttr |= FILE_ATTRIBUTE_HIDDEN;
	cout << "\nÓñòàíîâèòü àòðèáóò 'òîëüêî äëÿ ÷òåíèÿ'?\n+ - Äà\n";
	cin >> flag;
	if (flag == '1') {
		SetFileAttributesA(path.c_str(), fAttr);
		fAttr |= FILE_ATTRIBUTE_READONLY;
	}
	cout << "\nÓñòàíîâèòü àòðèáóò 'ñèñòåìíûé ôàéë'?\n+ - Äà\n";
	cin >> flag;
	if (flag == '1')
		fAttr |= FILE_ATTRIBUTE_SYSTEM;
	cout << "\nÓñòàíîâèòü àòðèáóò 'äëÿ âðåìåííîãî õðàíåíèÿ'?\n+ - Äà\n";
	cin >> flag;
	if (flag == '1')
		fAttr |= FILE_ATTRIBUTE_TEMPORARY;
	//óñòàíîâêà âûáðàííûõ àòðèáóòîâ
	//ïðè óñïåøíîì çàâåðøåíèè ôóíêöèÿ
	//âîçâðàùàåò íåíóëåâîå çíà÷åíèå
	if (SetFileAttributesA(path.c_str(), fAttr))
		cout << "\nÀòðèáóòû äëÿ ôàéëà óñïåøíî óñòàíîâëåíû.\n";
};

void fileInfo() {
	HANDLE fHandle;
	BY_HANDLE_FILE_INFORMATION FileInfo;
	SYSTEMTIME sTime, lTime;
	string path;
	cout << "Óêàæèòå ïóòü ê ôàéëó: ";
	path = validPath();
	fHandle = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileInformationByHandle(fHandle, &FileInfo);
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		cout << "Ôàéë íå èìååò äðóãèõ óñòàíîâëåííûõ àòðèáóòîâ\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
		cout << "Ôàéë ñæàòûé\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		cout << "Ôàéë èäåíòèôèöèðîâàí êàê êàòàëîã\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
		cout << "Ôàéë çàøèôðîâàí\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		cout << "Ôàéë ñêðûò\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		cout << "Ôàéë òîëüêî äëÿ ÷òåíèÿ\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
		cout << "Ôàéë - ðàçðåæåííûé\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		cout << "Ôàéë ÷àñòè÷íî èëè èñêëþ÷èòåëüíî èñïîëüçóåòñÿ îïåðàöèîííîé ñèñòåìîé (ñèñòåìíûé ôàéë)\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
		cout << "Ôàéë èñïîëüçóåòñÿ äëÿ âðåìåííîãî õðàíåíèÿ\n";
	FileTimeToSystemTime(&FileInfo.ftCreationTime, &sTime);
	SystemTimeToTzSpecificLocalTime(NULL, &sTime, &lTime);
	cout << "\nÂðåìÿ ñîçäàíèÿ ôàéëà: \n" << sTime.wDay << ".";
	cout << lTime.wMonth << "." << lTime.wYear << " " << lTime.wHour << ":" << lTime.wMinute << "\n";
	FileTimeToSystemTime(&(FileInfo.ftLastWriteTime), &sTime);
	SystemTimeToTzSpecificLocalTimeEx(NULL, &sTime, &lTime);
	cout << "Âðåìÿ ïîñëåäíåé çàïèñè â ôàéë: \n" << lTime.wDay << ".";
	cout << lTime.wMonth << "." << lTime.wYear << " " << lTime.wHour << ":" << lTime.wMinute << "\n";
	cout << "Ïîðÿäêîâûé íîìåð òîìà, ñîäåðæàùåãî ôàéë: " << FileInfo.dwVolumeSerialNumber << "\n";
	cout << "Ñòàðøàÿ ÷àñòü ðàçìåðà ôàéëà: " << FileInfo.nFileSizeHigh << "\n";
	cout << "Ìëàäøàÿ ÷àñòü ðàçìåðà ôàéëà: " << FileInfo.nFileSizeLow << "\n";
	cout << "×èñëî ñâÿçåé ñ ýòèì ôàéëîì: " << FileInfo.nNumberOfLinks << "\n";
	cout << "Ñòàðøàÿ ÷àñòü óíèêàëüíîãî èäåíòèôèêàòîðà ñâÿçè ñ ôàéëîì: " << FileInfo.nFileIndexHigh << "\n";
	cout << "Ìëàäøàÿ ÷àñòü óíèêàëüíîãî èäåíòèôèêàòîðà ñâÿçè ñ ôàéëîì: " << FileInfo.nFileIndexLow << "\n";
	CloseHandle(fHandle);
};

void modifyDate() {
	HANDLE fHandle;
	TIME_ZONE_INFORMATION Tz;
	int Error;
	FILETIME fTimeCr, fTimeAc, fTimeWr;
	SYSTEMTIME sTimeCr = {}, sTimeAc = {}, sTimeWr = {}, UTimeCr = {}, UTimeAc = {}, UTimeWr = {};
	string path;
	cout << "Ââåäèòå ïóòü ê ôàéëó äàòó è âðåìÿ êîòîðîãî õîòèòå èçìåíèòü: ";
	path = validPath();
	fHandle = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WORD Year, Month, Day, Hour, Minute;
	cout << "Ââåäèòå âðåìÿ ñîçäàíèÿ ôàéëà.\n";
	cout << "\nÂâåäèòå ãîä: ";
	cin >> Year;
	sTimeCr.wYear = Year;
	cout << "\nÂâåäèòå ìåñÿö: ";
	cin >> Month;
	sTimeCr.wMonth = Month;
	cout << "\nÂâåäèòå äåíü: ";
	cin >> Day;
	sTimeCr.wDay = Day;
	cout << "\nÂâåäèòå ÷àñ: ";
	cin >> Hour;
	sTimeCr.wHour = Hour;
	cout << "\nÂâåäèòå ìèíóòû: ";
	cin >> Minute;
	sTimeCr.wMinute = Minute;
	GetTimeZoneInformation(&Tz);
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeCr, &UTimeCr);
	SystemTimeToFileTime(&UTimeCr, &fTimeCr);
	cout << "\nÂâåäèòå âðåìÿ ÷òåíèÿ èëè çàïèñè â ôàéë. ";
	cout << "\nÂâåäèòå ãîä: ";
	cin >> Year;
	sTimeAc.wYear = Year;
	cout << "\nÂâåäèòå ìåñÿö: ";
	cin >> Month;
	sTimeAc.wMonth = Month;
	cout << "\nÂâåäèòå äåíü: ";
	cin >> Day;
	sTimeAc.wDay = Day;
	cout << "\nÂâåäèòå ÷àñ: ";
	cin >> Hour;
	sTimeAc.wHour = Hour;
	cout << "\nÂâåäèòå ìèíóòû: ";
	cin >> Minute;
	sTimeAc.wMinute = Minute;
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeAc, &UTimeAc);
	SystemTimeToFileTime(&UTimeAc, &fTimeAc);
	cout << "Ââåäèòå âðåìÿ çàïèñè â ôàéë.";
	cout << "\nÂâåäèòå ãîä: ";
	cin >> Year;
	sTimeWr.wYear = Year;
	cout << "\nÂâåäèòå ìåñÿö: ";
	cin >> Month;
	sTimeWr.wMonth = Month;
	cout << "\nÂâåäèòå äåíü: ";
	cin >> Day;
	sTimeWr.wDay = Day;
	cout << "\nÂâåäèòå ÷àñ \: ";
	cin >> Hour;
	sTimeWr.wHour = Hour;
	cout << "\nÂâåäèòå ìèíóòû: ";
	cin >> Minute;
	sTimeWr.wMinute = Minute;
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeWr, &UTimeWr);
	SystemTimeToFileTime(&UTimeWr, &fTimeWr);
	if (SetFileTime(fHandle, &fTimeCr, &fTimeAc, &fTimeWr))
		cout << "\nÄàòà è âðåìÿ óñïåøíî èçìåíåíû!";
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
		//ïðè óñïåøíîì çàâåðøåíèè âîçâðàùàåìîå çíà÷åíèå - àòðèáóòû ôàéëà èëè êàòàëîãà
		if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES)
			error = GetLastError();
		if (error == 3 || error == 2) {
			cout << "Íå óäàëîñü íàéòè ôàéë èëè êàòàëîã ïî óêàçàííîìó ïóòè.\n";
			cout << "Ââåäèòå ïóòü åù¸ ðàç: ";
		}
	} while (error != 0);
	return path;
	
}
