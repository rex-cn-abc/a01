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

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
    static BOOL IsCalcStr = TRUE;           // 只在第一次WM_PAINT消息中计算s_iCol1、s_iCol2、s_iHeight
    static int s_iCol1, s_iCol2, s_iHeight; // 第一列、第二列字符串的最大宽度，字符串高度
    static int s_cxClient, s_cyClient;      // 客户区宽度、高度
    static int s_iVscrollPos;               // 垂直滚动条当前位置
    TCHAR szBuf[10];
    int y;

    switch (uMsg)
    {
    case WM_CREATE:
        // 设置垂直滚动条的范围和初始位置
        SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
        SetScrollPos(hwnd, SB_VERT, s_iVscrollPos, TRUE);
        return 0;

    case WM_SIZE:
        // 计算客户区宽度、高度，滚动条滚动一页的时候需要使用
        s_cxClient = LOWORD(lParam);
        s_cyClient = HIWORD(lParam);
        return 0;

    case WM_VSCROLL:
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            s_iVscrollPos -= 1;
            break;
        case SB_LINEDOWN:
            s_iVscrollPos += 1;
            break;
        case SB_PAGEUP:
            s_iVscrollPos -= s_cyClient / s_iHeight;
            break;
        case SB_PAGEDOWN:
            s_iVscrollPos += s_cyClient / s_iHeight;
            break;
        case SB_THUMBTRACK:
            s_iVscrollPos = HIWORD(wParam);
            break;
        }
        s_iVscrollPos = min(s_iVscrollPos, NUMLINES - 1);
        s_iVscrollPos = max(0, s_iVscrollPos);
        if (s_iVscrollPos != GetScrollPos(hwnd, SB_VERT))
        {
            SetScrollPos(hwnd, SB_VERT, s_iVscrollPos, TRUE);
            //UpdateWindow(hwnd);
            //InvalidateRect(hwnd, NULL, FALSE);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("宋体"));
        hFontOld = (HFONT)SelectObject(hdc, hFont);

        if (IsCalcStr)
        {
            SIZE size = { 0 };
            for (int i = 0; i < NUMLINES; i++)
            {
                GetTextExtentPoint32(hdc, METRICS[i].m_pLabel, _tcslen(METRICS[i].m_pLabel), &size);
                if (size.cx > s_iCol1)
                    s_iCol1 = size.cx;
                GetTextExtentPoint32(hdc, METRICS[i].m_pDesc, _tcslen(METRICS[i].m_pDesc), &size);
                if (size.cx > s_iCol2)
                    s_iCol2 = size.cx;
            }
            s_iHeight = size.cy + 2;    // 留一点行间距
            IsCalcStr = FALSE;
        }

        for (int i = 0; i < NUMLINES; i++)
        {
            y = s_iHeight * (i - s_iVscrollPos);
            TextOut(hdc, 0,                 y, METRICS[i].m_pLabel, _tcslen(METRICS[i].m_pLabel));
            TextOut(hdc, s_iCol1,           y, METRICS[i].m_pDesc,  _tcslen(METRICS[i].m_pDesc));
            TextOut(hdc, s_iCol1 + s_iCol2, y, szBuf,
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