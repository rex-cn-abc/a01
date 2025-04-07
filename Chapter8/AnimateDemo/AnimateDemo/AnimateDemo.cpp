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
    TCHAR szAppName[] = TEXT("AnimateDemo");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 430, 380, NULL, NULL, hInstance, NULL);

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
    static HWND hwndAnimate, hwndBtnOpen, hwndBtnPlay, hwndBtnStop, hwndBtnClose;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 动画控件
        hwndAnimate = CreateWindowEx(0, TEXT("SysAnimate32"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ACS_CENTER,
            10, 0, 330, 330, hwnd, (HMENU)IDC_ANIMATE, hInstance, NULL);

        // 打开、播放、停止、关闭按钮
        hwndBtnOpen = CreateWindowEx(0, TEXT("Button"), TEXT("打开"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            350, 0, 50, 20, hwnd, (HMENU)IDC_BTNOPEN, hInstance, NULL);
        hwndBtnPlay = CreateWindowEx(0, TEXT("Button"), TEXT("播放"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            350, 25, 50, 20, hwnd, (HMENU)IDC_BTNPLAY, hInstance, NULL);
        hwndBtnStop = CreateWindowEx(0, TEXT("Button"), TEXT("停止"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            350, 50, 50, 20, hwnd, (HMENU)IDC_BTNSTOP, hInstance, NULL);
        hwndBtnClose = CreateWindowEx(0, TEXT("Button"), TEXT("关闭"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            350, 75, 50, 20, hwnd, (HMENU)IDC_BTNCLOSE, hInstance, NULL);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTNOPEN:
            SendMessage(hwndAnimate, ACM_OPEN, NULL, (LPARAM)TEXT("Colck.avi"));
            break;
        case IDC_BTNPLAY:
            SendMessage(hwndAnimate, ACM_PLAY, -1, MAKELPARAM(0, -1));
            break;
        case IDC_BTNSTOP:
            SendMessage(hwndAnimate, ACM_STOP, NULL, NULL);
            break;
        case IDC_BTNCLOSE:
            SendMessage(hwndAnimate, ACM_OPEN, NULL, NULL);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}