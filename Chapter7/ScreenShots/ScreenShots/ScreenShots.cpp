#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

// 全局变量
BOOL g_bCapturing;      // 是否正在截图
INT g_nLBtnDownCount;   // 第一次鼠标按下作为截图区域的左上角，第二次作为右下角

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 第1次鼠标按下，画十字线
VOID DrawCrossLine(HWND hwnd, POINT ptBegin, int cxScreen, int cyScreen, HDC hdcMem);
// 第2次鼠标按下，画矩形
VOID DrawRect(HWND hwnd, POINT ptBegin, POINT ptEnd, int cxScreen, int cyScreen, HDC hdcMem);
// 保存图片
VOID SaveBmp(HDC hdc, HBITMAP hBitmap, LONG lWidth, LONG lHeight);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("ScreenShots");
    HWND hwnd;
    MSG msg;
    HMENU hMenu;
    HACCEL hAccel;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EAGLE));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, hMenu, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACC));
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (!TranslateAccelerator(hwnd, hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HMENU hMenu;                         // 主菜单句柄
    static int cxScreen, cyScreen;              // 屏幕宽度、高度
    static HDC hdcDesk, hdcMem, hdcMemResult;   // 屏幕DC、内存DC、截图结果用的内存DC句柄
    static HBITMAP hBitmapMem, hBitmapResult;   // 设备兼容位图、截图结果用的设备兼容位图句柄
    static POINT ptBegin, ptEnd;                // 截图的左上角和右下角坐标
    LONG lWidth, lHeight;                       // 截图的宽度、高度

    switch (uMsg)
    {
    case WM_CREATE:
        hMenu = ((CREATESTRUCT *)lParam)->hMenu;
        cxScreen = GetSystemMetrics(SM_CXSCREEN);
        cyScreen = GetSystemMetrics(SM_CYSCREEN);

        // 内存DC
        hdcDesk = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        hdcMem = CreateCompatibleDC(hdcDesk);
        hBitmapMem = CreateCompatibleBitmap(hdcDesk, cxScreen, cyScreen);
        SelectObject(hdcMem, hBitmapMem);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        if (!g_bCapturing)
            BitBlt(hdc, 0, 0, ptEnd.x - ptBegin.x, ptEnd.y - ptBegin.y, 
                hdcMem, ptBegin.x, ptBegin.y, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_START:
            hdc = GetDC(hwnd);
            // 先最小化本程序，截取整个屏幕内容到hdcMem
            SetMenu(hwnd, NULL);
            ShowWindow(hwnd, SW_MINIMIZE);
            Sleep(500);
            BitBlt(hdcMem, 0, 0, cxScreen, cyScreen, hdcDesk, 0, 0, SRCCOPY);

            // 去掉程序的标题栏、边框等，然后最大化显示，客户区中显示的是整个屏幕的内容
            SetWindowLongPtr(hwnd, GWL_STYLE, (LONG)(WS_OVERLAPPED | WS_VISIBLE));
            ShowWindow(hwnd, SW_RESTORE);
            SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);
            BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);
            ReleaseDC(hwnd, hdc);

            SetCursor(LoadCursor(NULL, IDC_CROSS));
            ptBegin = ptEnd = { 0, 0 };
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);
            g_bCapturing = TRUE;
            break;

        case ID_SAVE:
            lWidth = ptEnd.x - ptBegin.x + 1;
            lHeight = ptEnd.y - ptBegin.y + 1;
            hdc = GetDC(hwnd);
            hdcMemResult = CreateCompatibleDC(hdc);
            hBitmapResult = CreateCompatibleBitmap(hdc, lWidth, lHeight);
            SelectObject(hdcMemResult, hBitmapResult);
            BitBlt(hdcMemResult, 0, 0, lWidth, lHeight, hdcMem, ptBegin.x, ptBegin.y, SRCCOPY);
            SaveBmp(hdc, hBitmapResult, lWidth, lHeight);
            ReleaseDC(hwnd, hdc);
            break;

        case ID_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        return 0;

    case WM_LBUTTONDOWN:
        // 第一次按下鼠标左键
        if (g_bCapturing && g_nLBtnDownCount == 0)
        {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            // 擦除WM_MOUSEMOVE消息中最后一次画的十字线
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);
            // 初始化ptEnd
            ptEnd = ptBegin;
            g_nLBtnDownCount = 1;
        }
        // 第二次按下鼠标左键
        else if (g_bCapturing && g_nLBtnDownCount == 1)
        {
            g_bCapturing = FALSE;
            g_nLBtnDownCount = 0;

            DrawRect(hwnd, ptBegin, ptEnd, cxScreen, cyScreen, hdcMem);
            SetWindowLongPtr(hwnd, GWL_STYLE, (LONG)(WS_OVERLAPPEDWINDOW | WS_VISIBLE));
            SetMenu(hwnd, hMenu);
            SetWindowPos(hwnd, HWND_NOTOPMOST, 200, 100, 400, 300, SWP_SHOWWINDOW);
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            // 结束点的坐标有可能小于起始点
            if (ptEnd.x < ptBegin.x)
            {
                int x = ptBegin.x;
                ptBegin.x = ptEnd.x;
                ptEnd.x = x;
            }
            if (ptEnd.y < ptBegin.y)
            {
                int y = ptBegin.y;
                ptBegin.y = ptEnd.y;
                ptEnd.y = y;
            }
        }
        return 0;

    case WM_MOUSEMOVE:
        if (g_bCapturing && g_nLBtnDownCount == 0)
        {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            // 擦除上一条十字线
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);

            ptBegin.x = LOWORD(lParam);
            ptBegin.y = HIWORD(lParam);
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);
        }
        else if (g_bCapturing && g_nLBtnDownCount == 1)
        {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            // 擦除上一个矩形
            DrawRect(hwnd, ptBegin, ptEnd, cxScreen, cyScreen, hdcMem);

            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);
            DrawRect(hwnd, ptBegin, ptEnd, cxScreen, cyScreen, hdcMem);
        }
        return 0;

    case WM_DESTROY:
        DeleteObject(hBitmapMem);
        DeleteDC(hdcDesk);
        DeleteDC(hdcMem);
        if (hBitmapResult)  // 没有点击保存图片就不会创建hdcMemResult和hBitmapResult
            DeleteObject(hBitmapResult);
        if (hdcMemResult)
            DeleteDC(hdcMemResult);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

VOID DrawCrossLine(HWND hwnd, POINT ptBegin, int cxScreen, int cyScreen, HDC hdcMem)
{
    HDC hdc = GetDC(hwnd);
    SelectObject(hdcMem, CreatePen(PS_SOLID, 1, RGB(0, 255, 0)));
    SetROP2(hdcMem, R2_XORPEN);
    MoveToEx(hdcMem, 0, ptBegin.y, NULL);
    LineTo(hdcMem, cxScreen, ptBegin.y);
    MoveToEx(hdcMem, ptBegin.x, 0, NULL);
    LineTo(hdcMem, ptBegin.x, cyScreen);
    BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);
    DeleteObject(SelectObject(hdcMem, GetStockObject(BLACK_PEN)));
    ReleaseDC(hwnd, hdc);
}

VOID DrawRect(HWND hwnd, POINT ptBegin, POINT ptEnd, int cxScreen, int cyScreen, HDC hdcMem)
{
    HDC hdc = GetDC(hwnd);
    SelectObject(hdcMem, CreatePen(PS_SOLID, 1, RGB(125, 0, 125)));
    SetROP2(hdcMem, R2_NOTXORPEN);
    Rectangle(hdcMem, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
    BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);
    DeleteObject(SelectObject(hdcMem, GetStockObject(BLACK_PEN)));
    ReleaseDC(hwnd, hdc);
}

VOID SaveBmp(HDC hdc, HBITMAP hBitmap, LONG lWidth, LONG lHeight)
{
    BITMAPFILEHEADER bmfh = { 0 };
    BITMAPINFOHEADER bmih = { 0 };
    HANDLE hFile;
    DWORD dwPixelSize;
    LPVOID lpBmpData;

    dwPixelSize = lWidth * lHeight * 4; // 32位位图
    lpBmpData = new BYTE[dwPixelSize];
    DWORD dwBytesWritten;
    SYSTEMTIME stLocal;
    TCHAR szFileName[32] = { 0 };

    GetLocalTime(&stLocal);
    wsprintf(szFileName, TEXT("%d%0.2d%0.2d-%0.2d%0.2d%0.2d"),
        stLocal.wYear, stLocal.wMonth, stLocal.wDay,
        stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
    StringCchCat(szFileName, _countof(szFileName), TEXT(".bmp"));

    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biWidth = lWidth;
    bmih.biHeight = lHeight;
    bmih.biPlanes = 1;
    bmih.biBitCount = 32;
    bmih.biCompression = BI_RGB;

    bmfh.bfType = 0x4D42;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfh.bfSize = dwPixelSize + bmfh.bfOffBits;

    GetDIBits(hdc, hBitmap, 0, lHeight, lpBmpData, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
    hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, &bmih, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, lpBmpData, dwPixelSize, &dwBytesWritten, NULL);
    delete[] lpBmpData;
}