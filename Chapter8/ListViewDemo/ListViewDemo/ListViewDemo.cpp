#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <strsafe.h>
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
    static HWND hwndListView;                           // �б���ͼ�ؼ�
    static HWND hwndStatic, hwndComboBox;               // ��̬�ؼ�����Ͽ�
    static HWND hwndBtnCheck;                           // ��ȡѡ���ť
    static HIMAGELIST hImagListNormal, hImagListSmall;  // ͼ���б�
    static HFONT hFont;
    HICON hiconItem;

    LPTSTR arrItemText[] = { TEXT("ƽ�����"), TEXT("Web����"), TEXT("Windows�������Windows�������"),
        TEXT("��е���"), TEXT("���ܽ���"), TEXT("������") };
    LPTSTR arrPrice[] = { TEXT("15000"), TEXT("16000"), TEXT("19800"),
        TEXT("12000"), TEXT("10000"), TEXT("18000") };

    int nIndex;
    DWORD dwData;

    LVCOLUMN lvc = { 0 };       // ����Ϣ�ṹ
    LVITEM lvi = { 0 };         // �б�����Ϣ�ṹ
    LVTILEINFO lvti = { 0 };    // ƽ����ͼ��Ϣ�ṹ
    UINT arrUColumns[] = { 1 }; // ƽ����ͼҪ��ʾ������������

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // �б���ͼ�ؼ�
        hwndListView = CreateWindowEx(0, TEXT("SysListView32"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_SMALLICON | LVS_SHOWSELALWAYS | LVS_EDITLABELS,
            10, 0, 300, 200, hwnd, (HMENU)IDC_LISTVIEW, hInstance, NULL);
        // ��̬�ؼ�����Ͽ�
        hwndStatic = CreateWindowEx(0, TEXT("Static"), TEXT("��ͼ��ʽ��"),
            WS_CHILD | WS_VISIBLE, 320, 10, 100, 20, hwnd, (HMENU)(-1), hInstance, NULL);
        hwndComboBox = CreateWindowEx(0, TEXT("ComboBox"), NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL,
            320, 32, 120, 25, hwnd, (HMENU)IDC_COMBOBOX, hInstance, NULL);
        // ��ȡѡ���ť
        hwndBtnCheck = CreateWindowEx(0, TEXT("Button"), TEXT("��ȡѡ����"),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            320, 70, 100, 25, hwnd, (HMENU)IDC_BTNCHECK, hInstance, NULL);

        // ��Ͽ����һЩ�б�����ڸ����б���ͼ�ؼ���ʽ������ʽֵ�洢Ϊÿһ���б������Ŀ����
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

        // �����б���ͼ�ؼ�����չ��ʽ
        SendMessage(hwndListView, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES |
            LVS_EX_FULLROWSELECT | LVS_EX_AUTOCHECKSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

        // ͼ���б�
        hImagListNormal = ImageList_Create(GetSystemMetrics(SM_CXICON),
            GetSystemMetrics(SM_CYICON), ILC_MASK | ILC_COLOR32, 10, 0);
        hImagListSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON), ILC_MASK | ILC_COLOR32, 10, 0);
        for (int i = 0; i < 10; i++)
        {
            // ��Դ�ļ��ж�����10��ͼ��,IDI_ICON1��IDI_ICON10
            hiconItem = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1 + i));
            ImageList_AddIcon(hImagListNormal, hiconItem);
            ImageList_AddIcon(hImagListSmall, hiconItem);
            DestroyIcon(hiconItem);
        }
        SendMessage(hwndListView, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)hImagListSmall);

        // ����Ǳ�����ͼ��ʽ�����������б���
        lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_IMAGE;
        lvc.iSubItem = 0; lvc.cx = 160; lvc.pszText = TEXT("רҵ����"); lvc.iImage = 0;
        SendMessage(hwndListView, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);
        lvc.iSubItem = 1; lvc.cx = 100; lvc.pszText = TEXT("�۸�"); lvc.iImage = 1;
        SendMessage(hwndListView, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

        // �б���ͼ�ؼ����һЩ�б���
        lvi.mask = LVIF_TEXT | LVIF_IMAGE;
        for (int i = 0; i < _countof(arrItemText); i++)
        {
            lvi.iItem = i; lvi.iSubItem = 0; lvi.pszText = arrItemText[i]; lvi.iImage = i;
            SendMessage(hwndListView, LVM_INSERTITEM, i, (LPARAM)&lvi);

            // ���һ�����Ҳ���ǵ�2�У�����Ϊ1
            lvi.iSubItem = 1; lvi.pszText = arrPrice[i];
            SendMessage(hwndListView, LVM_SETITEM, 0, (LPARAM)&lvi);

            // ����ƽ����ͼҪ��ʾ����
            lvti.cbSize = sizeof(LVTILEINFO);
            lvti.iItem = i;
            lvti.cColumns = 1;
            lvti.puColumns = arrUColumns;
            SendMessage(hwndListView, LVM_SETTILEINFO, 0, (LPARAM)&lvti);
        }

        // ���þ�̬�ؼ�����Ͽ򡢰�ť����
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
        SendMessage(hwndStatic, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndComboBox, WM_SETFONT, (WPARAM)hFont, FALSE);
        SendMessage(hwndBtnCheck, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_COMMAND:
        // �л��б���ͼ��ʽ
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

        // ��ȡѡ���б���
        if (LOWORD(wParam) == IDC_BTNCHECK)
        {
            // ��ȡ��ѡ���б�������
            int nCount = SendMessage(hwndListView, LVM_GETSELECTEDCOUNT, 0, 0);
            if (nCount > 0)
            {
                LPTSTR pBuf = new TCHAR[nCount * 128];
                ZeroMemory(pBuf, sizeof(TCHAR) * nCount * 128);
                TCHAR szText[128] = { 0 };

                lvi.mask = LVIF_TEXT;
                lvi.iSubItem = 0; lvi.pszText = szText; lvi.cchTextMax = _countof(szText);

                // ����������һ��ѡ����
                int nIndex = SendMessage(hwndListView, LVM_GETNEXTITEM, -1, LVIS_SELECTED);
                // ��ȡ�б������Ϣ
                lvi.iItem = nIndex;
                SendMessage(hwndListView, LVM_GETITEM, 0, (LPARAM)&lvi);
                StringCchCopy(pBuf, nCount * 128, lvi.pszText);
                StringCchCat(pBuf, nCount * 128, TEXT("\n"));

                for (int i = 0; i < nCount - 1; i++)
                {
                    // ��������������
                    nIndex = SendMessage(hwndListView, LVM_GETNEXTITEM, nIndex, LVIS_SELECTED);
                    // ��ȡ�б������Ϣ
                    lvi.iItem = nIndex;
                    SendMessage(hwndListView, LVM_GETITEM, 0, (LPARAM)&lvi);
                    StringCchCat(pBuf, nCount * 128, lvi.pszText);
                    StringCchCat(pBuf, nCount * 128, TEXT("\n"));
                }
                MessageBox(hwnd, pBuf, TEXT("��ѡ���б���"), MB_OK);
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
            MessageBox(hwnd, TEXT("�û�����Ҽ������б���"), TEXT("��ʾ"), MB_OK);
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