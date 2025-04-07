#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("ȡɫ��ɫ��V1.0");
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
    static HWND hwndBtnStart, hwndBtnStop;  // ��ʼ��ֹͣ��ť
    static HWND hwndSBHorz[3];              // 3��ˮƽ������
    static HWND hwndStaticColor[3];         // 3����̬�ؼ�����ʵʱ��ʾ�졢�̡�����ɫֵ
    static HWND hwndStatic[3];              // 3����̬�ؼ�����ҳ��ɫ��RGB��ɫ�������ʼ��ť��ʼȡɫ
    static HWND hwndEdit[2];                // 2���༭�ؼ�����ҳ��ɫ��RGB��ɫ
    static HFONT hFont;

    COLORREF crPrim[3] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };
    static HBRUSH  hBrush[3];
    static int color[3];                    // �졢�̡�����ɫֵ
    static RECT rcColor;                    // ��ʾ��ɫ�����С����
    HDC hdc;
    PAINTSTRUCT ps;
    int n;
    TCHAR szBuf[24] = { 0 };

    static HDC hdcDesk, hdcMem;
    static HBITMAP hBitmapMem;
    static BOOL bStarting;                  // ��ʼȡɫ
    POINT pt;                               // �����λ��
    COLORREF crPixel;                       // �����λ�ô���COLORREF��ɫֵ

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // ��ʼ��ֹͣ��ť
        hwndBtnStart = CreateWindowEx(0, TEXT("Button"), TEXT("��ʼ"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 130, 50, 22, hwnd, (HMENU)IDC_BTNSTART, hInstance, NULL);
        hwndBtnStop = CreateWindowEx(0, TEXT("Button"), TEXT("ֹͣ"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            70, 130, 50, 22, hwnd, (HMENU)IDC_BTNSTOP, hInstance, NULL);

        // 3��ˮƽ������
        hwndSBHorz[0] = CreateWindowEx(0, TEXT("ScrollBar"),
            NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
            130, 0, 200, 17, hwnd, (HMENU)IDC_SBHORZ1, hInstance, NULL);
        hwndSBHorz[1] = CreateWindowEx(0, TEXT("ScrollBar"),
            NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
            130, 20, 200, 17, hwnd, (HMENU)IDC_SBHORZ2, hInstance, NULL);
        hwndSBHorz[2] = CreateWindowEx(0, TEXT("ScrollBar"),
            NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ,
            130, 40, 200, 17, hwnd, (HMENU)IDC_SBHORZ3, hInstance, NULL);

        // 3����̬�ؼ�����ʵʱ��ʾ�졢�̡�����ɫֵ
        hwndStaticColor[0] = CreateWindowEx(0, TEXT("Static"), TEXT("�죺0"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            130, 60, 60, 20, hwnd, (HMENU)IDC_STATICCOLOR1, hInstance, NULL);
        hwndStaticColor[1] = CreateWindowEx(0, TEXT("Static"), TEXT("�̣�0"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            200, 60, 60, 20, hwnd, (HMENU)IDC_STATICCOLOR2, hInstance, NULL);
        hwndStaticColor[2] = CreateWindowEx(0, TEXT("Static"), TEXT("����0"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            270, 60, 60, 20, hwnd, (HMENU)IDC_STATICCOLOR3, hInstance, NULL);

        // 2����̬�ؼ�����ҳ��ɫ��RGB��ɫ���Լ�2���༭�ؼ�
        hwndStatic[0] = CreateWindowEx(0, TEXT("Static"), TEXT("��ҳ��ɫ��"),
            WS_CHILD | WS_VISIBLE,
            130, 85, 70, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndStatic[1] = CreateWindowEx(0, TEXT("Static"), TEXT("RGB��ɫ��"),
            WS_CHILD | WS_VISIBLE,
            130, 110, 70, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndEdit[0] = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL,
            200, 85, 130, 22, hwnd, (HMENU)IDC_EDITWEB, hInstance, NULL);
        hwndEdit[1] = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL,
            200, 110, 130, 22, hwnd, (HMENU)IDC_EDITRGB, hInstance, NULL);
        // ��̬�ؼ��������ʼ��ť��ʼȡɫ
        hwndStatic[2] = CreateWindowEx(0, TEXT("Static"), TEXT("�����ʼ��ť��ʼȡɫ"),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            180, 145, 150, 20, hwnd, (HMENU)(-1), hInstance, NULL);

        // ��������
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
        SendMessage(hwndBtnStart, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndBtnStop, WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndStaticColor); i++)
            SendMessage(hwndStaticColor[i], WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndStatic); i++)
            SendMessage(hwndStatic[i], WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndEdit); i++)
            SendMessage(hwndEdit[i], WM_SETFONT, (WPARAM)hFont, FALSE);

        // ��ʼ����ҳ��ɫ��RGB��ɫ�༭�������
        wsprintf(szBuf, TEXT("#%0.2X%0.2X%0.2X"), color[0], color[1], color[2]);
        SetDlgItemText(hwnd, IDC_EDITWEB, szBuf);
        wsprintf(szBuf, TEXT("RGB(%d, %d, %d)"), color[0], color[1], color[2]);
        SetDlgItemText(hwnd, IDC_EDITRGB, szBuf);

        // ����������3����ˢ������ˮƽ������������3��ʵʱ��ʾ��������ɫֵ��̬�ؼ����ı���ɫ
        for (int i = 0; i < _countof(hBrush); i++)
            hBrush[i] = CreateSolidBrush(crPrim[i]);

        // ����ˮƽ�������ķ�Χ�ͳ�ʼλ��
        for (int i = 0; i < _countof(hwndSBHorz); i++)
        {
            SetScrollRange(hwndSBHorz[i], SB_CTL, 0, 255, FALSE);
            SetScrollPos(hwndSBHorz[i], SB_CTL, 0, FALSE);
        }

        // ��ʾ��ɫ������Ǹ�С���ε����꣬����InvalidateRect����������������Ч��ʱ����õ�
        SetRect(&rcColor, 130, 135, 170, 175);

        // �����豸����������ڴ��豸�������
        hdcDesk = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        hdcMem = CreateCompatibleDC(hdcDesk);
        hBitmapMem = CreateCompatibleBitmap(hdcDesk, 120, 120);
        SelectObject(hdcMem, hBitmapMem);

        // ��ϵͳ�˵������һ�����ڱ�����˵���
        AppendMenu(GetSystemMenu(hwnd, FALSE), MF_STRING, IDM_ABOUT, TEXT("���ڱ�����"));

        // Ϊ��ʼ��ť���ü��̽���
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
        SetFocus((HWND)lParam);    // ���ü��̽����Ժ�ſ���ʹ�ü���
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
            // ʵʱ��ʾ��ɫֵ�ľ�̬�ؼ�
            SetDlgItemInt(hwnd, IDC_STATICCOLOR1 + n, color[n], FALSE);
            // ��ҳ��ɫ��RGB��ɫ�༭�ؼ�
            wsprintf(szBuf, TEXT("#%0.2X%0.2X%0.2X"), color[0], color[1], color[2]);
            SetDlgItemText(hwnd, IDC_EDITWEB, szBuf);
            wsprintf(szBuf, TEXT("RGB(%d, %d, %d)"), color[0], color[1], color[2]);
            SetDlgItemText(hwnd, IDC_EDITRGB, szBuf);
            // �ػ���ɫ���С����
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
                // ��ֹͣ��ť��ΪĬ�ϰ�ť��ʽ��ûʲô���壬�������ṩһ���Ӿ�Ч��
                // ������ڶԻ�������У���û��������ť���м��̽���ʱ���û�����ͨ������Enter��ѡ��Ĭ�ϰ�ť
                SendMessage(hwndBtnStop, BM_SETSTYLE, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, FALSE);
                SetWindowText(hwndStatic[2], TEXT("�밴�ո��ֹͣȡɫ"));
            }
            else if (LOWORD(wParam) == IDC_BTNSTOP)
            {
                bStarting = FALSE;
                SetFocus(hwndBtnStart);
                KillTimer(hwnd, 1);
                KillTimer(hwnd, 2);
                SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                SendMessage(hwndBtnStop, BM_SETSTYLE, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, FALSE);
                SetWindowText(hwndStatic[2], TEXT("�����ʼ��ť��ʼȡɫ"));
            }
            break;

        case EN_SETFOCUS:
            if (LOWORD(wParam) == IDC_EDITWEB)
            {
                SendMessage(hwndEdit[1], EM_SETSEL, -1, -1);
                SendMessage(hwndEdit[0], EM_SETSEL, 0, -1);
                SendMessage(hwndEdit[0], WM_COPY, 0, 0);
                MessageBox(hwnd, TEXT("��ҳ��ɫ�Ѿ����Ƶ�������"), TEXT("��ʾ"), MB_OK);
            }
            else if (LOWORD(wParam) == IDC_EDITRGB)
            {
                SendMessage(hwndEdit[0], EM_SETSEL, -1, -1);
                SendMessage(hwndEdit[1], EM_SETSEL, 0, -1);
                SendMessage(hwndEdit[1], WM_COPY, 0, 0);
                MessageBox(hwnd, TEXT("RGB��ɫ�Ѿ����Ƶ�������"), TEXT("��ʾ"), MB_OK);
            }
            return 0;
        }

        return 0;

    case WM_SYSCOMMAND:
        if (LOWORD(wParam) == IDM_ABOUT)
        {
            MessageBox(hwnd, TEXT("��������\n��ʮ��ĥһ����Windows������ơ����ܽ��ܴ���������мҡ������ṩ\n\n��������������!"), TEXT("���ڱ�����"), MB_OK);
            return 0;
        }
        break;

    case WM_TIMER:
        if (wParam == 1)
        {
            // ��ȡ����괦����ɫֵ��Ȼ�����ˮƽ����������ɫֵ��̬�ؼ��������༭����ɫ�������
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

            // ʵʱ��ʾ����긽����ͼ��
            hdc = GetDC(hwnd);
            SelectObject(hdcMem, GetStockObject(NULL_PEN));
            Rectangle(hdcMem, 0, 0, 121, 121);
            // ȡ�������Χ��40*40����ͼ�񣬷Ŵ�3��
            StretchBlt(hdcMem, 0, 0, 120, 120, hdcDesk, pt.x - 20, pt.y - 20, 40, 40, SRCCOPY);
            DrawIcon(hdcMem, 60 - GetSystemMetrics(SM_CXICON) / 2,
                60 - GetSystemMetrics(SM_CYICON) / 2, LoadCursor(NULL, IDC_CROSS));
            BitBlt(hdc, 0, 0, 120, 120, hdcMem, 0, 0, SRCCOPY);
            ReleaseDC(hwnd, hdc);
        }
        else
        {
            // ֮����1�����һ�α���ʱ����������100ms
            // ��Ϊ�˷�ֹ������������ò������̽��㣬Ӱ���û�����
            // GetActiveWindow() != hwnd��GetForegroundWindow() != hwndҲ����
            if (GetFocus() != hwndBtnStop)
            {
                RECT rect;
                // ���������λ�ã�Ȼ��������λ�����õ��ͻ���(10, 10)��Ȼ����
                GetCursorPos(&pt);
                GetClientRect(hwnd, &rect);
                ClientToScreen(hwnd, (LPPOINT)&rect);
                SetCursorPos(rect.left + 10, rect.top + 10);
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                // �ָ������λ��
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
        // ��ɫ�������
        SelectObject(hdc, CreateSolidBrush(RGB(color[0], color[1], color[2])));
        Rectangle(hdc, rcColor.left, rcColor.top, rcColor.right, rcColor.bottom);
        DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
        // �������Χ��ͼ��
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