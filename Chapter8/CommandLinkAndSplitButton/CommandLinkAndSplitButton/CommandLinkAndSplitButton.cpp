#include <Windows.h>
#include <Commctrl.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("CommandLinkAndSplitButton");    // 程序标题、窗口类名
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

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
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
    static HINSTANCE hInstance;
    NMBCDROPDOWN* pDropDown;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

        CreateWindowEx(0, TEXT("Button"), TEXT("命令链接按钮"),
            WS_CHILD | WS_VISIBLE | BS_COMMANDLINK, 10, 10, 200, 50, hwnd,
            (HMENU)IDC_COMMANDLINK, hInstance, NULL);
        SendDlgItemMessage(hwnd, 1000, BCM_SETNOTE, 0, (LPARAM)TEXT("这是一个命令链接按钮"));

        CreateWindowEx(0, TEXT("Button"), TEXT("拆分按钮"),
            WS_CHILD | WS_VISIBLE | BS_SPLITBUTTON, 10, 70, 200, 30, hwnd,
            (HMENU)IDC_SPLITBUTTON, hInstance, NULL);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_EDIT_CUT:
            MessageBox(hwnd, TEXT("ID_EDIT_CUT"), TEXT("提示"), MB_OK);
            break;
        case ID_EDIT_COPY:
            MessageBox(hwnd, TEXT("ID_EDIT_COPY"), TEXT("提示"), MB_OK);
            break;
        case ID_EDIT_PASTE:
            MessageBox(hwnd, TEXT("ID_EDIT_PASTE"), TEXT("提示"), MB_OK);
            break;

        case IDC_COMMANDLINK:
            MessageBox(hwnd, TEXT("点击了命令链接按钮"), TEXT("提示"), MB_OK);
            break;
        case IDC_SPLITBUTTON:
            MessageBox(hwnd, TEXT("点击了拆分按钮"), TEXT("提示"), MB_OK);
            break;
        }
        return 0;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case BCN_DROPDOWN:
            pDropDown = (NMBCDROPDOWN*)lParam;
            if (pDropDown->hdr.hwndFrom = GetDlgItem(hwnd, IDC_SPLITBUTTON))
            {
                POINT pt;
                HMENU hMenu;
                pt.x = pDropDown->rcButton.left;
                pt.y = pDropDown->rcButton.bottom;
                // 拆分按钮的矩形客户区坐标转换为屏幕坐标
                ClientToScreen(pDropDown->hdr.hwndFrom, &pt);
                hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
                TrackPopupMenu(GetSubMenu(hMenu, 0), TPM_LEFTALIGN | TPM_TOPALIGN, 
                    pt.x, pt.y, 0, hwnd, NULL);
            }
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}