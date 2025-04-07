#include <Windows.h>
#include <tchar.h>

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("LocalTime");
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
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    SIZE size;
    SYSTEMTIME stLocal;
    LPTSTR arrWeek[] = { TEXT("星期日"), TEXT("星期一"), TEXT("星期二"), TEXT("星期三"),
        TEXT("星期四"), TEXT("星期五"), TEXT("星期六") };
    TCHAR szBuf[32] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        // 设置窗口大小
        GetLocalTime(&stLocal);
        wsprintf(szBuf, TEXT("%d年%02d月%02d日 %s %02d:%02d:%02d"), 
            stLocal.wYear, stLocal.wMonth, stLocal.wDay, arrWeek[stLocal.wDayOfWeek], 
            stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
        GetTextExtentPoint32(hdc, szBuf, _tcslen(szBuf), &size);
        SetRect(&rect, 0, 0, size.cx, size.cy);
        AdjustWindowRectEx(&rect, GetWindowLongPtr(hwnd, GWL_STYLE),
            GetMenu(hwnd) != NULL, GetWindowLongPtr(hwnd, GWL_EXSTYLE));
        SetWindowPos(hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOZORDER | SWP_NOMOVE);
        ReleaseDC(hwnd, hdc);
        // 创建计时器
        SetTimer(hwnd, 1, 1000, NULL);
        return 0;

    case WM_TIMER:
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetLocalTime(&stLocal);
        wsprintf(szBuf, TEXT("%d年%02d月%02d日 %s %02d:%02d:%02d"), 
            stLocal.wYear, stLocal.wMonth, stLocal.wDay, arrWeek[stLocal.wDayOfWeek], 
            stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
        TextOut(hdc, 0, 0, szBuf, _tcslen(szBuf));
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}