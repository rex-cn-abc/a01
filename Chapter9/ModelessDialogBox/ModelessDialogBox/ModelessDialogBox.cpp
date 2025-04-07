#include <Windows.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 全局变量
HWND g_hwndDlgModeless;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("ModelessDialogBox");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACC));
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (g_hwndDlgModeless == NULL || !IsDialogMessage(g_hwndDlgModeless, &msg))
        {
            if (!TranslateAccelerator(hwnd, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance;
    static HMENU hMenu;
    TCHAR szText[64] = { 0 };
    TCHAR szBuf[128] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
        SetMenu(hwnd, hMenu);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:
        case ID_FILE_OPEN:
        case ID_FILE_SAVE:
        case ID_FILE_SAVEAS:
        case ID_FILE_EXIT:
        case ID_EDIT_CUT:
        case ID_EDIT_COPY:
        case ID_EDIT_PASTE:
            GetMenuString(hMenu, LOWORD(wParam), szText, _countof(szText), MF_BYCOMMAND);
            wsprintf(szBuf, TEXT("您点击了 %s 菜单项，命令ID：%d\n"), szText, LOWORD(wParam));
            MessageBox(hwnd, szBuf, TEXT("提示"), MB_OK);
            if (LOWORD(wParam) == ID_FILE_EXIT)
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        case ID_HELP_ABOUT:
            // 创建非模态对话框，判断句柄是否为NULL以免创建多个
            //if (g_hwndDlgModeless == NULL)    // 或者
            if (!IsWindow(g_hwndDlgModeless))
                g_hwndDlgModeless = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG),
                    hwnd, DialogProc, NULL);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 对话框窗口过程
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            DestroyWindow(hwndDlg);
            g_hwndDlgModeless = NULL;
            break;
        }
        return TRUE;
    }

    return FALSE;
}