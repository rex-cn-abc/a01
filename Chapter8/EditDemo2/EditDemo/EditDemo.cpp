#include <Windows.h>
#include <Commctrl.h>

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("EditDemo");
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
    static HINSTANCE hInstance;
    static HWND hwndUserName, hwndPassword, hwndAge, hwndRegister, hwndTip;
    static HWND hwndStatic[4];
    static HFONT hFont;
    int cx, cy;
    RECT rect;
    TOOLINFO ti = { 0 };
    RECT rcClient;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));

        hwndStatic[0] = CreateWindowEx(0, TEXT("Static"), TEXT("��Աע��"),
            WS_CHILD | WS_VISIBLE | SS_CENTER, 130, 20, 80, 20, hwnd, (HMENU)(-1), hInstance, NULL);

        // �û���
        hwndStatic[1] = CreateWindowEx(0, TEXT("Static"), TEXT("�û�����"),
            WS_CHILD | WS_VISIBLE, 20, 50, 60, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndUserName = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            80, 50, 220, 20, hwnd, (HMENU)(1000), hInstance, NULL);

        // ����
        hwndStatic[2] = CreateWindowEx(0, TEXT("Static"), TEXT("��  �룺"),
            WS_CHILD | WS_VISIBLE, 20, 75, 60, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndPassword = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD,
            80, 75, 180, 20, hwnd, (HMENU)(1001), hInstance, NULL);

        // ����
        hwndStatic[3] = CreateWindowEx(0, TEXT("Static"), TEXT("��  �䣺"),
            WS_CHILD | WS_VISIBLE, 20, 100, 60, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndAge = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
            80, 100, 180, 20, hwnd, (HMENU)(1002), hInstance, NULL);

        // ע�ᰴť
        hwndRegister = CreateWindowEx(0, TEXT("Button"), TEXT("ע��"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY | WS_TABSTOP,
            85, 130, 150, 25, hwnd, (HMENU)(1003), hInstance, NULL);

        // �������пؼ�����
        SendMessage(hwndUserName, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndPassword, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndAge, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndRegister, WM_SETFONT, (WPARAM)hFont, FALSE);
        for (int i = 0; i < _countof(hwndStatic); i++)
            SendMessage(hwndStatic[i], WM_SETFONT, (WPARAM)hFont, FALSE);

        // �û��������롢����ֱ���������20��12��3���ַ�
        SendMessage(hwndUserName, EM_SETLIMITTEXT, 20, 0);
        SendMessage(hwndPassword, EM_SETLIMITTEXT, 12, 0);
        SendMessage(hwndAge, EM_SETLIMITTEXT, 3, 0);

        // ���ó��򴰿ڴ�С���ͻ���������С��СΪ320 * 175
        SetRect(&rect, 0, 0, 320, 175);
        AdjustWindowRectEx(&rect, GetWindowLongPtr(hwnd, GWL_STYLE),
            GetMenu(hwnd) != NULL, GetWindowLongPtr(hwnd, GWL_EXSTYLE));
        SetWindowPos(hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOZORDER | SWP_NOMOVE);

        //////////////////////////////////////////////////////////////////////////
        // ������ʾ�ؼ�
        hwndTip = CreateWindowEx(WS_EX_TOPMOST, TEXT("tooltips_class32"), NULL, 
            WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
            hwnd, NULL, hInstance, NULL);

        // ������ʾ�ؼ��ı����ͼ��
        SendMessage(hwndTip, TTM_SETTITLE,
            (WPARAM)LoadIcon(NULL, IDI_INFORMATION), (LPARAM)TEXT("��ʾ��Ϣ"));

        // ���ó�ʱʱ��
        SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_INITIAL, 100);
        SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 10000);

        // ��ӡ����ߡ�
        ZeroMemory(&ti, sizeof(ti));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;

        ti.uId = (UINT_PTR)hwndUserName;
        ti.lpszText = TEXT("�������û���������3���ַ�");
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

        ti.uId = (UINT_PTR)hwndPassword;
        ti.lpszText = TEXT("���������룬����3���ַ�");
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

        ti.uId = (UINT_PTR)hwndAge;
        ti.lpszText = TEXT("������0��120�������");
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);

        // �ͻ������½�50*50���صľ���������Ϊ�����ߡ�(�ͻ�������������С�������)
        GetClientRect(hwnd, &rcClient);
        SetRect(&rcClient, rcClient.right - 50, rcClient.bottom - 50, rcClient.right, rcClient.bottom);
        ZeroMemory(&ti, sizeof(ti));
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.rect = rcClient;
        ti.hwnd = hwnd;
        ti.uId = 1;
        ti.lpszText = TEXT("�ѳ��򴰿ڵĿͻ������½�50*50���صľ���������Ϊ�����ߡ�");
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);
        //////////////////////////////////////////////////////////////////////////
        return 0;

    case WM_LBUTTONDBLCLK:
        ZeroMemory(&ti, sizeof(ti));
        ti.cbSize = sizeof(TOOLINFO);
        GetClientRect(hwnd, &ti.rect);
        ti.hwnd = hwnd;
        SendMessage(hwndTip, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);
        return 0;

    case WM_SIZE:
        if (LOWORD(lParam) >= 320 && HIWORD(lParam) >= 175)
        {
            cx = (LOWORD(lParam) - 320) / 2;
            cy = (HIWORD(lParam) - 175) / 2;
            SetWindowPos(hwndStatic[0], NULL, 130 + cx, 20  + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndStatic[1], NULL, 20  + cx, 50  + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndStatic[2], NULL, 20  + cx, 75  + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndStatic[3], NULL, 20  + cx, 100 + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndUserName,  NULL, 80  + cx, 50  + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndPassword,  NULL, 80  + cx, 75  + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndAge,       NULL, 80  + cx, 100 + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
            SetWindowPos(hwndRegister,  NULL, 85  + cx, 130 + cy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }
        return 0;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            if (LOWORD(wParam) == 1003)
            {
                int nLen;
                LPTSTR lpUserName, lpPassword;
                TCHAR szBuf[64] = { 0 };
                nLen = SendMessage(hwndUserName, EM_LINELENGTH, 0, 0);
                if (nLen < 3)
                {
                    MessageBox(hwnd, TEXT("�û���������Ҫ3���ַ�"), TEXT("������ʾ"), MB_OK);
                    return 0;
                }
                lpUserName = new TCHAR[nLen + 1];
                ZeroMemory(lpUserName, (nLen + 1) * sizeof(TCHAR));
                lpUserName[0] = nLen + 1;
                SendMessage(hwndUserName, EM_GETLINE, 0, (LPARAM)lpUserName);

                nLen = SendMessage(hwndPassword, EM_LINELENGTH, 0, 0);
                if (nLen < 3)
                {
                    MessageBox(hwnd, TEXT("����������Ҫ3���ַ�"), TEXT("������ʾ"), MB_OK);
                    return 0;
                }
                lpPassword = new TCHAR[nLen + 1];
                ZeroMemory(lpPassword, (nLen + 1) * sizeof(TCHAR));
                lpPassword[0] = nLen + 1;
                SendMessage(hwndPassword, EM_GETLINE, 0, (LPARAM)lpPassword);

                wsprintf(szBuf, TEXT("�û�����%s\n��  �룺%s\n��  �䣺%d"), 
                    lpUserName, lpPassword, GetDlgItemInt(hwnd, 1002, NULL, FALSE));
                MessageBox(hwnd, szBuf, TEXT("ע����Ϣ"), MB_OK);

                delete[] lpUserName;
                delete[] lpPassword;
            }
        }
        return 0;

    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_DESTROY:
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}