#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("DirList");
    HWND hwnd;
    MSG msg;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance;
    static HFONT hFont;
    static HWND hwndStaticPath, hwndListBox, hwndEdit;
    TCHAR szPath[] = TEXT("*.*");
    TCHAR szStr[MAX_PATH] = { 0 };  // 存放选中列表项的文本

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        hwndStaticPath = CreateWindowEx(0, TEXT("Static"), TEXT(""),
            WS_CHILD | WS_VISIBLE, 10, 0, 380, 20, hwnd, (HMENU)IDC_STATICPATH, hInstance, NULL);
        hwndListBox = CreateWindowEx(0, TEXT("ListBox"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            10, 25, 200, 240, hwnd, (HMENU)IDC_LISTBOX, hInstance, NULL);
        hwndEdit = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | 
            ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
            0, 0, 0, 0, hwnd, (HMENU)IDC_EDIT, hInstance, NULL);

        // 列出目录列表
        DlgDirList(hwnd, szPath, IDC_LISTBOX, IDC_STATICPATH, 
            DDL_ARCHIVE | DDL_READONLY | DDL_SYSTEM | DDL_DIRECTORY | DDL_DRIVES);

        // 设置控件字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndStaticPath, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndListBox, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_SIZE:
        // 调整列表框和多行编辑控件的大小
        SetWindowPos(hwndListBox, NULL, 0, 0, 200, HIWORD(lParam) - 25, SWP_NOZORDER | SWP_NOMOVE);
        MoveWindow(hwndEdit, 220, 25, LOWORD(lParam) - 220, HIWORD(lParam) - 25, FALSE);
        return 0;

    case WM_COMMAND:
        if (HIWORD(wParam) == LBN_DBLCLK && LOWORD(wParam) == IDC_LISTBOX)
        {
            BOOL bRet;
            HANDLE hFile;
            BYTE byBuf[8192] = { 0 };   // 最多读取8K字节文本
            DWORD dwBytesReaded;        // 实际读取字节数

            // 当前选定的列表项的文本内容，如果当前选择是目录名称，则返回值为TRUE，否则FALSE
            bRet = DlgDirSelectEx(hwnd, szStr, MAX_PATH, IDC_LISTBOX);
            if (bRet)
            {
                // 当前选择是目录名称
                DlgDirList(hwnd, szStr, IDC_LISTBOX, IDC_STATICPATH,
                    DDL_ARCHIVE | DDL_READONLY | DDL_SYSTEM | DDL_DIRECTORY | DDL_DRIVES);
            } 
            else
            {
                // 当前选择是文件
                hFile = CreateFile(szStr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
                ReadFile(hFile, byBuf, _countof(byBuf) - 2, &dwBytesReaded, NULL);
                // 简单的判断是否是Unicode格式文本
                if (IsTextUnicode(byBuf, dwBytesReaded, 0))
                    SetDlgItemTextW(hwnd, IDC_EDIT, (LPWSTR)byBuf);
                else
                    SetDlgItemTextA(hwnd, IDC_EDIT, (LPSTR)byBuf);
            }
        }
        return 0;

    case WM_CTLCOLORSTATIC:
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_DESTROY:
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}