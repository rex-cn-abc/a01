#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

// ȫ�ֱ���
HINSTANCE g_hInstance;

// �������������ڹ���
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
    TCHAR szBuf[64] = { 0 };

    static HWND hwndStatus;
    UINT uArrIDs[] = { 0, ID_FILE };

    switch (uMsg)
    {
    case WM_CREATE:
        hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU));
        SetMenu(hwnd, hMenu);

        // ����״̬��
        hwndStatus = CreateWindowEx(0, TEXT("msctls_statusbar32"), NULL,
            WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, hwnd, (HMENU)IDC_STATUSBAR, g_hInstance, NULL);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:   // �½�
            wsprintf(szBuf, TEXT("������� �½� �˵������ID��%d\n"), ID_FILE_NEW);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
            break;
        case ID_EDIT_CUT:   // ����
            wsprintf(szBuf, TEXT("������� ���� �˵������ID��%d\n"), ID_EDIT_CUT);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
            break;
        case ID_HELP_ABOUT: // ����HelloWindows
            wsprintf(szBuf, TEXT("������� ����HelloWindows �˵������ID��%d\n"), ID_HELP_ABOUT);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
            break;
        case ID_FILE_EXIT:  // �˳�
            wsprintf(szBuf, TEXT("������� �˳� �˵������ID��%d\n"), ID_FILE_EXIT);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        return 0;

    case WM_SYSCOMMAND:
        switch (wParam & 0xFFF0)
        {
        case SC_CLOSE:
            MessageBox(hwnd, TEXT("������� ϵͳ�˵� �ر� �˵���"), TEXT("��ʾ"), MB_OK);
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return 0;

    case WM_MENUSELECT:
        MenuHelp(uMsg, wParam, lParam, (HMENU)lParam, g_hInstance, hwndStatus, uArrIDs);
        return 0;

    case WM_SIZE:
        SendMessage(hwndStatus, WM_SIZE, 0, 0);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}