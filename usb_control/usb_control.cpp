// usb_control.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "usb_control.h"
#include "usb_operation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;
CLogFile LogFile(_T("debug.log"));

using namespace std;

int main(int argc, char **argv)
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: ���Ĵ�������Է���������Ҫ
            wprintf(L"����: MFC ��ʼ��ʧ��\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
			const struct libusb_version* version;
			char c = NULL;

			lib_usb_main(argc, argv);

			LogFile.i(_T(""), _T("Press enter/space key to exist"));
			while ((c != 0xd) && (c != 0x20)) {
				c = _getch();
			}
        }
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        wprintf(L"����: GetModuleHandle ʧ��\n");
        nRetCode = 1;
    }

    return nRetCode;
}
