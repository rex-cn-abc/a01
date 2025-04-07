#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("HotKeyDemo");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 290, 150, NULL, NULL, hInstance, NULL);

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
    static HWND hwndHotKeyHwnd, hwndBtnSetHwnd;     // 与窗口激活热键相关的热键控件和按钮句柄
    static HWND hwndHotKeyThread, hwndBtnSetThread; // 与特定线程热键相关的热键控件和按钮句柄
    static HFONT hFont;
    DWORD dwHotKey;         // 发送HKM_GETHOTKEY消息获取热键控件的修饰键和伴随键的返回值
    DWORD dwRet;            // 发送WM_SETHOTKEY消息设置窗口激活热键的返回值
    UINT fsModifiers = 0;   // RegisterHotKey函数的fsModifiers参数，修饰键标志

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 2个热键控件
        hwndHotKeyHwnd = CreateWindowEx(0, TEXT("msctls_hotkey32"), NULL,
            WS_CHILD | WS_VISIBLE,
            10, 10, 120, 22, hwnd, (HMENU)IDC_HOTKEYHWND, hInstance, NULL);
        hwndHotKeyThread = CreateWindowEx(0, TEXT("msctls_hotkey32"), NULL,
            WS_CHILD | WS_VISIBLE,
            140, 10, 120, 22, hwnd, (HMENU)IDC_HOTKEYTHREAD, hInstance, NULL);

        // 2个按钮
        hwndBtnSetHwnd = CreateWindowEx(0, TEXT("Button"), TEXT("设置窗口激活热键"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 40, 120, 25, hwnd, (HMENU)IDC_BTNSETHWND, hInstance, NULL);
        hwndBtnSetThread = CreateWindowEx(0, TEXT("Button"), TEXT("注册特定线程热键"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            140, 40, 120, 25, hwnd, (HMENU)IDC_BTNSETTHREAD, hInstance, NULL);

        // 设置字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndHotKeyHwnd, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndHotKeyThread, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndBtnSetHwnd, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndBtnSetThread, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_SETFOCUS:
        SetFocus(hwndHotKeyHwnd);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTNSETHWND:
            dwHotKey = SendMessage(hwndHotKeyHwnd, HKM_GETHOTKEY, 0, 0);
            if (!HIBYTE(LOWORD(dwHotKey)) || !LOBYTE(LOWORD(dwHotKey)))
            {
                MessageBox(hwnd, TEXT("设置窗口激活热键需要修饰键和伴随键"), TEXT("错误"), MB_OK);
                return 0;
            }
            // 设置窗口激活热键
            dwRet = SendMessage(hwnd, WM_SETHOTKEY, LOWORD(dwHotKey), 0);
            if (dwRet <= 0)
                MessageBox(hwnd, TEXT("窗口激活热键设置失败"), TEXT("错误"), MB_OK);
            else if (dwRet == 1)
                MessageBox(hwnd, TEXT("成功，没有其他窗口具有"), TEXT("成功"), MB_OK);
            else if (dwRet == 2)
                MessageBox(hwnd, TEXT("成功，但另一个窗口已具有"), TEXT("警告"), MB_OK);
            break;

        case  IDC_BTNSETTHREAD:
            dwHotKey = SendMessage(hwndHotKeyThread, HKM_GETHOTKEY, 0, 0);
            if (HIBYTE(LOWORD(dwHotKey)) & HOTKEYF_SHIFT)
                fsModifiers |= MOD_SHIFT;
            if (HIBYTE(LOWORD(dwHotKey)) & HOTKEYF_CONTROL)
                fsModifiers |= MOD_CONTROL;
            if (HIBYTE(LOWORD(dwHotKey)) & HOTKEYF_ALT)
                fsModifiers |= MOD_ALT;
            if (!fsModifiers || !LOBYTE(LOWORD(dwHotKey)))
            {
                MessageBox(hwnd, TEXT("注册特定线程热键需要修饰键和伴随键"), TEXT("错误"), MB_OK);
                return 0;
            }
            // 注册特定线程热键
            if (RegisterHotKey(hwnd, 1, fsModifiers, LOBYTE(LOWORD(dwHotKey))))
                MessageBox(hwnd, TEXT("注册特定线程热键成功"), TEXT("注册成功"), MB_OK);
            else
                MessageBox(hwnd, TEXT("注册特定线程热键失败"), TEXT("注册失败"), MB_OK);
            break;
        }
        return 0;

    case WM_SYSCOMMAND:
        // 如果该窗口处于活动状态则会收到WM_SYSCOMMAND消息
        // 实际编程中很少处理SC_HOTKEY的WM_SYSCOMMAND消息
        if ((wParam & 0xFFF0) == SC_HOTKEY)
        {
            MessageBox(hwnd, TEXT("窗口激活热键消息"), TEXT("消息"), MB_OK);
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);

    case WM_HOTKEY:
        // 处理特定线程热键消息
        if (wParam == 1)
            MessageBox(hwnd, TEXT("特定线程热键消息"), TEXT("消息"), MB_OK);
        return 0;

    case WM_DESTROY:
        UnregisterHotKey(hwnd, 1);
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}