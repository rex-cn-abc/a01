#include <Windows.h>
#include <math.h>

// ��������
const int       CLOCK_SIZE = 200;             // �ӱ��ֱ��
const DOUBLE    TWOPI = 2 * 3.1415926;

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID DrawDots(HDC hdc);
VOID DrawLine(HDC hdc, int nAngle, int nRadiusAdjust);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("Clock");
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

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_POPUP | WS_SYSMENU,
        200, 100, CLOCK_SIZE, CLOCK_SIZE, NULL, NULL, hInstance, NULL);

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
    HRGN hRgn;
    SYSTEMTIME st;

    switch (uMsg)
    {
    case WM_CREATE:
        // ������Բ�ü�����
        hRgn = CreateEllipticRgn(0, 0, CLOCK_SIZE, CLOCK_SIZE);
        SetWindowRgn(hwnd, hRgn, TRUE);
        // ����Ϊ������ǰ
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        // ������ʱ��
        SetTimer(hwnd, 1, 1000, NULL);
        return 0;

    case WM_TIMER:
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_LBUTTONDOWN:
        // ��ס�����������϶�����
        SetCursor(LoadCursor(NULL, IDC_HAND));
        SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetLocalTime(&st);
        // ����
        SelectObject(hdc, GetStockObject(BLACK_BRUSH));
        DrawDots(hdc);

        // �����롢���롢ʱ��
        SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
        DrawLine(hdc, st.wSecond * 6, 10);      // ���룬������� = �� * 6

        DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 0))));
        DrawLine(hdc, st.wMinute * 6, 20);      // ���룬������� = �� * 6

        DeleteObject(SelectObject(hdc, CreatePen(PS_SOLID, 3, RGB(0, 0, 0))));
        // ʱ�룬ʱ����� = ʱ * 30 + �� / 2
        DrawLine(hdc, (st.wHour % 12) * 30 + st.wMinute / 2, 30);
        DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));

        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
        EndPaint(hwnd, &ps);
        return 0;

    case WM_CHAR:
        switch (wParam)
        {
        case '1':   // �������ڱ���
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND,
                (LONG)CreatePatternBrush((HBITMAP)LoadImage(NULL, TEXT("Back1.bmp"),
                    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)));
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case '2':   // �������ڱ���
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND,
                (LONG)CreatePatternBrush((HBITMAP)LoadImage(NULL, TEXT("Back2.bmp"),
                    IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)));
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case 0x1B:  // Esc
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

VOID DrawDots(HDC hdc)
{
    int x, y;
    int nRadius;

    for (int nAngle = 0; nAngle < 360; nAngle += 6)
    {
        x = CLOCK_SIZE / 2 + (int)((CLOCK_SIZE / 2 - 4) * sin(TWOPI * nAngle / 360));
        y = CLOCK_SIZE / 2 - (int)((CLOCK_SIZE / 2 - 4) * cos(TWOPI * nAngle / 360));
        // ÿ��30�Ȼ���Բ��Сʱ��Բ�㣬ÿ��6�Ȼ�СԲ�����ӵ�Բ��
        nRadius = nAngle % 5 ? 2 : 4;
        Ellipse(hdc, x - nRadius, y - nRadius, x + nRadius, y + nRadius);
    }
}

VOID DrawLine(HDC hdc, int nAngle, int nRadiusAdjust)
{
    int x1, y1, x2, y2;

    x1 = CLOCK_SIZE / 2 + (int)(((CLOCK_SIZE / 2 - 4) - nRadiusAdjust) * sin(TWOPI * nAngle / 360));
    y1 = CLOCK_SIZE / 2 - (int)(((CLOCK_SIZE / 2 - 4) - nRadiusAdjust) * cos(TWOPI * nAngle / 360));
    x2 = CLOCK_SIZE / 2 + (int)(10 * sin(TWOPI * ((DOUBLE)nAngle + 180) / 360));
    y2 = CLOCK_SIZE / 2 - (int)(10 * cos(TWOPI * ((DOUBLE)nAngle + 180) / 360));

    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}