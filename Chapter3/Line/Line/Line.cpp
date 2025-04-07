#include <Windows.h>
#include <tchar.h>

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("直线");
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
    wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 300, NULL, NULL, hInstance, NULL);

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
    POINT arrPtPolyPolyline[] = {                           // PolyPolyline函数的点
        110,60, 10,60, 60,10, 160,10,
        10,60, 10,160, 110,160, 110,60, 160,10, 160,110, 110,160,
        /*10,160, 60,110, 160,110,
        60,110, 60,10*/
    };
    DWORD arrGroup[] = { 4, 7 };
    POINT arrPtPolyline[] = { 10,220, 110,200, 210,220 };   // Polyline函数的点
    POINT arrPtPolylineTo[] = { 110,260, 210,240 };         // PolylineTo函数的点

    switch (uMsg)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        // 宽度为3的红色实心画笔画立方体
        SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(255, 0, 0)));
        PolyPolyline(hdc, arrPtPolyPolyline, arrGroup, _countof(arrGroup));

        // 绿色划线画笔画1条线
        DeleteObject(SelectObject(hdc, CreatePen(PS_DASH, 1, RGB(0, 255, 0))));
        MoveToEx(hdc, 10, 180, NULL);
        LineTo(hdc, 210, 180);

        // 蓝色点线画笔画2条线
        DeleteObject(SelectObject(hdc, CreatePen(PS_DOT, 1, RGB(0, 0, 255))));
        Polyline(hdc, arrPtPolyline, _countof(arrPtPolyline));

        // 黑色点划线画笔画2条线
        DeleteObject(SelectObject(hdc, CreatePen(PS_DASHDOT, 1, RGB(0, 0, 0))));
        MoveToEx(hdc, 10, 240, NULL);
        PolylineTo(hdc, arrPtPolylineTo, _countof(arrPtPolylineTo));

        DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}