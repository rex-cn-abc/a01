#include <Windows.h>
#include <WindowsX.h>
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
    TCHAR szAppName[] = TEXT("TrackTool");
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
    static HWND hwndTip;
    static BOOL bTracking = FALSE;
    static int oldX, oldY;
    int newX, newY;
    static TOOLINFO ti = { sizeof(TOOLINFO) };
    TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
    POINT pt;
    TCHAR szBuf[24] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 创建工具提示控件
        hwndTip = CreateWindowEx(WS_EX_TOPMOST, TEXT("tooltips_class32"), NULL,
            WS_POPUP | TTS_ALWAYSTIP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            hwnd, NULL, hInstance, NULL);

        // 添加跟踪工具，客户区域
        ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
        ti.hwnd = hwnd;
        ti.uId = 1001;
        GetClientRect(hwnd, &ti.rect);
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);
        return 0;

    case WM_MOUSEMOVE:
        // 程序可以通过调用TrackMouseEvent函数让系统发送另外两条消息：
        // 当鼠标光标悬停在客户区一段时间后发送WM_MOUSEHOVER消息，
        // 当光标离开客户区时发送WM_MOUSELEAVE消息。
        if (!bTracking)
        {
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);

            bTracking = TRUE;
        }

        // 激活跟踪工具提示
        SendMessage(hwndTip, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&ti);

        newX = GET_X_LPARAM(lParam);
        newY = GET_Y_LPARAM(lParam);
        if ((newX != oldX) || (newY != oldY))
        {
            oldX = newX;
            oldY = newY;

            // 更改“工具”的提示文本
            wsprintf(szBuf, TEXT("鼠标的客户区坐标：%d, %d"), newX, newY);
            ti.lpszText = szBuf;
            SendMessage(hwndTip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);

            // 移动跟踪工具提示的位置
            pt = { newX, newY };
            ClientToScreen(hwnd, &pt);
            SendMessage(hwndTip, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
        }
        return 0;

    case WM_MOUSELEAVE:
        // 停用跟踪工具提示
        SendMessage(hwndTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);
        bTracking = FALSE;
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}