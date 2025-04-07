#include <Windows.h>
#include <tchar.h>

// ȫ�ֱ���
HINSTANCE g_hInstance;

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("SRCPAINT");
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
    HDC hdc;
    static HBITMAP hBitmap, hBitmapMask;
    static BITMAP bmp;
    static HDC hdcMem, hdcMemMask;
    RECT rect;

    switch (uMsg)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        // Դλͼ���������ã�hdcMem
        hdcMem = CreateCompatibleDC(hdc);
        hBitmap = (HBITMAP)LoadImage(NULL, TEXT("Girl.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        SelectObject(hdcMem, hBitmap);
        GetObject(hBitmap, sizeof(bmp), &bmp);

        // ����λͼ����ɫ������ɫ��Բ��hdcMemMask
        hdcMemMask = CreateCompatibleDC(hdc);
        hBitmapMask = CreateCompatibleBitmap(hdc, bmp.bmWidth, bmp.bmHeight);
        SelectObject(hdcMemMask, hBitmapMask);

        SelectObject(hdcMemMask, GetStockObject(NULL_PEN));
        Rectangle(hdcMemMask, 0, 0, bmp.bmWidth + 1, bmp.bmHeight + 1);
        SelectObject(hdcMemMask, GetStockObject(BLACK_BRUSH));
        Ellipse(hdcMemMask, 0, 0, bmp.bmWidth + 1, bmp.bmHeight + 1);
        SelectObject(hdcMemMask, GetStockObject(BLACK_PEN));
        SelectObject(hdcMemMask, GetStockObject(WHITE_BRUSH));
        ReleaseDC(hwnd, hdc);

        // ����λͼ���Ƶ��ڴ�λͼ(����)����ִ�й�դ����
        SetRect(&rect, 0, 0, bmp.bmWidth, bmp.bmHeight);
        SetBkMode(hdcMem, TRANSPARENT);
        SetTextColor(hdcMem, RGB(255, 255, 255));
        DrawText(hdcMem, TEXT("����Ů ���Ӻ���"), _tcslen(TEXT("����Ů ���Ӻ���")),
            &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        BitBlt(hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMemMask, 0, 0, SRCPAINT);

        // ���ô��ڴ�С
        AdjustWindowRectEx(&rect, GetWindowLongPtr(hwnd, GWL_STYLE),
            GetMenu(hwnd) != NULL, GetWindowLongPtr(hwnd, GWL_EXSTYLE));
        SetWindowPos(hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOZORDER | SWP_NOMOVE);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // �Ѿ�ִ�й�դ�������ڴ�λͼ���Ƶ����ڿͻ�����
        BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        DeleteObject(hBitmap);
        DeleteObject(hBitmapMask);
        DeleteDC(hdcMem);
        DeleteDC(hdcMemMask);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}