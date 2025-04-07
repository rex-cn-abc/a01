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

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

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
    HMENU hMenu;
    WORD arrMenuTemplate[] = {
        // MENUITEMTEMPLATEHEADER结构
        0,
        0,

        // 多个MENUITEMTEMPLATE结构，一行一个
        // 第1个弹出菜单, 最后一个子菜单项需要MF_HILITE标志
        MF_STRING | MF_POPUP, L'编', L'辑', 0,
        MF_STRING, 40030, L'剪', L'切', L'(', L'&', L'T', L')', L'\t', L'C', L't', L'r',
            L'l', L'+', L'X', 0,
        MF_STRING, 40031, L'复', L'制', L'(', L'&', L'C', L')', L'\t', L'C', L't', L'r',
            L'l', L'+', L'C', 0,
        MF_STRING, 40032, L'粘', L'贴', L'(', L'&', L'P', L')', L'\t', L'C', L't', L'r',
            L'l', L'+', L'V', 0,
        MF_SEPARATOR, 0, 0,
        MF_STRING, 40033, L'红', L'色', L'(', L'&', L'R', L')', 0,
        MF_STRING, 40034, L'绿', L'色', L'(', L'&', L'G', L')', 0,
        MF_STRING | MF_HILITE, 40035, L'蓝', L'色', L'(', L'&', L'B', L')', 0,

        // 第2个弹出菜单, 最后一个子菜单项需要MF_HILITE标志
        MF_STRING | MF_POPUP, L'视', L'图', 0,
        MF_STRING, 40036, L'大', L'图', L'标', L'(', L'&', L'D', L')', 0,
        MF_STRING | MF_HILITE, 40037, L'小', L'图', L'标', L'(', L'&', L'S', L')', 0,

        // 第3个弹出菜单, 开始和结束都需要MF_HILITE标志，说明这是最后一个菜单
        MF_STRING | MF_POPUP | MF_HELP | MF_HILITE, L'帮', L'助', 0,
        MF_STRING | MF_HILITE, 40038, L'关', L'于', L'H', L'e', L'l', L'l', L'o', L'W', 
            L'i', L'n', L'd', L'o', L'w', L's', L'(', L'&', L'A', L')', 0
    };

    switch (uMsg)
    {
    case WM_CREATE:
        hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU));
        SetMenu(hwnd, hMenu);
        return 0;

    case WM_CHAR:
        // 从内存中动态加载菜单资源(菜单模板)
        hMenu = LoadMenuIndirect(arrMenuTemplate);
        SetMenu(hwnd, hMenu);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 40030:
            MessageBox(hwnd, TEXT("按下了 剪切"), TEXT("提示"), MB_OK);
            break;
        case 40036:
            MessageBox(hwnd, TEXT("按下了 大图标"), TEXT("提示"), MB_OK);
            break;
        case 40038:
            MessageBox(hwnd, TEXT("按下了 关于HelloWindows"), TEXT("提示"), MB_OK);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}