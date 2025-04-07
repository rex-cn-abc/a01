#include <Windows.h>
#include "resource.h"

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID DrawClient(HWND hwnd, int nColor, int nShape);
VOID DrawStatic(HWND hwndStatic, int nColor, int nShape);

typedef struct
{
    int nColor;
    int nShape;
} COLORSHAPE, *PCOLORSHAPE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("ModalDialogBox");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

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
    static HINSTANCE hInstance;
    static HMENU hMenu;
    TCHAR szText[64] = { 0 };
    TCHAR szBuf[128] = { 0 };

    PAINTSTRUCT ps;
    static COLORSHAPE cs = { IDC_RADIO_RED, IDC_RADIO_ELLIPSE };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
        SetMenu(hwnd, hMenu);
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
            GetMenuString(hMenu, LOWORD(wParam), szText, _countof(szText), MF_BYCOMMAND);
            wsprintf(szBuf, TEXT("������� %s �˵������ID��%d\n"), szText, LOWORD(wParam));
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
            if (LOWORD(wParam) == ID_FILE_EXIT)
                SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        case ID_HELP_ABOUT:
            // ����û��������ģ̬�Ի����ȷ����ť��ˢ�¿ͻ���
            // ����û����ȷ����EndDialog��nResult��������ΪIDOK�����ȡ��������ΪIDCANCEL
            if (DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG),
                hwnd, DialogProc, (LPARAM)&cs) == IDOK)
            {
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        return 0;

    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        DrawClient(hwnd, cs.nColor, cs.nShape);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// �Ի��򴰿ڹ���
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static COLORSHAPE cs, *pCS;
    static HWND hwndStatic;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        // lParam ��������DialogBoxParam������dwInitParam�������ݹ������Զ�������
        pCS = (PCOLORSHAPE)lParam;  // �ò�����ֵ���û����ȷ����ť�Ժ�����
        cs = *pCS;                  // �ò������ڱ����ڹ�������Ϊ�ֲ���̬����ʹ��

        // ���ڻ�ͼ�ľ�̬�ؼ����
        hwndStatic = GetDlgItem(hwndDlg, IDC_STATIC_DRAW);

        // ������ɫ����״��ѡ��ť��ѡ��״̬
        CheckRadioButton(hwndDlg, IDC_RADIO_WHITE, IDC_RADIO_BLACK, cs.nColor);
        CheckRadioButton(hwndDlg, IDC_RADIO_RECT, IDC_RADIO_ELLIPSE, cs.nShape);

        SetFocus(GetDlgItem(hwndDlg, cs.nColor));
        return FALSE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_RADIO_WHITE:
        case IDC_RADIO_RED:
        case IDC_RADIO_GREEN:
        case IDC_RADIO_BLUE:
        case IDC_RADIO_CYAN:
        case IDC_RADIO_MAGENTA:
        case IDC_RADIO_YELLOW:
        case IDC_RADIO_BLACK:
            cs.nColor = LOWORD(wParam);
            DrawStatic(hwndStatic, cs.nColor, cs.nShape);
            break;

        case IDC_RADIO_RECT:
        case IDC_RADIO_ELLIPSE:
            cs.nShape = LOWORD(wParam);
            DrawStatic(hwndStatic, cs.nColor, cs.nShape);
            break;

        case IDOK:
            pCS->nColor = cs.nColor;
            pCS->nShape = cs.nShape;
            EndDialog(hwndDlg, IDOK);
            break;

        case IDCANCEL:
            EndDialog(hwndDlg, IDCANCEL);
            break;
        }
        return TRUE;

    case WM_PAINT:
        DrawStatic(hwndStatic, cs.nColor, cs.nShape);
        break;
    }

    return FALSE;
}

VOID DrawClient(HWND hwnd, int nColor, int nShape)
{
    COLORREF crColor[8] = {
        RGB(255,255,255), RGB(255,0,0), RGB(0,255,0), RGB(0,0,255),
        RGB(0,255,255), RGB(255,0,255), RGB(255,255,0), RGB(0,0,0) };
    HDC hdc;
    HBRUSH hBrush, hBrushOld;
    RECT rect;

    hdc = GetDC(hwnd);
    hBrush = CreateSolidBrush(crColor[nColor - IDC_RADIO_WHITE]);
    hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);
    GetClientRect(hwnd, &rect);

    if (nShape == IDC_RADIO_RECT)
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    else
        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

    DeleteObject(SelectObject(hdc, hBrushOld));
    ReleaseDC(hwnd, hdc);
}

VOID DrawStatic(HWND hwndStatic, int nColor, int nShape)
{
    InvalidateRect(hwndStatic, NULL, TRUE);
    UpdateWindow(hwndStatic);

    DrawClient(hwndStatic, nColor, nShape);
}