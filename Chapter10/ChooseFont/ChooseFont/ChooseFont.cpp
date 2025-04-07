#include <windows.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ȫ�ֱ���
HINSTANCE g_hInstance;

// ��������
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    // ����ģ̬�Ի���
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
                wsprintf(szBuf, TEXT("�������ƣ�%s\n  �����С��%d��\n  ��ɾ���ߣ�%s\n  ���»��ߣ�%s\n  ������ɫ��%06X\n  "),
                    cf.lpLogFont->lfFaceName,
                    cf.iPointSize / 10,
                    cf.lpLogFont->lfStrikeOut ? TEXT("��") : TEXT("��"),
                    cf.lpLogFont->lfUnderline ? TEXT("��") : TEXT("��"),
                    cf.rgbColors);
                MessageBox(hwndDlg, szBuf, TEXT("��ʾ"), MB_OK);
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
        DrawText(hdc, TEXT("Windows������"), _countof(TEXT("Windows������")),
            &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwndDlg, &ps);
        DeleteObject(SelectObject(hdc, hFontOld));
        break;
    }

    return FALSE;
}