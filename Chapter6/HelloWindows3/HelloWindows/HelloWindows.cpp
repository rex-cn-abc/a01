#include <Windows.h>
#include <tchar.h>
#include "resource.h"

// 全局变量
HINSTANCE g_hInstance;

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("HelloWindows");
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

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, hMenu, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACC));
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
    LPMEASUREITEMSTRUCT lpMIS;
    LPDRAWITEMSTRUCT lpDIS;

    switch (uMsg)
    {
    case WM_MEASUREITEM:
        lpMIS = (LPMEASUREITEMSTRUCT)lParam;
        // 弹出菜单的宽度高度
        if (lpMIS->itemID == 65535)
        {
            lpMIS->itemWidth = 50;
            lpMIS->itemHeight = 30;
        }
        // 子菜单项的宽度高度
        else
        {
            lpMIS->itemWidth = 150;
            lpMIS->itemHeight = 30;
        }
        return TRUE;

    case WM_DRAWITEM:
        lpDIS = (LPDRAWITEMSTRUCT)lParam;

        if (lpDIS->itemState & ODS_SELECTED || lpDIS->itemState & ODS_HOTLIGHT)
        {
            FillRect(lpDIS->hDC, &(lpDIS->rcItem), GetSysColorBrush(COLOR_WINDOWTEXT));

            SetBkMode(lpDIS->hDC, TRANSPARENT);
            SetTextColor(lpDIS->hDC, RGB(255, 255, 255));
            DrawText(lpDIS->hDC, LPTSTR(lpDIS->itemData), _tcslen(LPTSTR(lpDIS->itemData)),
                &lpDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        else
        {
            FillRect(lpDIS->hDC, &(lpDIS->rcItem), GetSysColorBrush(COLOR_MENU));

            SetBkMode(lpDIS->hDC, TRANSPARENT);
            DrawText(lpDIS->hDC, LPTSTR(lpDIS->itemData), _tcslen(LPTSTR(lpDIS->itemData)),
                &lpDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        return TRUE;

    /*case WM_MENUCHAR:
        return 0;*/

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:
            MessageBox(hwnd, TEXT("点击了 新建 按钮"), TEXT("提示"), MB_OK);
            break;

        case ID_EDIT_CUT:
            MessageBox(hwnd, TEXT("点击了 剪切 按钮"), TEXT("提示"), MB_OK);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}