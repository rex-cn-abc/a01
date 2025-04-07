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
    static HWND hwndHotKeyHwnd, hwndBtnSetHwnd;     // �봰�ڼ����ȼ���ص��ȼ��ؼ��Ͱ�ť���
    static HWND hwndHotKeyThread, hwndBtnSetThread; // ���ض��߳��ȼ���ص��ȼ��ؼ��Ͱ�ť���
    static HFONT hFont;
    DWORD dwHotKey;         // ����HKM_GETHOTKEY��Ϣ��ȡ�ȼ��ؼ������μ��Ͱ�����ķ���ֵ
    DWORD dwRet;            // ����WM_SETHOTKEY��Ϣ���ô��ڼ����ȼ��ķ���ֵ
    UINT fsModifiers = 0;   // RegisterHotKey������fsModifiers���������μ���־

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 2���ȼ��ؼ�
        hwndHotKeyHwnd = CreateWindowEx(0, TEXT("msctls_hotkey32"), NULL,
            WS_CHILD | WS_VISIBLE,
            10, 10, 120, 22, hwnd, (HMENU)IDC_HOTKEYHWND, hInstance, NULL);
        hwndHotKeyThread = CreateWindowEx(0, TEXT("msctls_hotkey32"), NULL,
            WS_CHILD | WS_VISIBLE,
            140, 10, 120, 22, hwnd, (HMENU)IDC_HOTKEYTHREAD, hInstance, NULL);

        // 2����ť
        hwndBtnSetHwnd = CreateWindowEx(0, TEXT("Button"), TEXT("���ô��ڼ����ȼ�"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 40, 120, 25, hwnd, (HMENU)IDC_BTNSETHWND, hInstance, NULL);
        hwndBtnSetThread = CreateWindowEx(0, TEXT("Button"), TEXT("ע���ض��߳��ȼ�"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            140, 40, 120, 25, hwnd, (HMENU)IDC_BTNSETTHREAD, hInstance, NULL);

        // ��������
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
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
                MessageBox(hwnd, TEXT("���ô��ڼ����ȼ���Ҫ���μ��Ͱ����"), TEXT("����"), MB_OK);
                return 0;
            }
            // ���ô��ڼ����ȼ�
            dwRet = SendMessage(hwnd, WM_SETHOTKEY, LOWORD(dwHotKey), 0);
            if (dwRet <= 0)
                MessageBox(hwnd, TEXT("���ڼ����ȼ�����ʧ��"), TEXT("����"), MB_OK);
            else if (dwRet == 1)
                MessageBox(hwnd, TEXT("�ɹ���û���������ھ���"), TEXT("�ɹ�"), MB_OK);
            else if (dwRet == 2)
                MessageBox(hwnd, TEXT("�ɹ�������һ�������Ѿ���"), TEXT("����"), MB_OK);
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
                MessageBox(hwnd, TEXT("ע���ض��߳��ȼ���Ҫ���μ��Ͱ����"), TEXT("����"), MB_OK);
                return 0;
            }
            // ע���ض��߳��ȼ�
            if (RegisterHotKey(hwnd, 1, fsModifiers, LOBYTE(LOWORD(dwHotKey))))
                MessageBox(hwnd, TEXT("ע���ض��߳��ȼ��ɹ�"), TEXT("ע��ɹ�"), MB_OK);
            else
                MessageBox(hwnd, TEXT("ע���ض��߳��ȼ�ʧ��"), TEXT("ע��ʧ��"), MB_OK);
            break;
        }
        return 0;

    case WM_SYSCOMMAND:
        // ����ô��ڴ��ڻ״̬����յ�WM_SYSCOMMAND��Ϣ
        // ʵ�ʱ���к��ٴ���SC_HOTKEY��WM_SYSCOMMAND��Ϣ
        if ((wParam & 0xFFF0) == SC_HOTKEY)
        {
            MessageBox(hwnd, TEXT("���ڼ����ȼ���Ϣ"), TEXT("��Ϣ"), MB_OK);
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);

    case WM_HOTKEY:
        // �����ض��߳��ȼ���Ϣ
        if (wParam == 1)
            MessageBox(hwnd, TEXT("�ض��߳��ȼ���Ϣ"), TEXT("��Ϣ"), MB_OK);
        return 0;

    case WM_DESTROY:
        UnregisterHotKey(hwnd, 1);
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}