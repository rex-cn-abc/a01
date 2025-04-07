#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("SysLinkDemo");
    HWND hwnd;
    MSG msg;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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
    static HFONT hFont;
    static HWND hwndSysLink;

    LITEM li = { 0 };
    PNMLINK pnmLink;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        hwndSysLink = CreateWindowEx(0, TEXT("SysLink"),
          TEXT("我喜欢<a href=\"http://www.WindowsChs.com/\" ID=\"Windows\" >Windows程序设计</a>\n")
          TEXT("我喜欢<a href=\"http://www.TaoBao.com/\" ID=\"TaoBao\" >淘宝购物</a>\n")
          TEXT("我喜欢<a href=\"http://www.360Buy.com/\" ID=\"360Buy\" >京东商城</a>"),
          WS_CHILD | WS_VISIBLE | WS_TABSTOP | LWS_TRANSPARENT,
          10, 10, 200, 60, hwnd, (HMENU)(1001), hInstance, NULL);
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndSysLink, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_LBUTTONDBLCLK:
        li.mask = LIF_ITEMINDEX | LIF_URL;
        li.iLink = 0;
        StringCchCopy(li.szUrl, L_MAX_URL_LENGTH, TEXT("https://msdn.microsoft.com/"));
        SendMessage(hwndSysLink, LM_SETITEM, 0, (LPARAM)&li);
        return 0;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case NM_CLICK:
        case NM_RETURN:
            pnmLink = (PNMLINK)lParam;
            if (pnmLink->hdr.hwndFrom == hwndSysLink)
            {
                if (pnmLink->item.iLink == 0)
                    ShellExecute(NULL, TEXT("open"), pnmLink->item.szUrl, NULL, NULL, SW_SHOW);
                else if (pnmLink->item.iLink == 1)
                    ShellExecute(NULL, TEXT("open"), pnmLink->item.szUrl, NULL, NULL, SW_SHOW);
                else if (pnmLink->item.iLink == 2)
                    ShellExecute(NULL, TEXT("open"), pnmLink->item.szUrl, NULL, NULL, SW_SHOW);
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