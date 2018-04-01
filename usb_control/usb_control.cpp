// usb_control.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "usb_control.h"
#include "usb_operation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;
CLogFile LogFile(_T("debug.log"));

using namespace std;

int main(int argc, char **argv)
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
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
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }

    return nRetCode;
}
