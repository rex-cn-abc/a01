#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 全局变量
HWND hwndChild[4];

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcChild(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("TabControlDemo");
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
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, hInstance, NULL);

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
    HINSTANCE hInstance;
    RECT rect;
    static HWND hwndTabControl;
    static HFONT hFont;
    TCITEM tci;
    int nIndex;

    WNDCLASSEX wndclass;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 根据父窗口客户区的大小创建选项卡控件
        GetClientRect(hwnd, &rect);
        hwndTabControl = CreateWindowEx(0, TEXT("SysTabControl32"), NULL,
            WS_CHILD | WS_VISIBLE | TCS_MULTILINE | TCS_TOOLTIPS,
            0, 0, rect.right, rect.bottom, hwnd, (HMENU)IDC_TABCONTROL, hInstance, NULL);

        // 添加选项卡
        tci.mask = TCIF_TEXT;
        tci.pszText = TEXT("平面设计");
        SendMessage(hwndTabControl, TCM_INSERTITEM, 0, (LPARAM)&tci);
        tci.pszText = TEXT("Web开发");
        SendMessage(hwndTabControl, TCM_INSERTITEM, 1, (LPARAM)&tci);
        tci.pszText = TEXT("Windows程序设计");
        SendMessage(hwndTabControl, TCM_INSERTITEM, 2, (LPARAM)&tci);
        tci.pszText = TEXT("加密解密");
        SendMessage(hwndTabControl, TCM_INSERTITEM, 3, (LPARAM)&tci);

        // 为4个选项卡创建对应的4个子窗口
        wndclass.cbSize = sizeof(WNDCLASSEX);
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WindowProcChild;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = TEXT("ChildWindow");
        wndclass.hIconSm = NULL;
        RegisterClassEx(&wndclass);
        for (int i = 0; i < 4; i++)
            hwndChild[i] = CreateWindowEx(0, TEXT("ChildWindow"), NULL, WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, NULL, hInstance, NULL);

        // 只显示当前选择的选项卡对应的子窗口(初始情况下索引为0的选项卡为选中状态)
        nIndex = SendMessage(hwndTabControl, TCM_GETCURSEL, 0, 0);
        for (int i = 0; i < 4; i++)
            ShowWindow(hwndChild[i], SW_HIDE);
        ShowWindow(hwndChild[nIndex], SW_SHOW);

        // 设置选项卡控件字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndTabControl, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_SIZE:
        // 根据父窗口客户区的大小调整选项卡控件的大小
        MoveWindow(hwndTabControl, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);

        // 根据选项卡控件的窗口矩形大小计算显示区域的大小，然后调整子窗口的大小
        SetRect(&rect, 0, 0, LOWORD(lParam), HIWORD(lParam));
        SendMessage(hwndTabControl, TCM_ADJUSTRECT, FALSE, (LPARAM)&rect);
        for (int i = 0; i < 4; i++)
            MoveWindow(hwndChild[i],    // 注意MoveWindow函数的位置和大小参数的符号
                rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        return 0;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case TCN_SELCHANGE:
            nIndex = SendMessage(hwndTabControl, TCM_GETCURSEL, 0, 0);
            for (int i = 0; i < 4; i++)
                ShowWindow(hwndChild[i], SW_HIDE);
            ShowWindow(hwndChild[nIndex], SW_SHOW);
            break;

        case TTN_NEEDTEXT:
            LPNMTTDISPINFO lpnmTDI;
            lpnmTDI = (LPNMTTDISPINFO)lParam;
            switch (lpnmTDI->hdr.idFrom)
            {
            case 0:
                StringCchCopy(lpnmTDI->szText, 80, TEXT("这是平面设计的工具提示文本"));
                break;
            case 1:
                StringCchCopy(lpnmTDI->szText, 80, TEXT("这是Web开发的工具提示文本"));
                break;
            case 2:
                StringCchCopy(lpnmTDI->szText, 80, TEXT("这是Windows程序设计的工具提示文本"));
                break;
            case 3:
                StringCchCopy(lpnmTDI->szText, 80, TEXT("这是加密解密的工具提示文本"));
                break;
            }
            break;
        }
        return 0;

    case WM_DESTROY:
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProcChild(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        if (hwnd == hwndChild[0])
            DrawText(hdc, TEXT("平面设计 价格1万"), _tcslen(TEXT("平面设计 价格1万")),
                &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        else if (hwnd == hwndChild[1])
            DrawText(hdc, TEXT("Web开发 价格2万"), _tcslen(TEXT("Web开发 价格2万")),
                &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        else if (hwnd == hwndChild[2])
            DrawText(hdc, TEXT("Windows程序设计 价格3万"), _tcslen(TEXT("Windows程序设计 价格3万")),
                &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        else
            DrawText(hdc, TEXT("加密解密 价格4万"), _tcslen(TEXT("加密解密 价格4万")),
                &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}