#include <Windows.h>
#include <tchar.h>
#include "resource.h"

// 全局变量
HINSTANCE g_hInstance;

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("DDB");
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
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
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
    PAINTSTRUCT ps;
    HDC hdc, hdcMem;
    HBITMAP hBitmap;
    BITMAP bmp;

    switch (uMsg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        hdcMem = CreateCompatibleDC(hdc);
        //hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_GIRL));
        hBitmap = (HBITMAP)LoadImage(NULL, TEXT("Girl.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        SelectObject(hdcMem, hBitmap);

        // 绘图操作
        SetBkMode(hdcMem, TRANSPARENT);
        TextOut(hdcMem, 10, 10, TEXT("窈窕淑女 君子好逑"), _tcslen(TEXT("窈窕淑女 君子好逑")));

        // 把内存位图复制到窗口客户区中
        GetObject(hBitmap, sizeof(bmp), &bmp);
        BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}