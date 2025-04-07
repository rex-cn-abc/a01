#include <Windows.h>
#include <tchar.h>
#include "Metrics.h"

const int NUMLINES = sizeof(METRICS) / sizeof(METRICS[0]);

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                            // RegisterClassEx函数用的WNDCLASSEX结构
    TCHAR szClassName[] = TEXT("MyWindow");         // RegisterClassEx函数注册的窗口类的名称
    TCHAR szAppName[] = TEXT("GetSystemMetrics");   // 窗口标题
    HWND hwnd;                                      // CreateWindowEx函数创建的窗口的句柄
    MSG msg;                                        // 消息循环所用的消息结构体

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
    static int s_iCol1, s_iCol2, s_iCol3, s_iHeight;// 第1～3列字符串的最大宽度，字符串高度
    static int s_cxClient, s_cyClient;              // 客户区宽度、高度
    static int s_cxChar;                            // 平均字符宽度，用于水平滚动条滚动单位
    int iVertPos, iHorzPos;                         // 垂直、水平滚动条的当前位置
    SIZE size = { 0 };
    int x, y;
    TCHAR szBuf[10];
    int nPaintBeg, nPaintEnd;                       // 无效区域
    //RECT rect;

    switch (uMsg)
    {
    case WM_CREATE:
        hdc = GetDC(hwnd);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("宋体"));
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
        // 客户区宽度、高度
        s_cxClient = LOWORD(lParam);
        s_cyClient = HIWORD(lParam);
        // 设置垂直滚动条的范围和页面大小
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = NUMLINES - 1;
        si.nPage = s_cyClient / s_iHeight;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        // 设置水平滚动条的范围和页面大小
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
        // 设置位置，然后获取位置，如果si.nPos越界，Windows不会设置
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hwnd, SB_VERT, &si);
        // 如果Windows更新了滚动条位置，我们更新客户区
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
        // 设置位置，然后获取位置，如果si.nPos越界，Windows不会设置
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS;
        SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
        GetScrollInfo(hwnd, SB_HORZ, &si);
        // 如果Windows更新了滚动条位置，我们更新客户区
        if (iHorzPos != si.nPos)
        {
            ScrollWindow(hwnd, s_cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
            UpdateWindow(hwnd);
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // 获取垂直滚动条、水平滚动条位置
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS | SIF_PAGE;
        GetScrollInfo(hwnd, SB_VERT, &si);
        iVertPos = si.nPos;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_POS;
        GetScrollInfo(hwnd, SB_HORZ, &si);
        iHorzPos = si.nPos;

        SetBkMode(hdc, TRANSPARENT);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("宋体"));
        hFontOld = (HFONT)SelectObject(hdc, hFont);

        // 获取无效区域
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