#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 按钮类新窗口过程
LRESULT CALLBACK ButtonProc(HWND hwndButton, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 按下默认按钮
VOID OnDefPushButton(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("Buttons");    // 程序标题、窗口类名
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
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

struct
{
    int     m_nStyle;
    PTSTR   m_pText;
}Buttons[] = {
    BS_PUSHBUTTON | BS_NOTIFY | WS_TABSTOP,                 TEXT("普通按钮"), // CtrlID 1000
    BS_ICON | BS_NOTIFY | WS_TABSTOP,                       TEXT("图标按钮"),
    BS_BITMAP | BS_NOTIFY | WS_TABSTOP,                     TEXT("位图按钮"),
    BS_OWNERDRAW,                                           TEXT("自绘按钮"),

    BS_GROUPBOX,                                            TEXT("政治面貌"), // CtrlID 1004
    BS_AUTORADIOBUTTON | BS_NOTIFY | WS_GROUP | WS_TABSTOP, TEXT("中共党员"),
    BS_AUTORADIOBUTTON | BS_NOTIFY,                         TEXT("共青团员"),
    BS_AUTORADIOBUTTON | BS_NOTIFY,                         TEXT("无党派人士"),

    BS_GROUPBOX,                                            TEXT("个人爱好"), // CtrlID 1008
    BS_AUTOCHECKBOX | BS_NOTIFY | WS_GROUP | WS_TABSTOP,    TEXT("看书"),
    BS_AUTOCHECKBOX | BS_NOTIFY,                            TEXT("唱歌"),
    BS_AUTOCHECKBOX | BS_NOTIFY,                            TEXT("听音乐"),

    BS_GROUPBOX,                                            TEXT("荣誉称号"), // CtrlID 1012
    BS_AUTO3STATE | BS_NOTIFY | WS_GROUP | WS_TABSTOP,      TEXT("团队核心"),
    BS_AUTO3STATE | BS_NOTIFY,                              TEXT("技术能手"),
    BS_AUTO3STATE | BS_NOTIFY,                              TEXT("先进个人"),

    BS_DEFPUSHBUTTON | BS_NOTIFY | WS_TABSTOP,              TEXT("默认按钮"), // CtrlID 1016
};

#define NUM (sizeof(Buttons) / sizeof(Buttons[0]))

WNDPROC ButtonProcOld[NUM];     // 保存子窗口控件原窗口过程

// 响应Tab键的8个子窗口控件
int idFocus[] = { IDC_PUSHBUTTON, IDC_ICONBUTTON, IDC_BITMAPBUTTON, IDC_OWNERDRAWBUTTON,
            IDC_AUTORADIOBUTTON1, IDC_AUTOCHECKBOX1, IDC_AUTO3STATE1, IDC_DEFPUSHBUTTON };
// 当前具有键盘焦点的按钮索引
int idFocusIndex;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndButton[NUM];                // 子窗口控件句柄数组
    int arrPos[NUM] = { 10, 40, 70, 100,        // 每个子窗口控件的起始Y坐标
        130, 150, 180, 210,
        250, 270, 300, 330,
        370, 390, 420, 450,
        490 };
    LPDRAWITEMSTRUCT lpDIS;
    HFONT hFont;

    switch (uMsg)
    {
    case WM_CREATE:
        // 创建17个子窗口控件
        for (int i = 0; i < NUM; i++)
        {
            hwndButton[i] = CreateWindowEx(0, TEXT("Button"), Buttons[i].m_pText,
                WS_CHILD | WS_VISIBLE | Buttons[i].m_nStyle, 20, arrPos[i],
                150, 25, hwnd, (HMENU)(1000 + i),
                ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        }

        // 移动3个分组框的位置
        MoveWindow(hwndButton[4], 10, arrPos[4], 170, 115, TRUE);
        MoveWindow(hwndButton[8], 10, arrPos[8], 170, 115, TRUE);
        MoveWindow(hwndButton[12], 10, arrPos[12], 170, 115, TRUE);

        // 为图标按钮、位图按钮设置图标、位图
        SendDlgItemMessage(hwnd, IDC_ICONBUTTON, BM_SETIMAGE, IMAGE_ICON,
            (LPARAM)LoadImage(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDI_SMILE),
                IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR));
        SendDlgItemMessage(hwnd, IDC_BITMAPBUTTON, BM_SETIMAGE, IMAGE_BITMAP,
            (LPARAM)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_KONGLONG)));

        // 设置默认按钮的文本
        SetDlgItemText(hwnd, IDC_DEFPUSHBUTTON, TEXT("获取单选复选状态"));

        // 单选按钮、复选框、三态复选框默认情况下分别选中一项
        CheckRadioButton(hwnd, IDC_AUTORADIOBUTTON1, IDC_AUTORADIOBUTTON3, IDC_AUTORADIOBUTTON2);
        CheckDlgButton(hwnd, IDC_AUTOCHECKBOX3, BST_CHECKED);
        CheckDlgButton(hwnd, IDC_AUTO3STATE2, BST_INDETERMINATE);

        // 设置子窗口控件字体
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("宋体"));
        for (int i = 0; i < NUM; i++)
            SendMessage(hwndButton[i], WM_SETFONT, (WPARAM)hFont, FALSE);

        // 子窗口控件子类化
        for (int i = 0; i < NUM; i++)
            ButtonProcOld[i] = (WNDPROC)SetWindowLongPtr(hwndButton[i], GWLP_WNDPROC, (LONG_PTR)ButtonProc);
        return 0;

    case WM_SETFOCUS:
        SetFocus(GetDlgItem(hwnd, idFocus[idFocusIndex]));
        return 0;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            switch (LOWORD(wParam))
            {
            // 子窗口控件ID常量定义请参见resource.h，可以根据需要在此处理每个控件的点击事件
            case IDC_PUSHBUTTON:      break;
            case IDC_ICONBUTTON:      break;
            case IDC_BITMAPBUTTON:    break;
            case IDC_OWNERDRAWBUTTON: break;
            case IDC_AUTORADIOBUTTON1:break;
            case IDC_AUTORADIOBUTTON2:break;
            case IDC_AUTORADIOBUTTON3:break;
            case IDC_AUTOCHECKBOX1:   break;
            case IDC_AUTOCHECKBOX2:   break;
            case IDC_AUTOCHECKBOX3:   break;
            case IDC_AUTO3STATE1:     break;
            case IDC_AUTO3STATE2:     break;
            case IDC_AUTO3STATE3:     break;
            case IDC_DEFPUSHBUTTON:   OnDefPushButton(hwnd); break;
            }
        }
        return 0;

    case WM_DRAWITEM:
        lpDIS = (LPDRAWITEMSTRUCT)lParam;
        // 先把按钮矩形填充为和窗口背景一致的白色，然后画一个黑色圆角矩形
        SelectObject(lpDIS->hDC, GetStockObject(NULL_PEN));
        //SelectObject(lpDIS->hDC, GetStockObject(WHITE_BRUSH));
        //Rectangle(lpDIS->hDC, 0, 0, lpDIS->rcItem.right + 1, lpDIS->rcItem.bottom + 1);
        SelectObject(lpDIS->hDC, GetStockObject(BLACK_BRUSH));
        RoundRect(lpDIS->hDC, 0, 0, lpDIS->rcItem.right + 1, lpDIS->rcItem.bottom + 1, 20, 20);

        // 当用户点击按钮的时候，绘制COLOR_HIGHLIGHT颜色的圆角矩形
        if (lpDIS->itemState & ODS_SELECTED)
        {
            SelectObject(lpDIS->hDC, GetSysColorBrush(COLOR_HIGHLIGHT));
            RoundRect(lpDIS->hDC, 0, 0, lpDIS->rcItem.right + 1, lpDIS->rcItem.bottom + 1, 20, 20);
        }
        // 当按钮获得焦点的时候，可以绘制一个焦点矩形
        if (lpDIS->itemState & ODS_FOCUS)
        {
            InflateRect(&lpDIS->rcItem, -2, -2);
            DrawFocusRect(lpDIS->hDC, &lpDIS->rcItem);
        }

        // 自绘按钮的文本，透明背景的白色文字
        SetBkMode(lpDIS->hDC, TRANSPARENT);
        SetTextColor(lpDIS->hDC, RGB(255, 255, 255));
        DrawText(lpDIS->hDC, TEXT("自绘按钮"), _tcslen(TEXT("自绘按钮")),
            &lpDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // 恢复设备环境
        SelectObject(lpDIS->hDC, GetStockObject(BLACK_PEN));
        SelectObject(lpDIS->hDC, GetStockObject(WHITE_BRUSH));
        return TRUE;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

VOID OnDefPushButton(HWND hwnd)
{
    TCHAR szBuf[128] = { 0 };

    if (IsDlgButtonChecked(hwnd, IDC_AUTORADIOBUTTON1) & BST_CHECKED)
        StringCchCopy(szBuf, _countof(szBuf), TEXT("政治面貌：中共党员\n"));
    if (IsDlgButtonChecked(hwnd, IDC_AUTORADIOBUTTON2) & BST_CHECKED)
        StringCchCopy(szBuf, _countof(szBuf), TEXT("政治面貌：共青团员\n"));
    if (IsDlgButtonChecked(hwnd, IDC_AUTORADIOBUTTON3) & BST_CHECKED)
        StringCchCopy(szBuf, _countof(szBuf), TEXT("政治面貌：无党派人士\n"));

    StringCchCat(szBuf, _countof(szBuf), TEXT("个人爱好："));
    if (IsDlgButtonChecked(hwnd, IDC_AUTOCHECKBOX1) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("看书 "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTOCHECKBOX2) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("唱歌 "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTOCHECKBOX3) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("听音乐"));
    StringCchCat(szBuf, _countof(szBuf), TEXT("\n"));

    StringCchCat(szBuf, _countof(szBuf), TEXT("荣誉称号："));
    if (IsDlgButtonChecked(hwnd, IDC_AUTO3STATE1) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("团队核心 "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTO3STATE2) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("技术能手 "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTO3STATE3) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("先进个人"));
    StringCchCat(szBuf, _countof(szBuf), TEXT("\n"));

    MessageBox(hwnd, szBuf, TEXT("个人简介汇总"), MB_OK);
}

LRESULT CALLBACK ButtonProc(HWND hwndButton, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int id = GetWindowLongPtr(hwndButton, GWLP_ID);

    switch (uMsg)
    {
    case WM_CHAR:
        if (wParam == VK_TAB)
        {
            idFocusIndex += GetKeyState(VK_SHIFT) < 0 ? 7 : 1;
            idFocusIndex %= 8;
            SetFocus(GetDlgItem(GetParent(hwndButton), idFocus[idFocusIndex]));
        }
        if (wParam == VK_RETURN)
        {
            SendDlgItemMessage(GetParent(hwndButton), IDC_DEFPUSHBUTTON, BM_CLICK, 0, 0);
        }
        break;
    }

    return CallWindowProc(ButtonProcOld[id - 1000], hwndButton, uMsg, wParam, lParam);
}