#pragma once

class CLogFile
{
public:
	TCHAR szLogFileName[256];
	CFile logFile;
public:
	CLogFile(void);
	CLogFile(LPCTSTR name);
public:
	~CLogFile(void);
public:
	boolean NewFile(LPCTSTR name);
	boolean Write(LPCTSTR str);
	boolean WriteBin(unsigned char *bin, unsigned int length);
	boolean ReadBin(unsigned char *bin, unsigned int length);
	boolean e(LPCTSTR TAG, LPCTSTR str, ...);
	boolean d(LPCTSTR TAG, LPCTSTR str, ...);
	boolean i(LPCTSTR TAG, LPCTSTR str, ...);
	boolean DumpMsg(LPCTSTR TAG, LPCTSTR str, int level);
	boolean SaveMemory(LPCTSTR TAG, unsigned char *memory, unsigned int length, unsigned int num_perline = 8);
	ULONGLONG GetFlieSize(void);
	boolean CheckUnicodeFile(void);
};

extern CLogFile LogFile;

