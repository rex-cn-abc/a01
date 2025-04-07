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
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    HFONT hFontOld;

    static LOGFONT lf = { 0 };
    static CHOOSEFONT cf = { 0 };
    TCHAR szBuf[256] = { 0 };

    switch (uMsg)
    {
    case WM_INITDIALOG:
        GetObject(GetStockObject(OEM_FIXED_FONT), sizeof(lf), &lf);
        cf.lStructSize = sizeof(CHOOSEFONT);
        cf.hwndOwner = hwndDlg;
        cf.lpLogFont = &lf;
        cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_EDIT_FONT:
            if (ChooseFont(&cf))
            {
                wsprintf(szBuf, TEXT("字体名称：%s\n  字体大小：%d磅\n  有删除线：%s\n  有下划线：%s\n  字体颜色：%06X\n  "),
                    cf.lpLogFont->lfFaceName,
                    cf.iPointSize / 10,
                    cf.lpLogFont->lfStrikeOut ? TEXT("是") : TEXT("否"),
                    cf.lpLogFont->lfUnderline ? TEXT("是") : TEXT("否"),
                    cf.rgbColors);
                MessageBox(hwndDlg, szBuf, TEXT("提示"), MB_OK);
                InvalidateRect(hwndDlg, NULL, TRUE);
            }
            break;

        case IDCANCEL:
            EndDialog(hwndDlg, 0);
            break;
        }
        return TRUE;

    case WM_PAINT:
        hdc = BeginPaint(hwndDlg, &ps);
        GetClientRect(hwndDlg, &rect);
        hFontOld = (HFONT)SelectObject(hdc, CreateFontIndirect(&lf));
        SetTextColor(hdc, cf.rgbColors);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, TEXT("Windows中文网"), _countof(TEXT("Windows中文网")),
            &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwndDlg, &ps);
        DeleteObject(SelectObject(hdc, hFontOld));
        break;
    }

    return FALSE;
}