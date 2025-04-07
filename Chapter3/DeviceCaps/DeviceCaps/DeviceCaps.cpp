#include <Windows.h>
#include <tchar.h>
#include "Caps.h"

const int NUMLINES = sizeof(CAPS) / sizeof(CAPS[0]);

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                        // RegisterClassEx�����õ�WNDCLASSEX�ṹ
    TCHAR szClassName[] = TEXT("MyWindow");     // RegisterClassEx����ע��Ĵ����������
    TCHAR szAppName[] = TEXT("GetDeviceCaps");  // ���ڱ���
    HWND hwnd;                                  // CreateWindowEx���������Ĵ��ڵľ��
    MSG msg;                                    // ��Ϣѭ�����õ���Ϣ�ṹ��

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
    HFONT hFont, hFontOld;
    static int s_nCol1, s_nCol2, s_nCol3, s_nHeight;// ��1��3���ַ���������ȣ��ַ����߶�
    SIZE size;
    int y;
    TCHAR szBuf[10];

    switch (uMsg)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        for (int i = 0; i < NUMLINES; i++)
        {
            GetTextExtentPoint32(hdc, CAPS[i].m_pLabel, _tcslen(CAPS[i].m_pLabel), &size);
            if (size.cx > s_nCol1)
                s_nCol1 = size.cx;
            GetTextExtentPoint32(hdc, CAPS[i].m_pDesc, _tcslen(CAPS[i].m_pDesc), &size);
            if (size.cx > s_nCol2)
                s_nCol2 = size.cx;
            GetTextExtentPoint32(hdc, szBuf,
                wsprintf(szBuf, TEXT("%d"), GetSystemMetrics(CAPS[i].m_nIndex)), &size);
            if (size.cx > s_nCol3)
                s_nCol3 = size.cx;
        }
        s_nHeight = size.cy + 2;
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        ReleaseDC(hwnd, hdc);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        for (int i = 0; i < NUMLINES; i++)
        {
            y = s_nHeight * i;
            TextOut(hdc, 0, y, CAPS[i].m_pLabel, _tcslen(CAPS[i].m_pLabel));
            TextOut(hdc, s_nCol1 + 10, y, CAPS[i].m_pDesc, _tcslen(CAPS[i].m_pDesc));
            TextOut(hdc, s_nCol1 + s_nCol2 + 20, y, szBuf,
                wsprintf(szBuf, TEXT("%d"), GetDeviceCaps(hdc, CAPS[i].m_nIndex)));
        }
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}