#include <Windows.h>
#include <tchar.h>
#include "Metrics.h"

const int NUMLINES = sizeof(METRICS) / sizeof(METRICS[0]);

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                            // RegisterClassEx�����õ�WNDCLASSEX�ṹ
    TCHAR szClassName[] = TEXT("MyWindow");         // RegisterClassEx����ע��Ĵ����������
    TCHAR szAppName[] = TEXT("GetSystemMetrics");   // ���ڱ���
    HWND hwnd;                                      // CreateWindowEx���������Ĵ��ڵľ��
    MSG msg;                                        // ��Ϣѭ�����õ���Ϣ�ṹ��

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

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
    TEXTMETRIC tm;
    SCROLLINFO si;
    HFONT hFont, hFontOld;
    static int s_iCol1, s_iCol2, s_iCol3, s_iHeight;// ��1��3���ַ���������ȣ��ַ����߶�
    static int s_cxClient, s_cyClient;              // �ͻ�����ȡ��߶�
    static int s_cxChar;                            // ƽ���ַ���ȣ�����ˮƽ������������λ
    int iVertPos, iHorzPos;                         // ��ֱ��ˮƽ�������ĵ�ǰλ��
    SIZE size = { 0 };
    int x, y;
    TCHAR szBuf[10];
    int nPaintBeg, nPaintEnd;                       // ��Ч����
    //RECT rect;

    switch (uMsg)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));
        hFontOld = (HFONT)SelectObject(hdc, hFont);
        for (int i = 0; i < NUMLINES; i++)
        {
            GetTextExtentPoint32(hdc, METRICS[i].m_pLabel, _tcslen(METRICS[i].m_pLabel), &size);
            if (size.cx > s_iCol1)
                s_iCol1 = size.cx;
            GetTextExtentPoint32(hdc, METRICS[i].m_pDesc, _tcslen(METRICS[i].m_pDesc), &size);
            if (size.cx > s_iCol2)
                s_iCol2 = size.cx;
            GetTextExtentPoint32(hdc, szBuf,
                wsprintf(szBuf, TEXT("%d"), GetSystemMetrics(METRICS[i].m_nIndex)), &size);
            if (size.cx > s_iCol3)
                s_iCol3 = size.cx;
        }
        s_iHeight = size.cy + 2;

        GetTextMetrics(hdc, &tm);
        s_cxChar = tm.tmAveCharWidth;

        //GetClientRect(hwnd, &rect);
        //rect.right = s_iCol1 + s_iCol2 + s_iCol3 + GetSystemMetrics(SM_CXVSCROLL);
        //AdjustWindowRectEx(&rect, GetWindowLongPtr(hwnd, GWL_STYLE),
        //    GetMenu(hwnd) != NULL, GetWindowLongPtr(hwnd, GWL_EXSTYLE));
        ////MoveWindow(hwnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        //SetWindowPos(hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
        //    SWP_NOZORDER | SWP_NOMOVE);

        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        ReleaseDC(hwnd, hdc);
        return 0;

    case WM_SIZE:
        // �ͻ�����ȡ��߶�
        s_cxClient = LOWORD(lParam);
        s_cyClient = HIWORD(lParam);
        // ���ô�ֱ�������ķ�Χ��ҳ���С
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = NUMLINES - 1;
        si.nPage = s_cyClient / s_iHeight;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        // ����ˮƽ�������ķ�Χ��ҳ���С
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = (s_iCol1 + s_iCol2 + s_iCol3) / s_cxChar - 1;
        si.nPage = s_cxClient / s_cxChar;
        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
        return 0;

    case WM_VSCROLL:
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);
        iVertPos = si.nPos;
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            si.nPos -= 1;
            break;
        case SB_LINEDOWN:
            si.nPos += 1;
            break;
        case SB_PAGEUP:
            si.nPos -= si.nPage;
            break;
        case SB_PAGEDOWN:
            si.nPos += si.nPage;
            break;
        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
        }
        // ����λ�ã�Ȼ���ȡλ�ã����si.nPosԽ�磬Windows��������
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hwnd, SB_VERT, &si);
        // ���Windows�����˹�����λ�ã����Ǹ��¿ͻ���
        if (iVertPos != si.nPos)
        {
            ScrollWindow(hwnd, 0, s_iHeight * (iVertPos - si.nPos), NULL, NULL);
            UpdateWindow(hwnd);
        }
        return 0;

    case WM_HSCROLL:
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_HORZ, &si);
        iHorzPos = si.nPos;
        switch (LOWORD(wParam))
        {
        case SB_LINELEFT:
            si.nPos -= 1;
            break;
        case SB_LINERIGHT:
            si.nPos += 1;
            break;
        case SB_PAGELEFT:
            si.nPos -= si.nPage;
            break;
        case SB_PAGERIGHT:
            si.nPos += si.nPage;
            break;
        case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
        }
        // ����λ�ã�Ȼ���ȡλ�ã����si.nPosԽ�磬Windows��������
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
        GetScrollInfo(hwnd, SB_HORZ, &si);
        // ���Windows�����˹�����λ�ã����Ǹ��¿ͻ���
        if (iHorzPos != si.nPos)
        {
            ScrollWindow(hwnd, s_cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
            UpdateWindow(hwnd);
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // ��ȡ��ֱ��������ˮƽ������λ��
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS | SIF_PAGE;
        GetScrollInfo(hwnd, SB_VERT, &si);
        iVertPos = si.nPos;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS;
        GetScrollInfo(hwnd, SB_HORZ, &si);
        iHorzPos = si.nPos;

        SetBkMode(hdc, TRANSPARENT);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));
        hFontOld = (HFONT)SelectObject(hdc, hFont);

        // ��ȡ��Ч����
        nPaintBeg = max(0, iVertPos + ps.rcPaint.top / s_iHeight);
        nPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / s_iHeight);
        for (int i = nPaintBeg; i <= nPaintEnd; i++)
        {
            x = s_cxChar * (-iHorzPos);
            y = s_iHeight * (i - iVertPos);
            TextOut(hdc, x,                     y, METRICS[i].m_pLabel, _tcslen(METRICS[i].m_pLabel));
            TextOut(hdc, x + s_iCol1,           y, METRICS[i].m_pDesc,  _tcslen(METRICS[i].m_pDesc));
            TextOut(hdc, x + s_iCol1 + s_iCol2, y, szBuf,
                wsprintf(szBuf, TEXT("%d"), GetSystemMetrics(METRICS[i].m_nIndex)));
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