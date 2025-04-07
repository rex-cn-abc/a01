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
    static CHOOSECOLOR cc = { 0 };
    static COLORREF crCustColors[16] = { 0 };
    TCHAR szBuf[128] = { 0 };

    switch (uMsg)
    {
    case WM_INITDIALOG:
        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hwndDlg;
        cc.rgbResult = RGB(255, 0, 0);
        cc.lpCustColors = crCustColors;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_EDIT_COLOR:
            if (ChooseColor(&cc))
            {
                wsprintf(szBuf, TEXT("颜色值：%06X"), cc.rgbResult);
                MessageBox(hwndDlg, szBuf, TEXT("提示"), MB_OK);
            }
            break;

        case IDCANCEL:
            EndDialog(hwndDlg, 0);
            break;
        }
        return TRUE;
    }

    return FALSE;
}