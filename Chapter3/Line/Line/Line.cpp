#include <Windows.h>
#include <tchar.h>

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("ֱ��");
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
    POINT arrPtPolyPolyline[] = {                           // PolyPolyline�����ĵ�
        110,60, 10,60, 60,10, 160,10,
        10,60, 10,160, 110,160, 110,60, 160,10, 160,110, 110,160,
        /*10,160, 60,110, 160,110,
        60,110, 60,10*/
    };
    DWORD arrGroup[] = { 4, 7 };
    POINT arrPtPolyline[] = { 10,220, 110,200, 210,220 };   // Polyline�����ĵ�
    POINT arrPtPolylineTo[] = { 110,260, 210,240 };         // PolylineTo�����ĵ�

    switch (uMsg)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        // ���Ϊ3�ĺ�ɫʵ�Ļ��ʻ�������
        SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(255, 0, 0)));
        PolyPolyline(hdc, arrPtPolyPolyline, arrGroup, _countof(arrGroup));

        // ��ɫ���߻��ʻ�1����
        DeleteObject(SelectObject(hdc, CreatePen(PS_DASH, 1, RGB(0, 255, 0))));
        MoveToEx(hdc, 10, 180, NULL);
        LineTo(hdc, 210, 180);

        // ��ɫ���߻��ʻ�2����
        DeleteObject(SelectObject(hdc, CreatePen(PS_DOT, 1, RGB(0, 0, 255))));
        Polyline(hdc, arrPtPolyline, _countof(arrPtPolyline));

        // ��ɫ�㻮�߻��ʻ�2����
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