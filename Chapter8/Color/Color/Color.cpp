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
    TCHAR szAppName[] = TEXT("取色调色器V1.0");
    HWND hwnd;
    MSG msg;

    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WindowProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PANDA));;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | 
        WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 360, 220, NULL, NULL, hInstance, NULL);

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
    static HWND hwndBtnStart, hwndBtnStop;  // 开始、停止按钮
    static HWND hwndSBHorz[3];              // 3个水平滚动条
    static HWND hwndStaticColor[3];         // 3个静态控件用于实时显示红、绿、蓝颜色值
    static HWND hwndStatic[3];              // 3个静态控件：网页颜色、RGB颜色、点击开始按钮开始取色
    static HWND hwndEdit[2];                // 2个编辑控件：网页颜色、RGB颜色
    static HFONT hFont;

    COLORREF crPrim[3] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
    static HBRUSH  hBrush[3];
    static int color[3];                    // 红、绿、蓝颜色值
    static RECT rcColor;                    // 显示颜色结果的小矩形
    HDC hdc;
    PAINTSTRUCT ps;
    int n;
    TCHAR szBuf[24] = { 0 };

    static HDC hdcDesk, hdcMem;
    static HBITMAP hBitmapMem;
    static BOOL bStarting;                  // 开始取色
    POINT pt;                               // 鼠标光标位置
    COLORREF crPixel;                       // 鼠标光标位置处的COLORREF颜色值

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 开始、停止按钮
        hwndBtnStart = CreateWindowEx(0, TEXT("Button"), TEXT("开始"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 130, 50, 22, hwnd, (HMENU)IDC_BTNSTART, hInstance, NULL);
        hwndBtnStop = CreateWindowEx(0, TEXT("Button"), TEXT("停止"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            70, 130, 50, 22, hwnd, (HMENU)IDC_BTNSTOP, hInstance, NULL);

        // 3个水平滚动条
        hwndSBHorz[0] = CreateWindowEx(0, TEXT("ScrollBar"),
            NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
            130, 0, 200, 17, hwnd, (HMENU)IDC_SBHORZ1, hInstance, NULL);
        hwndSBHorz[1] = CreateWindowEx(0, TEXT("ScrollBar"),
            NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
            130, 20, 200, 17, hwnd, (HMENU)IDC_SBHORZ2, hInstance, NULL);
        hwndSBHorz[2] = CreateWindowEx(0, TEXT("ScrollBar"),
            NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
            130, 40, 200, 17, hwnd, (HMENU)IDC_SBHORZ3, hInstance, NULL);

        // 3个静态控件用于实时显示红、绿、蓝颜色值
        hwndStaticColor[0] = CreateWindowEx(0, TEXT("Static"), TEXT("红：0"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            130, 60, 60, 20, hwnd, (HMENU)IDC_STATICCOLOR1, hInstance, NULL);
        hwndStaticColor[1] = CreateWindowEx(0, TEXT("Static"), TEXT("绿：0"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            200, 60, 60, 20, hwnd, (HMENU)IDC_STATICCOLOR2, hInstance, NULL);
        hwndStaticColor[2] = CreateWindowEx(0, TEXT("Static"), TEXT("蓝：0"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            270, 60, 60, 20, hwnd, (HMENU)IDC_STATICCOLOR3, hInstance, NULL);

        // 2个静态控件，网页颜色和RGB颜色，以及2个编辑控件
        hwndStatic[0] = CreateWindowEx(0, TEXT("Static"), TEXT("网页颜色："),
            WS_CHILD | WS_VISIBLE,
            130, 85, 70, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndStatic[1] = CreateWindowEx(0, TEXT("Static"), TEXT("RGB颜色："),
            WS_CHILD | WS_VISIBLE,
            130, 110, 70, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndEdit[0] = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL,
            200, 85, 130, 22, hwnd, (HMENU)IDC_EDITWEB, hInstance, NULL);
        hwndEdit[1] = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL,
            200, 110, 130, 22, hwnd, (HMENU)IDC_EDITRGB, hInstance, NULL);
        // 静态控件：点击开始按钮开始取色
        hwndStatic[2] = CreateWindowEx(0, TEXT("Static"), TEXT("点击开始按钮开始取色"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            180, 145, 150, 20, hwnd, (HMENU)(-1), hInstance, NULL);

        // 设置字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndBtnStart, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndBtnStop, WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndStaticColor); i++)
            SendMessage(hwndStaticColor[i], WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndStatic); i++)
            SendMessage(hwndStatic[i], WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndEdit); i++)
            SendMessage(hwndEdit[i], WM_SETFONT, (WPARAM)hFont, FALSE);

        // 初始化网页颜色、RGB颜色编辑框的内容
        wsprintf(szBuf, TEXT("#%0.2X%0.2X%0.2X"), color[0], color[1], color[2]);
        SetDlgItemText(hwnd, IDC_EDITWEB, szBuf);
        wsprintf(szBuf, TEXT("RGB(%d, %d, %d)"), color[0], color[1], color[2]);
        SetDlgItemText(hwnd, IDC_EDITRGB, szBuf);

        // 创建红绿蓝3个画刷，用于水平滚动条背景和3个实时显示红绿蓝颜色值静态控件的文本颜色
        for (int i = 0; i < _countof(hBrush); i++)
            hBrush[i] = CreateSolidBrush(crPrim[i]);

        // 设置水平滚动条的范围和初始位置
        for (int i = 0; i < _countof(hwndSBHorz); i++)
        {
            SetScrollRange(hwndSBHorz[i], SB_CTL, 0, 255, FALSE);
            SetScrollPos(hwndSBHorz[i], SB_CTL, 0, FALSE);
        }

        // 显示颜色结果的那个小矩形的坐标，调用InvalidateRect函数宣布该区域无效的时候会用到
        SetRect(&rcColor, 130, 135, 170, 175);

        // 桌面设备环境句柄，内存设备环境句柄
        hdcDesk = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        hdcMem = CreateCompatibleDC(hdcDesk);
        hBitmapMem = CreateCompatibleBitmap(hdcDesk, 120, 120);
        SelectObject(hdcMem, hBitmapMem);

        // 在系统菜单中添加一个关于本程序菜单项
        AppendMenu(GetSystemMenu(hwnd, FALSE), MF_STRING, IDM_ABOUT, TEXT("关于本程序"));

        // 为开始按钮设置键盘焦点
        SetFocus(hwndBtnStart);
        return 0;

    case WM_CTLCOLORSCROLLBAR:
        n = GetWindowLongPtr((HWND)lParam, GWLP_ID) - IDC_SBHORZ1;
        return (LRESULT)hBrush[n];

    case WM_CTLCOLORSTATIC:
        n = GetWindowLongPtr((HWND)lParam, GWLP_ID) - IDC_STATICCOLOR1;
        if (n >= 0 && n <= 2)
            SetTextColor((HDC)wParam, crPrim[n]);
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_HSCROLL:
        n = GetWindowLongPtr((HWND)lParam, GWLP_ID) - IDC_SBHORZ1;
        SetFocus((HWND)lParam);    // 设置键盘焦点以后才可以使用键盘
        switch (LOWORD(wParam))
        {
        case SB_LINELEFT:
            color[n] -= 1;
            break;
        case SB_LINERIGHT:
            color[n] += 1;
            break;
        case SB_PAGELEFT:
            color[n] -= 10;
            break;
        case SB_PAGERIGHT:
            color[n] += 10;
            break;
        case SB_THUMBTRACK:
            color[n] = HIWORD(wParam);
            break;
        case SB_LEFT:
            color[n] = 0;
            break;
        case SB_RIGHT:
            color[n] = 255;
            break;
        }
        color[n] = min(color[n], 255);
        color[n] = max(0, color[n]);
        if (color[n] != GetScrollPos((HWND)lParam, SB_CTL))
        {
            SetScrollPos((HWND)lParam, SB_CTL, color[n], TRUE);
            // 实时显示颜色值的静态控件
            SetDlgItemInt(hwnd, IDC_STATICCOLOR1 + n, color[n], FALSE);
            // 网页颜色、RGB颜色编辑控件
            wsprintf(szBuf, TEXT("#%0.2X%0.2X%0.2X"), color[0], color[1], color[2]);
            SetDlgItemText(hwnd, IDC_EDITWEB, szBuf);
            wsprintf(szBuf, TEXT("RGB(%d, %d, %d)"), color[0], color[1], color[2]);
            SetDlgItemText(hwnd, IDC_EDITRGB, szBuf);
            // 重绘颜色结果小矩形
            InvalidateRect(hwnd, &rcColor, FALSE);
        }
        return 0;

    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
            if (LOWORD(wParam) == IDC_BTNSTART)
            {
                bStarting = TRUE;
                SetFocus(hwndBtnStop);
                SetTimer(hwnd, 1, 100, NULL);
                SetTimer(hwnd, 2, 1000, NULL);
                SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                // 把停止按钮改为默认按钮样式，没什么意义，仅仅是提供一种视觉效果
                // 如果是在对话框程序中，当没有其他按钮具有键盘焦点时，用户可以通过按下Enter键选择默认按钮
                SendMessage(hwndBtnStop, BM_SETSTYLE, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, FALSE);
                SetWindowText(hwndStatic[2], TEXT("请按空格键停止取色"));
            }
            else if (LOWORD(wParam) == IDC_BTNSTOP)
            {
                bStarting = FALSE;
                SetFocus(hwndBtnStart);
                KillTimer(hwnd, 1);
                KillTimer(hwnd, 2);
                SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                SendMessage(hwndBtnStop, BM_SETSTYLE, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, FALSE);
                SetWindowText(hwndStatic[2], TEXT("点击开始按钮开始取色"));
            }
            break;

        case EN_SETFOCUS:
            if (LOWORD(wParam) == IDC_EDITWEB)
            {
                SendMessage(hwndEdit[1], EM_SETSEL, -1, -1);
                SendMessage(hwndEdit[0], EM_SETSEL, 0, -1);
                SendMessage(hwndEdit[0], WM_COPY, 0, 0);
                MessageBox(hwnd, TEXT("网页颜色已经复制到剪贴板"), TEXT("提示"), MB_OK);
            }
            else if (LOWORD(wParam) == IDC_EDITRGB)
            {
                SendMessage(hwndEdit[0], EM_SETSEL, -1, -1);
                SendMessage(hwndEdit[1], EM_SETSEL, 0, -1);
                SendMessage(hwndEdit[1], WM_COPY, 0, 0);
                MessageBox(hwnd, TEXT("RGB颜色已经复制到剪贴板"), TEXT("提示"), MB_OK);
            }
            return 0;
        }

        return 0;

    case WM_SYSCOMMAND:
        if (LOWORD(wParam) == IDM_ABOUT)
        {
            MessageBox(hwnd, TEXT("本程序由\n《十年磨一剑：Windows程序设计、加密解密从零基础到行家》作者提供\n\n老王，不负所望!"), TEXT("关于本程序"), MB_OK);
            return 0;
        }
        break;

    case WM_TIMER:
        if (wParam == 1)
        {
            // 获取鼠标光标处的颜色值，然后更新水平滚动条、颜色值静态控件、两个编辑框、颜色结果矩形
            GetCursorPos(&pt);
            crPixel = GetPixel(hdcDesk, pt.x, pt.y);
            color[0] = GetRValue(crPixel);
            color[1] = GetGValue(crPixel);
            color[2] = GetBValue(crPixel);

            SetScrollPos(hwndSBHorz[0], SB_CTL, color[0], TRUE);
            SetScrollPos(hwndSBHorz[1], SB_CTL, color[1], TRUE);
            SetScrollPos(hwndSBHorz[2], SB_CTL, color[2], TRUE);

            SetDlgItemInt(hwnd, IDC_STATICCOLOR1, color[0], FALSE);
            SetDlgItemInt(hwnd, IDC_STATICCOLOR2, color[1], FALSE);
            SetDlgItemInt(hwnd, IDC_STATICCOLOR3, color[2], FALSE);

            wsprintf(szBuf, TEXT("#%0.2X%0.2X%0.2X"), color[0], color[1], color[2]);
            SetDlgItemText(hwnd, IDC_EDITWEB, szBuf);
            wsprintf(szBuf, TEXT("RGB(%d, %d, %d)"), color[0], color[1], color[2]);
            SetDlgItemText(hwnd, IDC_EDITRGB, szBuf);

            InvalidateRect(hwnd, &rcColor, FALSE);

            // 实时显示鼠标光标附近的图像
            hdc = GetDC(hwnd);
            SelectObject(hdcMem, GetStockObject(NULL_PEN));
            Rectangle(hdcMem, 0, 0, 121, 121);
            // 取鼠标光标周围的40*40像素图像，放大3倍
            StretchBlt(hdcMem, 0, 0, 120, 120, hdcDesk, pt.x - 20, pt.y - 20, 40, 40, SRCCOPY);
            DrawIcon(hdcMem, 60 - GetSystemMetrics(SM_CXICON) / 2,
                60 - GetSystemMetrics(SM_CYICON) / 2, LoadCursor(NULL, IDC_CROSS));
            BitBlt(hdc, 0, 0, 120, 120, hdcMem, 0, 0, SRCCOPY);
            ReleaseDC(hwnd, hdc);
        }
        else
        {
            // 之所以1秒调用一次本计时器，而不是100ms
            // 是为了防止其他程序根本得不到键盘焦点，影响用户体验
            // GetActiveWindow() != hwnd或GetForegroundWindow() != hwnd也可以
            if (GetFocus() != hwndBtnStop)
            {
                RECT rect;
                // 保存鼠标光标位置，然后把鼠标光标位置设置到客户区(10, 10)，然后点击
                GetCursorPos(&pt);
                GetClientRect(hwnd, &rect);
                ClientToScreen(hwnd, (LPPOINT)&rect);
                SetCursorPos(rect.left + 10, rect.top + 10);
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                // 恢复鼠标光标位置
                SetCursorPos(pt.x, pt.y);
            }
        }
        return 0;

    case WM_SETFOCUS:
        if (bStarting)
            SetFocus(hwndBtnStop);
        /*else
            SetFocus(hwndBtnStart);*/
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // 颜色结果矩形
        SelectObject(hdc, CreateSolidBrush(RGB(color[0], color[1], color[2])));
        Rectangle(hdc, rcColor.left, rcColor.top, rcColor.right, rcColor.bottom);
        DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
        // 鼠标光标周围的图像
        BitBlt(hdc, 1, 0, 121, 120, hdcMem, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        DeleteObject(hFont);
        for (int i = 0; i < _countof(hBrush); i++)
            DeleteObject(hBrush[i]);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}