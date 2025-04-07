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
    TCHAR szAppName[] = TEXT("ProgressDemo");
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
    static HWND hwndProgress, hwndStatic;
    TCHAR szBuf[16] = { 0 };
    UINT nPos;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 进度条控件
        hwndProgress = CreateWindowEx(0, TEXT("msctls_progress32"), NULL,
            WS_CHILD | WS_VISIBLE,
            10, 10, 300, 20, hwnd, (HMENU)IDC_PROGRESS, hInstance, NULL);

        // 静态控件
        hwndStatic = CreateWindowEx(0, TEXT("Static"), TEXT("%0"),
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            320, 10, 60, 20, hwnd, (HMENU)(-1), hInstance, NULL);

        // 设置进度条控件的范围
        SendMessage(hwndProgress, PBM_SETRANGE32, 0, 100);

        // 计时器
        SetTimer(hwnd, 1, 100, NULL);
        return 0;

    case WM_TIMER:
        // 获取进度条的当前位置
        nPos = SendMessage(hwndProgress, PBM_GETPOS, 0, 0);
        if (nPos >= 100)
        {
            Sleep(2000);
            nPos = 0;
        }
        else
        {
            nPos++;
        }

        // 设置进度条的当前位置
        SendMessage(hwndProgress, PBM_SETPOS, nPos, 0);

        // 显示进度
        wsprintf(szBuf, TEXT("%%%d"), nPos);
        SetWindowText(hwndStatic, szBuf);
        return 0;

    case WM_CTLCOLORSTATIC:
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}