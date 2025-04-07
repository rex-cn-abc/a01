#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#pragma comment(linker, "\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ��ť���´��ڹ���
LRESULT CALLBACK ButtonProc(HWND hwndButton, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ����Ĭ�ϰ�ť
VOID OnDefPushButton(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("Buttons");    // ������⡢��������
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
    BS_PUSHBUTTON | BS_NOTIFY | WS_TABSTOP,                 TEXT("��ͨ��ť"), // CtrlID 1000
    BS_ICON | BS_NOTIFY | WS_TABSTOP,                       TEXT("ͼ�갴ť"),
    BS_BITMAP | BS_NOTIFY | WS_TABSTOP,                     TEXT("λͼ��ť"),
    BS_OWNERDRAW,                                           TEXT("�Ի水ť"),

    BS_GROUPBOX,                                            TEXT("������ò"), // CtrlID 1004
    BS_AUTORADIOBUTTON | BS_NOTIFY | WS_GROUP | WS_TABSTOP, TEXT("�й���Ա"),
    BS_AUTORADIOBUTTON | BS_NOTIFY,                         TEXT("������Ա"),
    BS_AUTORADIOBUTTON | BS_NOTIFY,                         TEXT("�޵�����ʿ"),

    BS_GROUPBOX,                                            TEXT("���˰���"), // CtrlID 1008
    BS_AUTOCHECKBOX | BS_NOTIFY | WS_GROUP | WS_TABSTOP,    TEXT("����"),
    BS_AUTOCHECKBOX | BS_NOTIFY,                            TEXT("����"),
    BS_AUTOCHECKBOX | BS_NOTIFY,                            TEXT("������"),

    BS_GROUPBOX,                                            TEXT("�����ƺ�"), // CtrlID 1012
    BS_AUTO3STATE | BS_NOTIFY | WS_GROUP | WS_TABSTOP,      TEXT("�ŶӺ���"),
    BS_AUTO3STATE | BS_NOTIFY,                              TEXT("��������"),
    BS_AUTO3STATE | BS_NOTIFY,                              TEXT("�Ƚ�����"),

    BS_DEFPUSHBUTTON | BS_NOTIFY | WS_TABSTOP,              TEXT("Ĭ�ϰ�ť"), // CtrlID 1016
};

#define NUM (sizeof(Buttons) / sizeof(Buttons[0]))

WNDPROC ButtonProcOld[NUM];     // �����Ӵ��ڿؼ�ԭ���ڹ���

// ��ӦTab����8���Ӵ��ڿؼ�
int idFocus[] = { IDC_PUSHBUTTON, IDC_ICONBUTTON, IDC_BITMAPBUTTON, IDC_OWNERDRAWBUTTON,
            IDC_AUTORADIOBUTTON1, IDC_AUTOCHECKBOX1, IDC_AUTO3STATE1, IDC_DEFPUSHBUTTON };
// ��ǰ���м��̽���İ�ť����
int idFocusIndex;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndButton[NUM];                // �Ӵ��ڿؼ��������
    int arrPos[NUM] = { 10, 40, 70, 100,        // ÿ���Ӵ��ڿؼ�����ʼY����
        130, 150, 180, 210,
        250, 270, 300, 330,
        370, 390, 420, 450,
        490 };
    LPDRAWITEMSTRUCT lpDIS;
    HFONT hFont;

    switch (uMsg)
    {
    case WM_CREATE:
        // ����17���Ӵ��ڿؼ�
        for (int i = 0; i < NUM; i++)
        {
            hwndButton[i] = CreateWindowEx(0, TEXT("Button"), Buttons[i].m_pText,
                WS_CHILD | WS_VISIBLE | Buttons[i].m_nStyle, 20, arrPos[i],
                150, 25, hwnd, (HMENU)(1000 + i),
                ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        }

        // �ƶ�3��������λ��
        MoveWindow(hwndButton[4], 10, arrPos[4], 170, 115, TRUE);
        MoveWindow(hwndButton[8], 10, arrPos[8], 170, 115, TRUE);
        MoveWindow(hwndButton[12], 10, arrPos[12], 170, 115, TRUE);

        // Ϊͼ�갴ť��λͼ��ť����ͼ�ꡢλͼ
        SendDlgItemMessage(hwnd, IDC_ICONBUTTON, BM_SETIMAGE, IMAGE_ICON,
            (LPARAM)LoadImage(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDI_SMILE),
                IMAGE_ICON, 20, 20, LR_DEFAULTCOLOR));
        SendDlgItemMessage(hwnd, IDC_BITMAPBUTTON, BM_SETIMAGE, IMAGE_BITMAP,
            (LPARAM)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_KONGLONG)));

        // ����Ĭ�ϰ�ť���ı�
        SetDlgItemText(hwnd, IDC_DEFPUSHBUTTON, TEXT("��ȡ��ѡ��ѡ״̬"));

        // ��ѡ��ť����ѡ����̬��ѡ��Ĭ������·ֱ�ѡ��һ��
        CheckRadioButton(hwnd, IDC_AUTORADIOBUTTON1, IDC_AUTORADIOBUTTON3, IDC_AUTORADIOBUTTON2);
        CheckDlgButton(hwnd, IDC_AUTOCHECKBOX3, BST_CHECKED);
        CheckDlgButton(hwnd, IDC_AUTO3STATE2, BST_INDETERMINATE);

        // �����Ӵ��ڿؼ�����
        hFont = CreateFont(12, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("����"));
        for (int i = 0; i < NUM; i++)
            SendMessage(hwndButton[i], WM_SETFONT, (WPARAM)hFont, FALSE);

        // �Ӵ��ڿؼ����໯
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
            // �Ӵ��ڿؼ�ID����������μ�resource.h�����Ը�����Ҫ�ڴ˴���ÿ���ؼ��ĵ���¼�
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
        // �ȰѰ�ť�������Ϊ�ʹ��ڱ���һ�µİ�ɫ��Ȼ��һ����ɫԲ�Ǿ���
        SelectObject(lpDIS->hDC, GetStockObject(NULL_PEN));
        //SelectObject(lpDIS->hDC, GetStockObject(WHITE_BRUSH));
        //Rectangle(lpDIS->hDC, 0, 0, lpDIS->rcItem.right + 1, lpDIS->rcItem.bottom + 1);
        SelectObject(lpDIS->hDC, GetStockObject(BLACK_BRUSH));
        RoundRect(lpDIS->hDC, 0, 0, lpDIS->rcItem.right + 1, lpDIS->rcItem.bottom + 1, 20, 20);

        // ���û������ť��ʱ�򣬻���COLOR_HIGHLIGHT��ɫ��Բ�Ǿ���
        if (lpDIS->itemState & ODS_SELECTED)
        {
            SelectObject(lpDIS->hDC, GetSysColorBrush(COLOR_HIGHLIGHT));
            RoundRect(lpDIS->hDC, 0, 0, lpDIS->rcItem.right + 1, lpDIS->rcItem.bottom + 1, 20, 20);
        }
        // ����ť��ý����ʱ�򣬿��Ի���һ���������
        if (lpDIS->itemState & ODS_FOCUS)
        {
            InflateRect(&lpDIS->rcItem, -2, -2);
            DrawFocusRect(lpDIS->hDC, &lpDIS->rcItem);
        }

        // �Ի水ť���ı���͸�������İ�ɫ����
        SetBkMode(lpDIS->hDC, TRANSPARENT);
        SetTextColor(lpDIS->hDC, RGB(255, 255, 255));
        DrawText(lpDIS->hDC, TEXT("�Ի水ť"), _tcslen(TEXT("�Ի水ť")),
            &lpDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        // �ָ��豸����
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
        StringCchCopy(szBuf, _countof(szBuf), TEXT("������ò���й���Ա\n"));
    if (IsDlgButtonChecked(hwnd, IDC_AUTORADIOBUTTON2) & BST_CHECKED)
        StringCchCopy(szBuf, _countof(szBuf), TEXT("������ò��������Ա\n"));
    if (IsDlgButtonChecked(hwnd, IDC_AUTORADIOBUTTON3) & BST_CHECKED)
        StringCchCopy(szBuf, _countof(szBuf), TEXT("������ò���޵�����ʿ\n"));

    StringCchCat(szBuf, _countof(szBuf), TEXT("���˰��ã�"));
    if (IsDlgButtonChecked(hwnd, IDC_AUTOCHECKBOX1) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("���� "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTOCHECKBOX2) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("���� "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTOCHECKBOX3) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("������"));
    StringCchCat(szBuf, _countof(szBuf), TEXT("\n"));

    StringCchCat(szBuf, _countof(szBuf), TEXT("�����ƺţ�"));
    if (IsDlgButtonChecked(hwnd, IDC_AUTO3STATE1) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("�ŶӺ��� "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTO3STATE2) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("�������� "));
    if (IsDlgButtonChecked(hwnd, IDC_AUTO3STATE3) & BST_CHECKED)
        StringCchCat(szBuf, _countof(szBuf), TEXT("�Ƚ�����"));
    StringCchCat(szBuf, _countof(szBuf), TEXT("\n"));

    MessageBox(hwnd, szBuf, TEXT("���˼�����"), MB_OK);
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