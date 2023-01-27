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
		cout << "Добро пожаловать в меню управления ФС!\n\n";
		cout << "1 - Вывод списка дисков, существующих в системе\n2 - Вывод информации о диске и свободном месте\n";
		cout << "3 - Создание и удаление каталогов\n4 - Создание файла\n";
		cout << "5 - Копирование файла\n6 - Перемещение файла\n";
		cout << "7 - Атрибуты файла\n8 - Изменение атрибутов файла\n";
		cout << "9 - Информация о файле\n10 - Изменение даты и времени файла\n";
		cout << "0 - Выход из программы\n";
		cout << "\nВведите число: ";
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
	cout << "Диски на устройстве: ";
	for (int i = 0; i < driverCount * trueBuf; i++) {
		printf("%c", lpBuffer[i]);
	}
	printf("\n");
}

void disksList() {
	int driverCount = 0;

	DWORD logic = GetLogicalDrives();
	//cout << "Результат вызова функции GetLogicalDrives: " << logic << "\n";
	//подсчет количества дисков
	driverCount = bin(logic, driverCount);
	//cchBuffer - максимальная длина буфера
	//MAX_PATH максимальная длина пути, параметр равный 260
	DWORD cchBuffer = MAX_PATH;
	//сюда будут помещены символьные данные о
	//корневом каталоге каждого лог. диска системы
	char lpBuffer[520] = { 0 };
	DWORD trueBuf = GetLogicalDriveStrings(cchBuffer, (LPWSTR)lpBuffer);
	//cout << "Результат вызова функции GetLogicalDriveStrings: " << trueBuf << "\n";
	disks(trueBuf, driverCount, lpBuffer);
}

void diskInfo() {
	char driverName;
	boolean flag;
	disksList();
	cout << "Выберите диск, информацию о котором хотите получить. (Пример ввода: D)\n";
	cin >> driverName;
	flag = iHopeDiskExists(driverName);
	if (flag) {
		//преобразование введенных данных
		//для вызова функции GetDriveType()
		char disk[4] = {' ', ':', '\\'};
		UINT id;
		disk[0] = driverName;

		cout << '\n';
		id = GetDriveTypeA(disk);
		switch (id) {
		case 0:
			cout << "Тип диска " << disk << " неизвестен!\n";
			break;
		case 1:
			cout << "Не удалось найти диск " << disk <<" !\n";
			break;
		case DRIVE_REMOVABLE:
			cout << "Диск " << disk <<" гибкий.\n";
			break;
		case DRIVE_FIXED:
			cout << "Диск " << disk << " жесткий.\n";
			break;
		case DRIVE_REMOTE:
			cout << "Диск " << disk << " сетевой.\n";
			break;
		case DRIVE_CDROM:
			cout << disk << " компакт диск.\n";
			break;
		case DRIVE_RAMDISK:
			cout << disk << " RAM диск.\n";
			break;
		}

		//получение подробной инфо о томе
		
		//если disk NULL
		//используется корень текущего каталога

		//указатель на буфер, получивший название тома
		char VNameBuffer[MAX_PATH];
		
		char SysNameBuffer[MAX_PATH];
		//серийный номер диска
		DWORD VSNumber;
		DWORD MCLength;
		//указатель на переменную, которая получает флаги, связанные с указанной файловой системой
		DWORD FileSF;

		if (GetVolumeInformationA(disk, VNameBuffer, sizeof(VNameBuffer),
			&VSNumber, &MCLength, &FileSF, SysNameBuffer, sizeof(SysNameBuffer)))
		{
			cout << "Имя тома:  " << VNameBuffer << "\n";
			cout << "Название файловой структуры: " << SysNameBuffer << "\n";
			cout << "Серийный номер диска: " << VSNumber << "\n";
			cout << "Максимальная длина имени файла: " << MCLength << "\n";
			if (FileSF & FS_CASE_IS_PRESERVED)
				cout << "При записи на диск сохраняется регистр букв в имени файла\n";
			if (FileSF & FS_CASE_SENSITIVE)
				cout << "ФC поддерживает поиск файлов с учетом регистра букв в именах\n";
			if (FileSF & FS_UNICODE_STORED_ON_DISK)
				cout << "ФC поддерживает хранение на диске имен файлов в UNICODE\n";
			if (FileSF & FILE_PERSISTENT_ACLS)
				cout << "ФC способна оперировать с ACL\n";
			if (FileSF & FILE_FILE_COMPRESSION)
				cout << "ФС поддерживает сжатие файлов\n";
			if (FileSF & FILE_NAMED_STREAMS)
				cout << "ФС поддерживает именованные потоки\n";
			if (FileSF & FILE_SUPPORTS_ENCRYPTION)
				cout << "ФС поддерживает Encrypted File System (EPS)\n";
			if (FileSF & FILE_SUPPORTS_OBJECT_IDS)
				cout << "ФС поддерживает идентификаторы объектов\n";
			if (FileSF & FILE_SUPPORTS_REPARSE_POINTS)
				cout << "ФС поддерживает точки повторного разбора\n";
			if (FileSF & FILE_SUPPORTS_SPARSE_FILES)
				cout << "ФС поддерживает разреженные файлы\n";
			if (FileSF & FILE_VOLUME_QUOTAS)
				cout << "ФС поддерживает VOLUME QUOTAS\n";
		}
		else
			cout << "Не удалось получить информацию о диске\n";
		DWORD SectorsPerCluster;
		DWORD BytesPerSector;
		DWORD NumberOfFreeClusters;
		DWORD TotalNumberOfClusters;

		if (GetDiskFreeSpaceA(disk, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters))
		{
			cout << "Секторов в кластере: " << SectorsPerCluster << "\n";
			cout << "Байт в секторе: " << BytesPerSector << "\n";
			cout << "Свободных кластеров на диске: " << NumberOfFreeClusters << "\n";
			cout << "Кластеров на диске: " << TotalNumberOfClusters << "\n";
			//cout << "Общий объём места на диске: " << double(SectorsPerCluster) * double(BytesPerSector) * double(TotalNumberOfClusters) / 1024 / 1024 / 1024 << " Гб" << endl;
			//cout << "Количество свободного места на диске: \n";
			//cout << double(SectorsPerCluster) * double(BytesPerSector) * double(NumberOfFreeClusters) / 1024 / 1024 / 1024 << " Гб" << endl;
		}
		else
			cout << "Не удалось получить информацию о свободном пространстве на диске\n";
	}
	else {
		cout << "Диск не существует!\n";
	}
}

boolean iHopeDiskExists(char driverName) {
	int driverCount = 0;

	DWORD logic = GetLogicalDrives();
	driverCount = bin(logic, driverCount);
	DWORD cchBuffer = MAX_PATH;
	char lpBuffer[520] = { 0 };
	DWORD trueBuf = GetLogicalDriveStrings(cchBuffer, (LPWSTR)lpBuffer);

	//проверка введенного диска на существование в системе
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
	cout << "Укажите путь к копируемому файлу: ";
	pathToFile = validPath();
	cout << "Укажите путь и новое имя для скопированного файла: ";
	cin >> pathToNewFile;
	if (CopyFileA(pathToFile.c_str(), pathToNewFile.c_str(), TRUE)) {
		cout << "Файл успешно скопирован в " << pathToNewFile << "\n";
	}
	else cout << "Копирование не удалось.\nВозможно, скопированный файл по указанному пути уже существует..\n";
};

void moveFile() {
	string pathToFile, newLocation;

	system("cls");
	cout << "Укажите путь к файлу, который желаете переместить: ";
	pathToFile = validPath();
	cout << "Укажите новый путь к файлу, с указанием файла: ";
	cin >> newLocation;
	//MOVEFILE_COPY_ALLOWED позволяет перенести файл в другой том
	//флаг MOVEFILE_WRITE_THROUGH призывает функцию не возвращать значение пока файл не будет
	//перемещен на диск
	if (MoveFileExA(pathToFile.c_str(), newLocation.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
		cout << "Файл успешно перемещен.\n";
	else cout << "Переместить файл не удалось. Возможно, по указнному пути уже существует файл.";
};

void crFile() {
	string pathCr;
	system("cls");
	cout << "Укажите путь вместе с именем файла для его создания: ";
	cin >> pathCr;
	//1 параметр - указатель на имя файла, 2 - запрос доступа на чтение и запись
	//3 - разрешение на совместное использование файла несколькими процессами
	//6 - файл всегда создается или пересоздается 7 - атрибуты создания файла (readonly)
	//в случае успеха вернется дескриптор файла 
	//ошибка - вернется значение INVALID_HANDLE_VALUE
	CreateFileA(pathCr.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	cout << "Файл создан!" << "\n";
}

void crDelDirectory() {
	char pickme;

	do {
		system("cls");
		cout << "1 - Создание каталога\n2 - Удаление каталога\n0 - Возвращение в меню\n";
		cout << "Введите число: ";
		cin >> pickme;
		switch (pickme)
		{
		case '1': {
			string dirName;
			string pathCr;
			system("cls");
			cout << "Введите путь для создания каталога: ";
			pathCr = validPath();
			cout << "Введите название каталога: ";
			cin >> dirName;
			pathCr += dirName;
			if (CreateDirectoryA(pathCr.c_str(), NULL)) {
				cout << "Каталог " << pathCr << " создан!\n";
			}
			else {
				cout << "Каталог " << pathCr << " уже существует!\n";
			}
			system("pause");
			break;
		}
		case '2': {
			string pathDel;
			system("cls");
			cout << "Введите путь к каталогу: ";
			pathDel = validPath();
			pathDel += "\\\\";
			//при успешном выполнении возвр значение != 0
			//удаление возможно только для пустых директорий
			if (RemoveDirectoryA(pathDel.c_str())) cout << "\nКаталог удалён!\n";
			else cout << "\nКаталог не удалось удалить...\n";
			system("pause");
			break;
		}
		}
	} while (pickme != '0');
}

void fileAttribute() {
	string path;
	DWORD attributeAnalysis;
	cout << "Укажите путь к файлу: ";
	cin >> path;
	attributeAnalysis = GetFileAttributesA(path.c_str());
	if (attributeAnalysis != INVALID_FILE_ATTRIBUTES) {
		if (attributeAnalysis & FILE_ATTRIBUTE_NORMAL)
			cout << "Файл не имеет других установленных атрибутов\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_COMPRESSED)
			cout << "Файл сжатый\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_DIRECTORY)
			cout << "Файл идентифицирован как каталог\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_ENCRYPTED)
			cout << "Файл зашифрован\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_HIDDEN)
			cout << "Файл скрыт\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_READONLY)
			cout << "Файл только для чтения\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_SPARSE_FILE)
			cout << "Файл - разреженный\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_SYSTEM)
			cout << "Файл частично или полностью используется операционной системой.\n";
		if (attributeAnalysis & FILE_ATTRIBUTE_TEMPORARY)
			cout << "Файл используется для временного хранения\n";
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
	cout << "Укажите путь к файлу атрибуты которого хотите изменить: ";
	path = validPath();
	cout << "\nУстановить атрибут 'зашифрован'\n+ - Да\nЛюбой другой символ - нет\n";
	cin >> flag;
	if (flag == '+')
	{
		fA = GetFileAttributesA(path.c_str());
		if (fA & FILE_ATTRIBUTE_COMPRESSED)
			cout << "\nФайл сжат, зашифровать его невозможно..\n";
		else
		{
			//шифрование файла или каталога
			if (EncryptFileA(path.c_str()))
				cout << "\nШифрование файла завершено.\n";
			else
				cout << "\nНе удалось зашифровать файл!\n";
		}
	}
	else
	{
		fA = GetFileAttributesA(path.c_str());
		if (fA & FILE_ATTRIBUTE_ENCRYPTED)
			//расшифровка файла или каталога
			DecryptFileA(path.c_str(), fA);
	}
	cout << "\nУстановить атрибут 'скрытый'?\n+ - Да\n";
	cin >> flag;
	if (flag == '+')
		fAttr |= FILE_ATTRIBUTE_HIDDEN;
	cout << "\nУстановить атрибут 'только для чтения'?\n+ - Да\n";
	cin >> flag;
	if (flag == '1') {
		SetFileAttributesA(path.c_str(), fAttr);
		fAttr |= FILE_ATTRIBUTE_READONLY;
	}
	cout << "\nУстановить атрибут 'системный файл'?\n+ - Да\n";
	cin >> flag;
	if (flag == '1')
		fAttr |= FILE_ATTRIBUTE_SYSTEM;
	cout << "\nУстановить атрибут 'для временного хранения'?\n+ - Да\n";
	cin >> flag;
	if (flag == '1')
		fAttr |= FILE_ATTRIBUTE_TEMPORARY;
	//установка выбранных атрибутов
	//при успешном завершении функция
	//возвращает ненулевое значение
	if (SetFileAttributesA(path.c_str(), fAttr))
		cout << "\nАтрибуты для файла успешно установлены.\n";
};

void fileInfo() {
	HANDLE fHandle;
	BY_HANDLE_FILE_INFORMATION FileInfo;
	SYSTEMTIME sTime, lTime;
	string path;
	cout << "Укажите путь к файлу: ";
	path = validPath();
	fHandle = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileInformationByHandle(fHandle, &FileInfo);
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
		cout << "Файл не имеет других установленных атрибутов\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
		cout << "Файл сжатый\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		cout << "Файл идентифицирован как каталог\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
		cout << "Файл зашифрован\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
		cout << "Файл скрыт\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		cout << "Файл только для чтения\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
		cout << "Файл - разреженный\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
		cout << "Файл частично или исключительно используется операционной системой (системный файл)\n";
	if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
		cout << "Файл используется для временного хранения\n";
	FileTimeToSystemTime(&FileInfo.ftCreationTime, &sTime);
	SystemTimeToTzSpecificLocalTime(NULL, &sTime, &lTime);
	cout << "\nВремя создания файла: \n" << sTime.wDay << ".";
	cout << lTime.wMonth << "." << lTime.wYear << " " << lTime.wHour << ":" << lTime.wMinute << "\n";
	FileTimeToSystemTime(&(FileInfo.ftLastWriteTime), &sTime);
	SystemTimeToTzSpecificLocalTimeEx(NULL, &sTime, &lTime);
	cout << "Время последней записи в файл: \n" << lTime.wDay << ".";
	cout << lTime.wMonth << "." << lTime.wYear << " " << lTime.wHour << ":" << lTime.wMinute << "\n";
	cout << "Порядковый номер тома, содержащего файл: " << FileInfo.dwVolumeSerialNumber << "\n";
	cout << "Старшая часть размера файла: " << FileInfo.nFileSizeHigh << "\n";
	cout << "Младшая часть размера файла: " << FileInfo.nFileSizeLow << "\n";
	cout << "Число связей с этим файлом: " << FileInfo.nNumberOfLinks << "\n";
	cout << "Старшая часть уникального идентификатора связи с файлом: " << FileInfo.nFileIndexHigh << "\n";
	cout << "Младшая часть уникального идентификатора связи с файлом: " << FileInfo.nFileIndexLow << "\n";
	CloseHandle(fHandle);
};

void modifyDate() {
	HANDLE fHandle;
	TIME_ZONE_INFORMATION Tz;
	int Error;
	FILETIME fTimeCr, fTimeAc, fTimeWr;
	SYSTEMTIME sTimeCr = {}, sTimeAc = {}, sTimeWr = {}, UTimeCr = {}, UTimeAc = {}, UTimeWr = {};
	string path;
	cout << "Введите путь к файлу дату и время которого хотите изменить: ";
	path = validPath();
	fHandle = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WORD Year, Month, Day, Hour, Minute;
	cout << "Введите время создания файла.\n";
	cout << "\nВведите год: ";
	cin >> Year;
	sTimeCr.wYear = Year;
	cout << "\nВведите месяц: ";
	cin >> Month;
	sTimeCr.wMonth = Month;
	cout << "\nВведите день: ";
	cin >> Day;
	sTimeCr.wDay = Day;
	cout << "\nВведите час: ";
	cin >> Hour;
	sTimeCr.wHour = Hour;
	cout << "\nВведите минуты: ";
	cin >> Minute;
	sTimeCr.wMinute = Minute;
	GetTimeZoneInformation(&Tz);
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeCr, &UTimeCr);
	SystemTimeToFileTime(&UTimeCr, &fTimeCr);
	cout << "\nВведите время чтения или записи в файл. ";
	cout << "\nВведите год: ";
	cin >> Year;
	sTimeAc.wYear = Year;
	cout << "\nВведите месяц: ";
	cin >> Month;
	sTimeAc.wMonth = Month;
	cout << "\nВведите день: ";
	cin >> Day;
	sTimeAc.wDay = Day;
	cout << "\nВведите час: ";
	cin >> Hour;
	sTimeAc.wHour = Hour;
	cout << "\nВведите минуты: ";
	cin >> Minute;
	sTimeAc.wMinute = Minute;
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeAc, &UTimeAc);
	SystemTimeToFileTime(&UTimeAc, &fTimeAc);
	cout << "Введите время записи в файл.";
	cout << "\nВведите год: ";
	cin >> Year;
	sTimeWr.wYear = Year;
	cout << "\nВведите месяц: ";
	cin >> Month;
	sTimeWr.wMonth = Month;
	cout << "\nВведите день: ";
	cin >> Day;
	sTimeWr.wDay = Day;
	cout << "\nВведите час \: ";
	cin >> Hour;
	sTimeWr.wHour = Hour;
	cout << "\nВведите минуты: ";
	cin >> Minute;
	sTimeWr.wMinute = Minute;
	TzSpecificLocalTimeToSystemTime(&Tz, &sTimeWr, &UTimeWr);
	SystemTimeToFileTime(&UTimeWr, &fTimeWr);
	if (SetFileTime(fHandle, &fTimeCr, &fTimeAc, &fTimeWr))
		cout << "\nДата и время успешно изменены!";
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
		//при успешном завершении возвращаемое значение - атрибуты файла или каталога
		if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES)
			error = GetLastError();
		if (error == 3 || error == 2) {
			cout << "Не удалось найти файл или каталог по указанному пути.\n";
			cout << "Введите путь ещё раз: ";
		}
	} while (error != 0);
	return path;
}
