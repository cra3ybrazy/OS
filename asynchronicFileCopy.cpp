#include <iostream>
#include <Windows.h>

using namespace std;

//дескриптор файла 
HANDLE ogHandle;
//дескриптор копии
HANDLE copyHandle;

DWORD cash = 0;
//счётчик APC функции
int cnt = 0;
//ассинхр чтение
void AsyncRead(DWORD64, DWORD, int, char**, OVERLAPPED*);
//ассинхр запись
void AsyncWrite(DWORD64, DWORD, int, char**, OVERLAPPED*);

//копирование файла
void fileCopy(int thread, int block);

//функция звершеения используемая
//как параметр в ассинхр в-в
//передается как адрес и вызывается всякий раз при завершении в-в
VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfWritten, LPOVERLAPPED lpOverlapped)
{
	cnt++;
}

int main() {
	setlocale(LC_ALL, "Russian");
	//размер кластера по умолчанию (4 кб)
	int clustersize = 4096;
	//блок данных кратный размеру кластера
	int block;
	//количество потоков
	int thread;
	//количество перекрывающих операций в-в
	int n;
	string pathToCopy;
	string pathToPaste;
	DWORD t_begin;
	DWORD t_end;

	cout << "Здравствуйте!\nДанное приложение умеет выполнять ";
	cout << "копирование файла методом ассинхронного копирования.\n\n";

	cout << "Введите количество потоков: ";
	cin >> thread;
	cout << "\nВведите коэффициент n: ";
	cin >> n;

	cout << "\nПуть к копируемому файлу: ";
	cin >> pathToCopy;
	cout << "\nПуть к копии файла: ";
	cin >> pathToPaste;

	//FILE_FLAG_NO_BUFFERING - файл откроется без системного кэширования
	//для чтения или записи данных
	//FILE_FLAG_OVERLAPPED указывает что
	//файл или устройство открывается для асинхронного ввода-вывода.
	ogHandle = CreateFileA(pathToCopy.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
	copyHandle = CreateFileA(pathToPaste.c_str(), GENERIC_WRITE, 
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

	block = clustersize * n;
	t_begin = GetTickCount();
	fileCopy(thread, block);

	CloseHandle(ogHandle);
	CloseHandle(copyHandle);
	t_end = GetTickCount();
	cout << "Затраченное время на копирование: ";
	cout << t_end - t_begin << " мс\n";
}

void fileCopy(int thread, int block) {
	DWORD low;
	//старшая часть 64-раз размера файла
	DWORD high;

	//блоки которыми будет проводиться чтение и запись в файл
	OVERLAPPED* readOvrlp = new OVERLAPPED[thread];
	OVERLAPPED* writeOvrlp = new OVERLAPPED[thread];

	char** buf = new char* [thread];

	DWORD64 fileSize, fileSizeDisk = 0;
	DWORD64 tmpSize = 0;
	//вз младшие 32 бита размера файла
	low = GetFileSize(ogHandle, &high);
	//истинный размер файла
	fileSize = ((DWORD64)high << 32) + low;
	while (fileSizeDisk < fileSize) fileSizeDisk += block;

	for (int i = 0; i < thread; i++)
	{
		buf[i] = new char[block];
		readOvrlp[i].OffsetHigh = ((DWORD64)(i * block) >> 32);
		writeOvrlp[i].OffsetHigh = ((DWORD64)(i * block) >> 32);
		readOvrlp[i].Offset = ((DWORD64)(i * block)) ^ ((DWORD64)readOvrlp[i].OffsetHigh << 32);
		writeOvrlp[i].Offset = ((DWORD64)(i * block)) ^ ((DWORD64)writeOvrlp[i].OffsetHigh << 32);
		readOvrlp[i].hEvent = NULL;
		writeOvrlp[i].hEvent = NULL;
	}

	do
	{
		AsyncRead(fileSize, block, thread, buf, readOvrlp);
		AsyncWrite(fileSize, block, thread, buf, writeOvrlp);
		tmpSize = tmpSize + (DWORD64)(block * thread);
	} while (tmpSize < fileSizeDisk);
	long lhigh = (long)high;
	//позиция указателя синхронного в-в в конец файла
	SetFilePointer(copyHandle, fileSize, &lhigh, FILE_BEGIN);
	//установка конца файла
	SetEndOfFile(copyHandle);
};

void AsyncRead(DWORD64 fileSize, DWORD block, int thread, char** buf, OVERLAPPED* readOvrlp) {
	DWORD64 written = 0, high = 0;
	int threadCounter = 0;
	DWORD oldCash = cash;
	for (int i = 0; i < thread && fileSize > cash; i++, threadCounter++)
	{
		//тревожный АВ
		ReadFileEx(ogHandle, buf[i], block, &readOvrlp[i], FileIOCompletionRoutine);
		cash += block;
	}

	while (cnt < threadCounter)
	{
		//поток запустивший операцию чтения
		//должен приостановить себя
		//чтобы функция завершения норм работала
		SleepEx(-1, true);
	};
	for (int i = 0; i < thread; i++)
	{
		written = ((DWORD64)readOvrlp[i].Offset) + (DWORD64)(block * thread) + ((DWORD64)readOvrlp[i].OffsetHigh << 32);
		high = written;
		readOvrlp[i].OffsetHigh = (high >> 32);
		readOvrlp[i].Offset = written ^ ((DWORD64)readOvrlp[i].OffsetHigh << 32);
	}
	cnt = 0;
	cash = oldCash;
};

void AsyncWrite(DWORD64 fileSize, DWORD block, int thread, char** buf, OVERLAPPED* writeOvrlp) {
	DWORD64 writtenBytes = 0, highWriten = 0;
	int threadCnt = 0;
	DWORD oldCash = cash;
	for (int i = 0; i < thread && fileSize > cash; i++)
	{
		//тревожный АВ
		threadCnt++;
		WriteFileEx(copyHandle, buf[i], block, &writeOvrlp[i], FileIOCompletionRoutine);
		cash = cash + block;
	}
	while (cnt < threadCnt)
	{
		//поток запустивший операцию записи
		//должен приостановить себя
		//чтобы функция завершения норм работала
		SleepEx(-1, true);
	}

	for (int i = 0; i < thread; i++)
	{
		writtenBytes = ((DWORD64)writeOvrlp[i].Offset) + (DWORD64)(block * thread) + 
			((DWORD64)writeOvrlp[i].OffsetHigh << 32);
		highWriten = writtenBytes;
		writeOvrlp[i].OffsetHigh = (highWriten >> 32);
		writeOvrlp[i].Offset = writtenBytes ^ ((DWORD64)writeOvrlp[i].OffsetHigh << 32);
	}
	cnt = 0;
};
