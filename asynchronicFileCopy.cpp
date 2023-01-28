#include <iostream>
#include <Windows.h>

using namespace std;

//���������� ����� 
HANDLE ogHandle;
//���������� �����
HANDLE copyHandle;

DWORD cash = 0;
//������� APC �������
int cnt = 0;
//������� ������
void AsyncRead(DWORD64, DWORD, int, char**, OVERLAPPED*);
//������� ������
void AsyncWrite(DWORD64, DWORD, int, char**, OVERLAPPED*);

//����������� �����
void fileCopy(int thread, int block);

//������� ���������� ������������
//��� �������� � ������� �-�
//���������� ��� ����� � ���������� ������ ��� ��� ���������� �-�
VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfWritten, LPOVERLAPPED lpOverlapped)
{
	cnt++;
}

int main() {
	setlocale(LC_ALL, "Russian");
	//������ �������� �� ��������� (4 ��)
	int clustersize = 4096;
	//���� ������ ������� ������� ��������
	int block;
	//���������� �������
	int thread;
	//���������� ������������� �������� �-�
	int n;
	string pathToCopy;
	string pathToPaste;
	DWORD t_begin;
	DWORD t_end;

	cout << "������������!\n������ ���������� ����� ��������� ";
	cout << "����������� ����� ������� ������������� �����������.\n\n";

	cout << "������� ���������� �������: ";
	cin >> thread;
	cout << "\n������� ����������� n: ";
	cin >> n;

	cout << "\n���� � ����������� �����: ";
	cin >> pathToCopy;
	cout << "\n���� � ����� �����: ";
	cin >> pathToPaste;

	//FILE_FLAG_NO_BUFFERING - ���� ��������� ��� ���������� �����������
	//��� ������ ��� ������ ������
	//FILE_FLAG_OVERLAPPED ��������� ���
	//���� ��� ���������� ����������� ��� ������������ �����-������.
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
	cout << "����������� ����� �� �����������: ";
	cout << t_end - t_begin << " ��\n";
}

void fileCopy(int thread, int block) {
	DWORD low;
	//������� ����� 64-��� ������� �����
	DWORD high;

	//����� �������� ����� ����������� ������ � ������ � ����
	OVERLAPPED* readOvrlp = new OVERLAPPED[thread];
	OVERLAPPED* writeOvrlp = new OVERLAPPED[thread];

	char** buf = new char* [thread];

	DWORD64 fileSize, fileSizeDisk = 0;
	DWORD64 tmpSize = 0;
	//�� ������� 32 ���� ������� �����
	low = GetFileSize(ogHandle, &high);
	//�������� ������ �����
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
	//������� ��������� ����������� �-� � ����� �����
	SetFilePointer(copyHandle, fileSize, &lhigh, FILE_BEGIN);
	//��������� ����� �����
	SetEndOfFile(copyHandle);
};

void AsyncRead(DWORD64 fileSize, DWORD block, int thread, char** buf, OVERLAPPED* readOvrlp) {
	DWORD64 written = 0, high = 0;
	int threadCounter = 0;
	DWORD oldCash = cash;
	for (int i = 0; i < thread && fileSize > cash; i++, threadCounter++)
	{
		//��������� ��
		ReadFileEx(ogHandle, buf[i], block, &readOvrlp[i], FileIOCompletionRoutine);
		cash += block;
	}

	while (cnt < threadCounter)
	{
		//����� ����������� �������� ������
		//������ ������������� ����
		//����� ������� ���������� ���� ��������
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
		//��������� ��
		threadCnt++;
		WriteFileEx(copyHandle, buf[i], block, &writeOvrlp[i], FileIOCompletionRoutine);
		cash = cash + block;
	}
	while (cnt < threadCnt)
	{
		//����� ����������� �������� ������
		//������ ������������� ����
		//����� ������� ���������� ���� ��������
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
