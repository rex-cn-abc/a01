#include <windows.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 全局变量
HINSTANCE g_hInstance;

// 函数声明
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    // 创建模态对话框
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        // 设置标题栏左侧和任务栏中的程序小图标
        SendMessage(hwndDlg, WM_SETICON,
            ICON_SMALL, (LPARAM)LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON_PANDA)));
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_BROWSE:    // 处理按下浏览按钮
            break;

        case IDC_BTN_OPEN:      // 处理按下打开按钮
            break;
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        return TRUE;
    }

    return FALSE;
}