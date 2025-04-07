#include <Windows.h>
#include <tchar.h>
#include "Metrics.h"

const int NUMLINES = sizeof(METRICS) / sizeof(METRICS[0]);

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                            // RegisterClassEx函数用的WNDCLASSEX结构
    TCHAR szClassName[] = TEXT("MyWindow");         // RegisterClassEx函数注册的窗口类的名称
    TCHAR szAppName[] = TEXT("GetSystemMetrics");   // 窗口标题
    HWND hwnd;                                      // CreateWindowEx函数创建的窗口的句柄
    MSG msg;                                        // 消息循环所用的消息结构体

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);    // 窗口背景使用标准系统颜色
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
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
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR szBuf[10];
    int y;

    switch (uMsg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        for (int i = 0; i < NUMLINES; i++)
        {
            y = 18 * i;
            TextOut(hdc,    0,      y,  METRICS[i].m_pLabel,    _tcslen(METRICS[i].m_pLabel));
            TextOut(hdc,    240,    y,  METRICS[i].m_pDesc,     _tcslen(METRICS[i].m_pDesc));
            TextOut(hdc,    760,    y,  szBuf,
                wsprintf(szBuf, TEXT("%d"), GetSystemMetrics(METRICS[i].m_nIndex)));
        }
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}