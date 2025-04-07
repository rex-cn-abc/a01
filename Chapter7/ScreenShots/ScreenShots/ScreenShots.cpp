#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

// ȫ�ֱ���
BOOL g_bCapturing;      // �Ƿ����ڽ�ͼ
INT g_nLBtnDownCount;   // ��һ����갴����Ϊ��ͼ��������Ͻǣ��ڶ�����Ϊ���½�

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ��1����갴�£���ʮ����
VOID DrawCrossLine(HWND hwnd, POINT ptBegin, int cxScreen, int cyScreen, HDC hdcMem);
// ��2����갴�£�������
VOID DrawRect(HWND hwnd, POINT ptBegin, POINT ptEnd, int cxScreen, int cyScreen, HDC hdcMem);
// ����ͼƬ
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
    static HMENU hMenu;                         // ���˵����
    static int cxScreen, cyScreen;              // ��Ļ��ȡ��߶�
    static HDC hdcDesk, hdcMem, hdcMemResult;   // ��ĻDC���ڴ�DC����ͼ����õ��ڴ�DC���
    static HBITMAP hBitmapMem, hBitmapResult;   // �豸����λͼ����ͼ����õ��豸����λͼ���
    static POINT ptBegin, ptEnd;                // ��ͼ�����ϽǺ����½�����
    LONG lWidth, lHeight;                       // ��ͼ�Ŀ�ȡ��߶�

    switch (uMsg)
    {
    case WM_CREATE:
        hMenu = ((CREATESTRUCT *)lParam)->hMenu;
        cxScreen = GetSystemMetrics(SM_CXSCREEN);
        cyScreen = GetSystemMetrics(SM_CYSCREEN);

        // �ڴ�DC
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
            // ����С�������򣬽�ȡ������Ļ���ݵ�hdcMem
            SetMenu(hwnd, NULL);
            ShowWindow(hwnd, SW_MINIMIZE);
            Sleep(500);
            BitBlt(hdcMem, 0, 0, cxScreen, cyScreen, hdcDesk, 0, 0, SRCCOPY);

            // ȥ������ı��������߿�ȣ�Ȼ�������ʾ���ͻ�������ʾ����������Ļ������
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
        // ��һ�ΰ���������
        if (g_bCapturing && g_nLBtnDownCount == 0)
        {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            // ����WM_MOUSEMOVE��Ϣ�����һ�λ���ʮ����
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);
            // ��ʼ��ptEnd
            ptEnd = ptBegin;
            g_nLBtnDownCount = 1;
        }
        // �ڶ��ΰ���������
        else if (g_bCapturing && g_nLBtnDownCount == 1)
        {
            g_bCapturing = FALSE;
            g_nLBtnDownCount = 0;

            DrawRect(hwnd, ptBegin, ptEnd, cxScreen, cyScreen, hdcMem);
            SetWindowLongPtr(hwnd, GWL_STYLE, (LONG)(WS_OVERLAPPEDWINDOW | WS_VISIBLE));
            SetMenu(hwnd, hMenu);
            SetWindowPos(hwnd, HWND_NOTOPMOST, 200, 100, 400, 300, SWP_SHOWWINDOW);
            SetCursor(LoadCursor(NULL, IDC_ARROW));

            // ������������п���С����ʼ��
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
            // ������һ��ʮ����
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);

            ptBegin.x = LOWORD(lParam);
            ptBegin.y = HIWORD(lParam);
            DrawCrossLine(hwnd, ptBegin, cxScreen, cyScreen, hdcMem);
        }
        else if (g_bCapturing && g_nLBtnDownCount == 1)
        {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            // ������һ������
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
        if (hBitmapResult)  // û�е������ͼƬ�Ͳ��ᴴ��hdcMemResult��hBitmapResult
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

    dwPixelSize = lWidth * lHeight * 4; // 32λλͼ
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