#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LogFile.h"

using namespace std;

CLogFile::CLogFile(LPCTSTR name)
{
	BOOL status;

	lstrcpy(szLogFileName, name);
	
	status = logFile.Open(szLogFileName, CFile::modeWrite | CFile::modeRead | CFile::shareExclusive | CFile::typeBinary);
	if(status == 0){
		status = logFile.Open(szLogFileName, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate | CFile::typeBinary);
	}	
	if(status == 0){
		/* open file fail */
		szLogFileName[0] = 0;
	}
	CheckUnicodeFile();
}

CLogFile::CLogFile(void)
{
	CLogFile(NULL);
}

CLogFile::~CLogFile(void)
{
	if(szLogFileName[0] != NULL){
		bool bNeedDelete = false;
		if(logFile.GetPosition() == 0){
			bNeedDelete = true;
		}
		logFile.Close();
		if(bNeedDelete){
			logFile.Remove(szLogFileName);
		}
	}
}

boolean CLogFile::NewFile(LPCTSTR name)
{
	BOOL status;

	if(szLogFileName[0] != NULL){
		szLogFileName[0] = 0;
		logFile.Close();
	}

	if(name != NULL){
		lstrcpy(szLogFileName, name);

		status = logFile.Open(szLogFileName, CFile::modeWrite | CFile::modeRead | CFile::shareExclusive | CFile::typeBinary);
		if(status == 0){
			status = logFile.Open(szLogFileName, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate | CFile::typeBinary);
		}	
		if(status == 0){
			/* open file fail */
			szLogFileName[0] = 0;
		}
	}
	CheckUnicodeFile();
	return true;
}

boolean CLogFile::Write(LPCTSTR str)
{
	if(szLogFileName[0] == NULL){
		return false;
	}
	logFile.Write(str, lstrlen(str)*sizeof(TCHAR));
	logFile.Write(_T("\r\n"), 2 * sizeof(TCHAR));
	logFile.Flush();

	return true;
}

boolean CLogFile::WriteBin(unsigned char *bin, unsigned int length)
{
	if(szLogFileName[0] == NULL){
		return false;
	}
	logFile.Write(bin, length);

	return true;
}

boolean CLogFile::ReadBin(unsigned char *bin, unsigned int length)
{
	int iReadLength;
	if(szLogFileName[0] == NULL){
		return false;
	}
	iReadLength = logFile.Read(bin, length);
	if(iReadLength != length){
		return false;
	}

	return true;
}

ULONGLONG CLogFile::GetFlieSize(void)
{
	return logFile.GetLength();
}

/************************************************************************/
/* Debug 信息输出添加规则												*/
/* 1. cmd不添加打印输出，错误已经在SG7710_IO里实现。					*/
/* 2. 其它的数据比较及信息需要在上层软件中增加打印输出。				*/
/************************************************************************/
boolean CLogFile::e(LPCTSTR TAG, LPCTSTR str, ...)
{
	TCHAR buf[10240];

	va_list ap;
	va_start(ap, str);
	vswprintf_s(buf, sizeof(buf)/sizeof(TCHAR), str, ap);
	va_end(ap);

	DumpMsg(TAG, buf, 0);
	return true;
}

boolean CLogFile::d(LPCTSTR TAG, LPCTSTR str, ...)
{
	TCHAR buf[10240];

	va_list ap;
	va_start(ap, str);
	vswprintf_s(buf, sizeof(buf) / sizeof(TCHAR), str, ap);
	va_end(ap);

	DumpMsg(TAG, buf, 1);
	return true;
}

boolean CLogFile::i(LPCTSTR TAG, LPCTSTR str, ...)
{
	TCHAR buf[10240];

	va_list ap;
	va_start(ap, str);
	vswprintf_s(buf, sizeof(buf) / sizeof(TCHAR), str, ap);
	va_end(ap);

	DumpMsg(TAG, buf, 2);
	return true;
}

boolean CLogFile::DumpMsg(LPCTSTR TAG, LPCTSTR str, int level)
{
	CTime tm = CTime::GetCurrentTime();
	CString tmp;
	TCHAR *Type;
	LPCTSTR pBuf;

	if (level == 0) {
		/* Error Msg */
		Type = _T("[E]");
	}
	else if (level == 1) {
		/* Debug Msg */
		Type = _T("[D]");
	}
	else {
		/* Information Msg */
		Type = _T("[I]");
	}

	tmp.Format(_T("%d:%d:%d %s:[%s] %s\r\n"), \
		tm.GetHour(), tm.GetMinute(), tm.GetSecond(), Type, TAG, str);

	if(szLogFileName[0] == NULL){
		OutputDebugString(tmp);
		return false;
	}	

	if (level <= 1) {
		LPCTSTR buf = tmp.GetBuffer();
		int len = wcslen(tmp.GetBuffer());
		logFile.Write(tmp.GetBuffer(), wcslen(tmp.GetBuffer())*sizeof(TCHAR));				
	}
	OutputDebugString(tmp);

	if(level == 0){
		logFile.Flush();
	}	

	return true;
}

boolean CLogFile::SaveMemory(LPCTSTR TAG, unsigned char *memory, unsigned int length, unsigned int num_perline)
{
	CString strLog;
	if(szLogFileName[0] == NULL){
		return false;
	}
	logFile.Write(TAG, lstrlen(TAG)*sizeof(TCHAR));
	logFile.Write(_T("\r\n"), 2 * sizeof(TCHAR));

	for(unsigned int i=0; i<length; i+=num_perline){
		strLog.Format(_T("0x%-8x: "), i);
		for(int j=0; j<num_perline; j++){
			if(i+j>=length){
				break;
			}
			strLog.AppendFormat(_T("0x%-5x"), (unsigned char)memory[i+j]);
		}
		strLog.Append(_T("\r\n"));
		logFile.Write(strLog.GetBuffer(), strLog.GetLength());
	}
	logFile.Flush();
	return true;
}

boolean CLogFile::CheckUnicodeFile(void)
{
	boolean status = true;
	unsigned char UnicodeTag[2] = {0xff, 0xfe};
	if (szLogFileName == NULL) {
		return false;
	}
	if (GetFlieSize() == 0) {
		logFile.Write(UnicodeTag, sizeof(UnicodeTag));
	}

	return status;
}