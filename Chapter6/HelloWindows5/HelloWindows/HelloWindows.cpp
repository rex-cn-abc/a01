#include <Windows.h>
#include <tchar.h>
#include "resource.h"

// ȫ�ֱ���
HINSTANCE g_hInstance;
UINT g_uLanguage;

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID ShowMenu(HWND hwnd, UINT uLanguage);

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
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FEATHER));
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, hMenu, hInstance, NULL);

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
    switch (uMsg)
    {
    case WM_CREATE:
        g_uLanguage = 10000;
        return 0;

    case WM_COMMAND:
        // �˴�û��ʵ�ֶ�����
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:
            MessageBox(hwnd, TEXT("������ �½�"), TEXT("��ʾ"), MB_OK);
            break;
        case ID_EDIT_CUT:
            MessageBox(hwnd, TEXT("������ ����"), TEXT("��ʾ"), MB_OK);
            break;
        case ID_HELP_ABOUT:
            MessageBox(hwnd, TEXT("������ ����HelloWindows"), TEXT("��ʾ"), MB_OK);
            break;
        case ID_FILE_EXIT:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case ID_LANGUAGE_CHINESE:
            g_uLanguage = 10000;
            ShowMenu(hwnd, g_uLanguage);
            break;
        case ID_LANGUAGE_ENGLISH:
            g_uLanguage = 20000;
            ShowMenu(hwnd, g_uLanguage);
            break;
        }
        return 0;

    case WM_INITMENUPOPUP:
        if (g_uLanguage == 10000)
            CheckMenuRadioItem(GetSubMenu(GetMenu(hwnd), 2), ID_LANGUAGE_CHINESE,
                ID_LANGUAGE_ENGLISH, ID_LANGUAGE_CHINESE, MF_BYCOMMAND);
        else
            CheckMenuRadioItem(GetSubMenu(GetMenu(hwnd), 2), ID_LANGUAGE_CHINESE,
                ID_LANGUAGE_ENGLISH, ID_LANGUAGE_ENGLISH, MF_BYCOMMAND);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

VOID ShowMenu(HWND hwnd, UINT uLanguage)
{
    HMENU hMenu, hMenuPopup, hMenuTemp, hMenuPopupTemp;
    TCHAR szBuf[256] = { 0 };
    UINT uID;

    hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU));   // �˵���Դ�����˵�
    hMenuTemp = CreateMenu();                                   // ���˵�
    for (int i = 0; i < GetMenuItemCount(hMenu); i++)
    {
        hMenuPopup = GetSubMenu(hMenu, i);                      // �˵���Դ��ÿ�������˵�
        hMenuPopupTemp = CreateMenu();                          // ÿ�������˵�
        for (int j = 0; j < GetMenuItemCount(hMenuPopup); j++)
        {
            uID = GetMenuItemID(hMenuPopup, j);
            GetMenuString(hMenuPopup, j, szBuf, _countof(szBuf), MF_BYPOSITION);
            // �ж��ǲ��Ƿָ���
            if (_tcslen(szBuf) != 0)
            {
                // �Ӳ˵����ID��40010��ʼ���Ӳ˵����ַ�����ID���ǣ�uID - 40000 + uLanguage
                LoadString(g_hInstance, uID - 40000 + uLanguage, szBuf, _countof(szBuf));
                AppendMenu(hMenuPopupTemp, MF_STRING, uID, szBuf);
            }
            else
            {
                AppendMenu(hMenuPopupTemp, MF_SEPARATOR, 0, NULL);
            }
        }
        // ÿ�������˵���ӵ����˵�
        LoadString(g_hInstance, uLanguage + i, szBuf, _countof(szBuf));
        AppendMenu(hMenuTemp, MF_STRING | MF_POPUP, (UINT_PTR)hMenuPopupTemp, szBuf);
    }

    SetMenu(hwnd, hMenuTemp);
    DrawMenuBar(hwnd);
}