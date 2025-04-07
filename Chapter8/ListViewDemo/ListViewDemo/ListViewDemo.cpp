#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <strsafe.h>
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
    TCHAR szAppName[] = TEXT("ListViewDemo");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 465, 250, NULL, NULL, hInstance, NULL);

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
    static HWND hwndListView;                           // 列表视图控件
    static HWND hwndStatic, hwndComboBox;               // 静态控件、组合框
    static HWND hwndBtnCheck;                           // 获取选中项按钮
    static HIMAGELIST hImagListNormal, hImagListSmall;  // 图像列表
    static HFONT hFont;
    HICON hiconItem;

    LPTSTR arrItemText[] = { TEXT("平面设计"), TEXT("Web开发"), TEXT("Windows程序设计Windows程序设计"),
        TEXT("机械设计"), TEXT("加密解密"), TEXT("大数据") };
    LPTSTR arrPrice[] = { TEXT("15000"), TEXT("16000"), TEXT("19800"),
        TEXT("12000"), TEXT("10000"), TEXT("18000") };

    int nIndex;
    DWORD dwData;

    LVCOLUMN lvc = { 0 };       // 列信息结构
    LVITEM lvi = { 0 };         // 列表项信息结构
    LVTILEINFO lvti = { 0 };    // 平铺视图信息结构
    UINT arrUColumns[] = { 1 }; // 平铺视图要显示的列索引数组

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 列表视图控件
        hwndListView = CreateWindowEx(0, TEXT("SysListView32"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_SMALLICON | LVS_SHOWSELALWAYS | LVS_EDITLABELS,
            10, 0, 300, 200, hwnd, (HMENU)IDC_LISTVIEW, hInstance, NULL);
        // 静态控件、组合框
        hwndStatic = CreateWindowEx(0, TEXT("Static"), TEXT("视图样式："),
            WS_CHILD | WS_VISIBLE, 320, 10, 100, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndComboBox = CreateWindowEx(0, TEXT("ComboBox"), NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
            320, 32, 120, 25, hwnd, (HMENU)IDC_COMBOBOX, hInstance, NULL);
        // 获取选中项按钮
        hwndBtnCheck = CreateWindowEx(0, TEXT("Button"), TEXT("获取选中项"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 70, 100, 25, hwnd, (HMENU)IDC_BTNCHECK, hInstance, NULL);

        // 组合框添加一些列表项，用于更换列表视图控件样式，把样式值存储为每一个列表项的项目数据
        nIndex = SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("LV_VIEW_ICON"));
        SendMessage(hwndComboBox, CB_SETITEMDATA, nIndex, LV_VIEW_ICON);
        nIndex = SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("LV_VIEW_SMALLICON"));
        SendMessage(hwndComboBox, CB_SETITEMDATA, nIndex, LV_VIEW_SMALLICON);
        nIndex = SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("LV_VIEW_LIST"));
        SendMessage(hwndComboBox, CB_SETITEMDATA, nIndex, LV_VIEW_LIST);
        nIndex = SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("LV_VIEW_DETAILS"));
        SendMessage(hwndComboBox, CB_SETITEMDATA, nIndex, LV_VIEW_DETAILS);
        nIndex = SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)TEXT("LV_VIEW_TILE"));
        SendMessage(hwndComboBox, CB_SETITEMDATA, nIndex, LV_VIEW_TILE);
        SendMessage(hwndComboBox, CB_SETCURSEL, 1, 0);

        // 设置列表视图控件的扩展样式
        SendMessage(hwndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES |
            LVS_EX_FULLROWSELECT | LVS_EX_AUTOCHECKSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

        // 图像列表
        hImagListNormal = ImageList_Create(GetSystemMetrics(SM_CXICON),
            GetSystemMetrics(SM_CYICON), ILC_MASK | ILC_COLOR32, 10, 0);
        hImagListSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON), ILC_MASK | ILC_COLOR32, 10, 0);
        for (int i = 0; i < 10; i++)
        {
            // 资源文件中定义了10个图标,IDI_ICON1～IDI_ICON10
            hiconItem = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1 + i));
            ImageList_AddIcon(hImagListNormal, hiconItem);
            ImageList_AddIcon(hImagListSmall, hiconItem);
            DestroyIcon(hiconItem);
        }
        SendMessage(hwndListView, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImagListSmall);

        // 如果是报表视图样式，必须设置列标题
        lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_IMAGE;
        lvc.iSubItem = 0; lvc.cx = 160; lvc.pszText = TEXT("专业名称"); lvc.iImage = 0;
        SendMessage(hwndListView, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);
        lvc.iSubItem = 1; lvc.cx = 100; lvc.pszText = TEXT("价格"); lvc.iImage = 1;
        SendMessage(hwndListView, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

        // 列表视图控件添加一些列表项
        lvi.mask = LVIF_TEXT | LVIF_IMAGE;
        for (int i = 0; i < _countof(arrItemText); i++)
        {
            lvi.iItem = i; lvi.iSubItem = 0; lvi.pszText = arrItemText[i]; lvi.iImage = i;
            SendMessage(hwndListView, LVM_INSERTITEM, i, (LPARAM)&lvi);

            // 添加一个子项，也就是第2列，索引为1
            lvi.iSubItem = 1; lvi.pszText = arrPrice[i];
            SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);

            // 设置平铺视图要显示的列
            lvti.cbSize = sizeof(LVTILEINFO);
            lvti.iItem = i;
            lvti.cColumns = 1;
            lvti.puColumns = arrUColumns;
            SendMessage(hwndListView, LVM_SETTILEINFO, 0, (LPARAM)&lvti);
        }

        // 设置静态控件、组合框、按钮字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndStatic, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndComboBox, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndBtnCheck, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_COMMAND:
        // 切换列表视图样式
        switch (HIWORD(wParam))
        {
        case CBN_SELCHANGE:
            nIndex = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);
            dwData = SendMessage(hwndComboBox, CB_GETITEMDATA, nIndex, 0);
            if (dwData == LV_VIEW_ICON || dwData == LV_VIEW_TILE)
            {
                SendMessage(hwndListView, LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)hImagListNormal);
                SendMessage(hwndListView, LVM_SETVIEW, dwData, 0);
            }
            else
            {
                SendMessage(hwndListView, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImagListSmall);
                SendMessage(hwndListView, LVM_SETVIEW, dwData, 0);
            }
            break;
        }

        // 获取选中列表项
        if (LOWORD(wParam) == IDC_BTNCHECK)
        {
            // 获取已选中列表项总数
            int nCount = SendMessage(hwndListView, LVM_GETSELECTEDCOUNT, 0, 0);
            if (nCount > 0)
            {
                LPTSTR pBuf = new TCHAR[nCount * 128];
                ZeroMemory(pBuf, sizeof(TCHAR) * nCount * 128);
                TCHAR szText[128] = { 0 };

                lvi.mask = LVIF_TEXT;
                lvi.iSubItem = 0; lvi.pszText = szText; lvi.cchTextMax = _countof(szText);

                // 先搜索出第一个选中项
                int nIndex = SendMessage(hwndListView, LVM_GETNEXTITEM, -1, LVIS_SELECTED);
                // 获取列表项的信息
                lvi.iItem = nIndex;
                SendMessage(hwndListView, LVM_GETITEM, 0, (LPARAM)&lvi);
                StringCchCopy(pBuf, nCount * 128, lvi.pszText);
                StringCchCat(pBuf, nCount * 128, TEXT("\n"));

                for (int i = 0; i < nCount - 1; i++)
                {
                    // 按索引往后搜索
                    nIndex = SendMessage(hwndListView, LVM_GETNEXTITEM, nIndex, LVIS_SELECTED);
                    // 获取列表项的信息
                    lvi.iItem = nIndex;
                    SendMessage(hwndListView, LVM_GETITEM, 0, (LPARAM)&lvi);
                    StringCchCat(pBuf, nCount * 128, lvi.pszText);
                    StringCchCat(pBuf, nCount * 128, TEXT("\n"));
                }
                MessageBox(hwnd, pBuf, TEXT("已选中列表项"), MB_OK);
                delete[] pBuf;
            }
        }
        return 0;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case LVN_ENDLABELEDIT:
            return TRUE;

        case NM_RCLICK:
            MessageBox(hwnd, TEXT("用户鼠标右键单击列表项"), TEXT("提示"), MB_OK);
            break;
        }
        return 0;

    case WM_CTLCOLORSTATIC:
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_DESTROY:
        DeleteObject(hFont);
        ImageList_Destroy(hImagListNormal);
        ImageList_Destroy(hImagListSmall);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}