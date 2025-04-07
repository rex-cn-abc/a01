#include <Windows.h>
#include <tchar.h>
#include "resource.h"

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
    WORD arrMenuTemplate[] = {
        // MENUITEMTEMPLATEHEADER�ṹ
        0,
        0,

        // ���MENUITEMTEMPLATE�ṹ��һ��һ��
        // ��1�������˵�, ���һ���Ӳ˵�����ҪMF_HILITE��־
        MF_STRING | MF_POPUP, L'��', L'��', 0,
        MF_STRING, 40030, L'��', L'��', L'(', L'&', L'T', L')', L'\t', L'C', L't', L'r',
            L'l', L'+', L'X', 0,
        MF_STRING, 40031, L'��', L'��', L'(', L'&', L'C', L')', L'\t', L'C', L't', L'r',
            L'l', L'+', L'C', 0,
        MF_STRING, 40032, L'ճ', L'��', L'(', L'&', L'P', L')', L'\t', L'C', L't', L'r',
            L'l', L'+', L'V', 0,
        MF_SEPARATOR, 0, 0,
        MF_STRING, 40033, L'��', L'ɫ', L'(', L'&', L'R', L')', 0,
        MF_STRING, 40034, L'��', L'ɫ', L'(', L'&', L'G', L')', 0,
        MF_STRING | MF_HILITE, 40035, L'��', L'ɫ', L'(', L'&', L'B', L')', 0,

        // ��2�������˵�, ���һ���Ӳ˵�����ҪMF_HILITE��־
        MF_STRING | MF_POPUP, L'��', L'ͼ', 0,
        MF_STRING, 40036, L'��', L'ͼ', L'��', L'(', L'&', L'D', L')', 0,
        MF_STRING | MF_HILITE, 40037, L'С', L'ͼ', L'��', L'(', L'&', L'S', L')', 0,

        // ��3�������˵�, ��ʼ�ͽ�������ҪMF_HILITE��־��˵���������һ���˵�
        MF_STRING | MF_POPUP | MF_HELP | MF_HILITE, L'��', L'��', 0,
        MF_STRING | MF_HILITE, 40038, L'��', L'��', L'H', L'e', L'l', L'l', L'o', L'W', 
            L'i', L'n', L'd', L'o', L'w', L's', L'(', L'&', L'A', L')', 0
    };

    switch (uMsg)
    {
    case WM_CREATE:
        hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU));
        SetMenu(hwnd, hMenu);
        return 0;

    case WM_CHAR:
        // ���ڴ��ж�̬���ز˵���Դ(�˵�ģ��)
        hMenu = LoadMenuIndirect(arrMenuTemplate);
        SetMenu(hwnd, hMenu);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 40030:
            MessageBox(hwnd, TEXT("������ ����"), TEXT("��ʾ"), MB_OK);
            break;
        case 40036:
            MessageBox(hwnd, TEXT("������ ��ͼ��"), TEXT("��ʾ"), MB_OK);
            break;
        case 40038:
            MessageBox(hwnd, TEXT("������ ����HelloWindows"), TEXT("��ʾ"), MB_OK);
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}