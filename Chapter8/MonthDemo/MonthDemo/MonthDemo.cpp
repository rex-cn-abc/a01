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
    TCHAR szAppName[] = TEXT("MonthDemo");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

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
    static HWND hwndMonthCal, hwndBtnGetDate;
    RECT rect;
    SYSTEMTIME st;
    TCHAR szBuf[64] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 日期控件
        hwndMonthCal = CreateWindowEx(0, TEXT("SysMonthCal32"), NULL,
            WS_CHILD | WS_VISIBLE | MCS_WEEKNUMBERS,
            0, 0, 0, 0, hwnd, (HMENU)IDC_MONTHCAL, hInstance, NULL);

        // 根据日期控件所需的最小大小调整其位置
        SendMessage(hwndMonthCal, MCM_GETMINREQRECT, 0, (LPARAM)&rect);
        MoveWindow(hwndMonthCal, 10, 0, rect.right, rect.bottom, FALSE);

        // 获取日期按钮
        hwndBtnGetDate = CreateWindowEx(0, TEXT("Button"), TEXT("获取日期"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            270, 0, 80, 25, hwnd, (HMENU)IDC_BTNGETDATE, hInstance, NULL);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTNGETDATE:
            SendMessage(hwndMonthCal, MCM_GETCURSEL, 0, (LPARAM)&st);
            wsprintf(szBuf, TEXT("%d年%0.2d月%0.2d日"), st.wYear, st.wMonth, st.wDay);
            MessageBox(hwnd, szBuf, TEXT("提示"), MB_OK);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}