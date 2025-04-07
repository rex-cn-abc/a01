#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define NUMBTNS 11

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
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);

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
    static HMENU hMenu;
    TCHAR szText[64] = { 0 };
    TCHAR szBuf[128] = { 0 };

    static HWND hwndStatus, hwndToolBar;
    UINT uArrIDs[] = { 0, ID_FILE };

    TBBUTTON tbButtons[NUMBTNS] = {
        { 0, ID_FILE_NEW,    TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("�½�") },
        { 1, ID_FILE_OPEN,   TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("��") },
        { 2, ID_FILE_SAVE,   TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("����") },
        { 3, ID_FILE_SAVEAS, TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("���Ϊ") },
        { 4, ID_FILE_EXIT,   TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("�˳�") },
        { 0, 0,              TBSTATE_ENABLED, TBSTYLE_SEP, { 0 }, 0, -1 },
        { 5, ID_EDIT_CUT,    TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("����") },
        { 6, ID_EDIT_COPY,   TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("����") },
        { 7, ID_EDIT_PASTE,  TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("ճ��") },
        { 0, 0,              TBSTATE_ENABLED, TBSTYLE_SEP, { 0 }, 0, -1 },
        { 8, ID_HELP_ABOUT,  TBSTATE_ENABLED, BTNS_BUTTON, { 0 }, 0, (INT_PTR)TEXT("����") } };
    //TBADDBITMAP tbab;
    static HIMAGELIST hImagList;

    switch (uMsg)
    {
    case WM_CREATE:
        hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU));
        SetMenu(hwnd, hMenu);

        // ����״̬��
        hwndStatus = CreateWindowEx(0, TEXT("msctls_statusbar32"), NULL,
            WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, hwnd, (HMENU)IDC_STATUSBAR, g_hInstance, NULL);

        // ����������
        hwndToolBar = CreateWindowEx(0, TEXT("ToolbarWindow32"), NULL,
            WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | CCS_ADJUSTABLE,
            0, 0, 0, 0, hwnd, (HMENU)IDC_TOOLBAR, g_hInstance, NULL);

        // ͼ���б�
        hImagList = ImageList_Create(50, 50, ILC_MASK | ILC_COLOR32, NUMBTNS, 0);
        ImageList_Add(hImagList, LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP)), NULL);
        SendMessage(hwndToolBar, TB_SETIMAGELIST, 0, (LPARAM)hImagList);

        // Ϊ���������ͼ��
        SendMessage(hwndToolBar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
        // ��Ϊ����ʹ����ͼ���б���������Ĵ���ע�͵�
        //SendMessage(hwndToolBar, TB_SETBITMAPSIZE, 0, MAKELPARAM(50, 50));
        //tbab.hInst = g_hInstance;
        //tbab.nID = IDB_BITMAP;      // ������ͼƬ�Ѿ���ӵ�λͼ��Դ
        //SendMessage(hwndToolBar, TB_ADDBITMAP, NUMBTNS - 2, (LPARAM)&tbab);

        // Ϊ��������Ӱ�ť
        SendMessage(hwndToolBar, TB_ADDBUTTONS, NUMBTNS, (LPARAM)&tbButtons);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_NEW:
        case ID_FILE_OPEN:
        case ID_FILE_SAVE:
        case ID_FILE_SAVEAS:
        case ID_FILE_EXIT:
        case ID_EDIT_CUT:
        case ID_EDIT_COPY:
        case ID_EDIT_PASTE:
        case ID_HELP_ABOUT:
            GetMenuString(hMenu, LOWORD(wParam), szText, _countof(szText), MF_BYCOMMAND);
            wsprintf(szBuf, TEXT("������� %s �˵������ID��%d\n"), szText, LOWORD(wParam));
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
            if (LOWORD(wParam) == ID_FILE_EXIT)
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
        SendMessage(hwndToolBar, TB_AUTOSIZE, 0, 0);
        return 0;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case TBN_QUERYINSERT:
        case TBN_QUERYDELETE:
            return TRUE;

        case TBN_GETBUTTONINFO:
            LPNMTOOLBAR lpnmTB;
            lpnmTB = (LPNMTOOLBAR)lParam;
            // ����������һ����ť
            if (lpnmTB->iItem < NUMBTNS)
            {
                lpnmTB->tbButton = tbButtons[lpnmTB->iItem];
                return TRUE;
            } 
            return FALSE;

        case TTN_NEEDTEXT:
            LPNMTTDISPINFO lpnmTDI;
            lpnmTDI = (LPNMTTDISPINFO)lParam;
            lpnmTDI->hinst = g_hInstance;
            lpnmTDI->lpszText = (LPTSTR)lpnmTDI->hdr.idFrom;
            break;
        }
        return 0;

    case WM_DESTROY:
        ImageList_Destroy(hImagList);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}