#include <Windows.h>
#include <tchar.h>
#include "resource.h"

// 全局变量
HINSTANCE g_hInstance;      // 程序实例句柄

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("ExtCreatePen2");
    HWND hwnd;
    MSG msg;

    g_hInstance = hInstance;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        100, 100, 560, 180, NULL, NULL, hInstance, NULL);

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
    HPEN hPen;
    LOGBRUSH logBrush;

    switch (uMsg)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        logBrush.lbStyle = BS_PATTERN;      // 位图图案画刷
        // LoadBitmap用于加载一副苹果图标的位图，这个函数后面再讲
        logBrush.lbHatch = (ULONG_PTR)LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_APPLE));

        hPen = ExtCreatePen(PS_GEOMETRIC, 100, &logBrush, 0, NULL);
        SelectObject(hdc, hPen);
        MoveToEx(hdc, 70, 70, NULL);
        LineTo(hdc, 470, 70);

        DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
        MoveToEx(hdc, 70, 70, NULL);
        LineTo(hdc, 470, 70);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}