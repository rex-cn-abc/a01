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
    TCHAR szAppName[] = TEXT("ListBoxDemo");
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

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 256, 280, NULL, NULL, hInstance, NULL);

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
    static HWND hwndListBox, hwndEditText, hwndEditData, hwndAdd, hwndModify, hwndDelete;
    HWND hwndStaticText, hwndStaticData;
    static HFONT hFont;
    static UINT WM_DRAGLIST;
    TCHAR szBuf[64] = { 0 };
    int nIndex, nCount;
    DWORD dwData;

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 列表框控件
        hwndListBox = CreateWindowEx(0, TEXT("ListBox"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            20, 20, 200, 116, hwnd, (HMENU)IDC_LISTBOX, hInstance, NULL);

        // 静态控件和编辑框控件，文本：文本编辑框，项目数据：项目数据编辑框
        hwndStaticText = CreateWindowEx(0, TEXT("Static"), TEXT("文本："),
            WS_CHILD | WS_VISIBLE, 20, 144, 60, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndEditText = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            80, 140, 140, 22, hwnd, (HMENU)IDC_EDITTEXT, hInstance, NULL);
        hwndStaticData = CreateWindowEx(0, TEXT("Static"), TEXT("项目数据："),
            WS_CHILD | WS_VISIBLE, 20, 169, 60, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndEditData = CreateWindowEx(0, TEXT("Edit"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
            80, 165, 140, 22, hwnd, (HMENU)IDC_EDITDATA, hInstance, NULL);

        // 三个按钮，添加、修改、删除
        hwndAdd = CreateWindowEx(0, TEXT("Button"), TEXT("添加"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY,
            20, 200, 60, 25, hwnd, (HMENU)IDC_BTNADD, hInstance, NULL);
        hwndModify = CreateWindowEx(0, TEXT("Button"), TEXT("修改"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY,
            90, 200, 60, 25, hwnd, (HMENU)IDC_BTNMODIFY, hInstance, NULL);
        hwndDelete = CreateWindowEx(0, TEXT("Button"), TEXT("删除"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY,
            160, 200, 60, 25, hwnd, (HMENU)IDC_BTNDELETE, hInstance, NULL);

        // 更改为拖动列表框
        MakeDragList(hwndListBox);
        WM_DRAGLIST = RegisterWindowMessage(DRAGLISTMSGSTRING);

        // 添加一些列表项
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("汇编语言"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("C语言面向过程"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("C++面向对象"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("Windows程序设计"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("Ollydbg调试工具"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("X64Dbg调试工具"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("IDA静态调试"));
        SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)TEXT("PE Explorer"));
        // 设置项目数据，具体编程中可以根据需要添加所需的项目数据，例如某些自定义数据的指针
        for (int i = 0; i < 8; i++)
            SendMessage(hwndListBox, LB_SETITEMDATA, i, 10000 + i);

        // 限制列表项文本最多可以输入20个字符，项目数据最多可以输入10个数字字符
        SendMessage(hwndEditText, EM_SETLIMITTEXT, 20, 0);
        SendMessage(hwndEditData, EM_SETLIMITTEXT, 10, 0);

        // 设置字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndListBox, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndStaticText, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndStaticData, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndEditText, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndEditData, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndAdd, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndModify, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndDelete, WM_SETFONT, (WPARAM)hFont, FALSE);

        // 禁用添加、修改、删除按钮
        EnableWindow(hwndAdd, FALSE);
        EnableWindow(hwndModify, FALSE);
        EnableWindow(hwndDelete, FALSE);
        return 0;

    case WM_CTLCOLORSTATIC:
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
            // 添加
            if (LOWORD(wParam) == IDC_BTNADD)
            {
                nCount = GetDlgItemText(hwnd, IDC_EDITTEXT, szBuf, _countof(szBuf));
                if (nCount > 0)
                {
                    nIndex = SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)szBuf);
                    if (nIndex >= 0)
                    {
                        dwData = GetDlgItemInt(hwnd, IDC_EDITDATA, NULL, TRUE);
                        SendMessage(hwndListBox, LB_SETITEMDATA, nIndex, dwData);
                        SendMessage(hwndListBox, LB_SETCURSEL, nIndex, 0);
                        EnableWindow(hwndModify, TRUE);
                        EnableWindow(hwndDelete, TRUE);
                    }
                }
            }
            // 修改
            else if (LOWORD(wParam) == IDC_BTNMODIFY)
            {
                nIndex = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                nCount = GetDlgItemText(hwnd, IDC_EDITTEXT, szBuf, _countof(szBuf));
                if (nIndex >= 0 && nCount > 0)
                {
                    SendMessage(hwndListBox, LB_DELETESTRING, nIndex, 0);
                    SendMessage(hwndListBox, LB_INSERTSTRING, nIndex, (LPARAM)szBuf);
                    dwData = GetDlgItemInt(hwnd, IDC_EDITDATA, NULL, TRUE);
                    SendMessage(hwndListBox, LB_SETITEMDATA, nIndex, dwData);
                    SendMessage(hwndListBox, LB_SETCURSEL, nIndex, 0);
                }
            }
            // 删除
            else if (LOWORD(wParam) == IDC_BTNDELETE)
            {
                nIndex = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                if (nIndex >= 0)
                {
                    SendMessage(hwndListBox, LB_DELETESTRING, nIndex, 0);
                    nCount = SendMessage(hwndListBox, LB_GETCOUNT, 0, 0);
                    if (nCount > 0)
                    {
                        if (nIndex < nCount)
                            SendMessage(hwndListBox, LB_SETCURSEL, nIndex, 0);
                        else
                            SendMessage(hwndListBox, LB_SETCURSEL, nIndex - 1, 0);
                    }
                    else
                    {
                        EnableWindow(hwndModify, FALSE);
                        EnableWindow(hwndDelete, FALSE);
                    }
                }
            }
            break;
        // 列表框中的选择已更改
        case LBN_SELCHANGE:
            if (LOWORD(wParam) == IDC_LISTBOX)
            {
                // 获取列表项文本和项目数据，显示到编辑框中
                nIndex = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                SendMessage(hwndListBox, LB_GETTEXT, nIndex, (LPARAM)szBuf);
                SetDlgItemText(hwnd, IDC_EDITTEXT, szBuf);
                dwData = SendMessage(hwndListBox, LB_GETITEMDATA, nIndex, 0);
                SetDlgItemInt(hwnd, IDC_EDITDATA, dwData, TRUE);

                // 启用修改、删除按钮
                EnableWindow(hwndModify, TRUE);
                EnableWindow(hwndDelete, TRUE);
            }
            break;

        // 编辑控件的内容已变化
        case EN_UPDATE:
            // 项目数据编辑框不需要检查有无内容，如果没有内容就是0
            if (LOWORD(wParam) == IDC_EDITTEXT)
            {
                nCount = GetDlgItemText(hwnd, IDC_EDITTEXT, szBuf, _countof(szBuf));
                if (nCount > 0)
                {
                    // 添加按钮
                    EnableWindow(hwndAdd, TRUE);
                    // 修改按钮
                    nIndex = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                    if (nIndex >= 0)
                        EnableWindow(hwndModify, TRUE);
                }
                else
                {
                    // 添加按钮
                    EnableWindow(hwndAdd, FALSE);
                    // 修改按钮
                    EnableWindow(hwndModify, FALSE);
                }
            }
            break;
        }
        return 0;

    case WM_DESTROY:
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    if (uMsg == WM_DRAGLIST)
    {
        LPDRAGLISTINFO pDragInfo = (LPDRAGLISTINFO)lParam;
        static int nIndexDrag;

        switch (pDragInfo->uNotification)
        {
        case DL_BEGINDRAG:
            nIndexDrag = LBItemFromPt(hwndListBox, pDragInfo->ptCursor, FALSE);
            return TRUE;

        case DL_DRAGGING:
            nIndex = LBItemFromPt(hwndListBox, pDragInfo->ptCursor, TRUE);
            if (nIndex >= 0)
                DrawInsert(hwnd, hwndListBox, nIndex);
            return DL_MOVECURSOR;

        case DL_DROPPED:
            nIndex = LBItemFromPt(hwndListBox, pDragInfo->ptCursor, FALSE);
            if (nIndex >= 0 && nIndex != nIndexDrag)
            {
                SendMessage(hwndListBox, LB_GETTEXT, nIndexDrag, (LPARAM)szBuf);
                dwData = SendMessage(hwndListBox, LB_GETITEMDATA, nIndexDrag, 0);
                if (nIndexDrag > nIndex)
                {
                    // 先删除再插入
                    SendMessage(hwndListBox, LB_DELETESTRING, nIndexDrag, 0);
                    SendMessage(hwndListBox, LB_INSERTSTRING, nIndex, (LPARAM)szBuf);
                    SendMessage(hwndListBox, LB_SETITEMDATA, nIndex, dwData);
                    SendMessage(hwndListBox, LB_SETCURSEL, nIndex, 0);
                }
                else
                {
                    // 先插入再删除
                    SendMessage(hwndListBox, LB_INSERTSTRING, nIndex, (LPARAM)szBuf);
                    SendMessage(hwndListBox, LB_SETITEMDATA, nIndex, dwData);
                    SendMessage(hwndListBox, LB_SETCURSEL, nIndex, 0);
                    SendMessage(hwndListBox, LB_DELETESTRING, nIndexDrag, 0);
                }
            }
            // 隐藏插入图标
            DrawInsert(hwnd, hwndListBox, -1);
            nIndexDrag = -1;
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}