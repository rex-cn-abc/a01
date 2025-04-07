#include <windows.h>
#include <Commctrl.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 全局变量
HINSTANCE g_hInstance;
BOOL g_bStartScanning = FALSE;            // 是否已经开始扫描

// 函数声明
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndBtnHome, hwndBtnClose, hwndBtnBrowse, hwndBtnStartStop, hwndListResult;
    HBITMAP hBitmap;
    BITMAP bmp;
    HBRUSH hBrush;
    LPDRAWITEMSTRUCT lpDrawItemStruct;      // 用于自绘4个按钮的WM_DRAWITEM消息
    HDC hdcMem;                             // 内存DC
    LVCOLUMN lvc;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        // 窗口句柄
        hwndBtnHome = GetDlgItem(hwndDlg, IDC_BTN_HOME);
        hwndBtnClose = GetDlgItem(hwndDlg, IDC_BTN_CLOSE);
        hwndBtnBrowse = GetDlgItem(hwndDlg, IDC_BTN_BROWSE);
        hwndBtnStartStop = GetDlgItem(hwndDlg, IDC_BTN_STARTSTOP);
        hwndListResult = GetDlgItem(hwndDlg, IDC_LIST_RESULT);

        // 设置程序窗口大小
        hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
        GetObject(hBitmap, sizeof(bmp), &bmp);
        SetWindowPos(hwndDlg, NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOMOVE | SWP_NOZORDER);
        DeleteObject(hBitmap);

        // 设置程序窗口标题(根据实际需要设置程序窗口标题)
        SetWindowText(hwndDlg, TEXT("老王专用杀毒软件"));

        // 列表视图控件扩展样式
        SendMessage(hwndListResult, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
        // 列表视图控件列标题
        lvc.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvc.iSubItem = 0; lvc.cx = 350; lvc.pszText = (LPTSTR)TEXT("文件路径");
        SendMessage(hwndListResult, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);
        lvc.iSubItem = 1; lvc.cx = 100; lvc.pszText = (LPTSTR)TEXT("扫描结果");
        SendMessage(hwndListResult, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);
        lvc.iSubItem = 2; lvc.cx = 100; lvc.pszText = (LPTSTR)TEXT("处理结果");
        SendMessage(hwndListResult, LVM_INSERTCOLUMN, 2, (LPARAM)&lvc);
        return TRUE;

    case WM_CTLCOLORDLG:
        // 程序窗口背景图片
        hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
        hBrush = CreatePatternBrush(hBitmap);
        DeleteObject(hBitmap);
        return (INT_PTR)hBrush;

    case WM_CTLCOLORSTATIC:
        // 静态控件和复选框等的背景颜色
        SetBkColor((HDC)wParam, RGB(221, 221, 221));
        hBrush = CreateSolidBrush(RGB(221, 221, 221));
        return (INT_PTR)hBrush;

    case WM_DRAWITEM:
        // 自绘按钮
        lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
        hdcMem = CreateCompatibleDC(lpDrawItemStruct->hDC);

        switch (lpDrawItemStruct->CtlID)
        {
        case IDC_BTN_HOME:
            hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_HOMEBTNNORMAL));
            SelectObject(hdcMem, hBitmap);
            GetObject(hBitmap, sizeof(bmp), &bmp);
            BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteObject(hBitmap);

            // 当用户点击按钮的时候
            if (lpDrawItemStruct->itemState & ODS_SELECTED)
            {
                hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_HOMEBTNDOWN));
                SelectObject(hdcMem, hBitmap);
                BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);
            }
            break;

        case IDC_BTN_CLOSE:
            hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_CLOSEBTNNORMAL));
            SelectObject(hdcMem, hBitmap);
            GetObject(hBitmap, sizeof(bmp), &bmp);
            BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteObject(hBitmap);

            // 当用户点击按钮的时候
            if (lpDrawItemStruct->itemState & ODS_SELECTED)
            {
                hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_CLOSEBTNDOWN));
                SelectObject(hdcMem, hBitmap);
                BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);
            }
            break;

        case IDC_BTN_BROWSE:
            hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BROWSEBTNNORMAL));
            SelectObject(hdcMem, hBitmap);
            GetObject(hBitmap, sizeof(bmp), &bmp);
            BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteObject(hBitmap);

            // 当用户点击按钮的时候
            if (lpDrawItemStruct->itemState & ODS_SELECTED)
            {
                hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BROWSEBTNDONW));
                SelectObject(hdcMem, hBitmap);
                BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);
            }
            break;

        case IDC_BTN_STARTSTOP:
            // 是否已经开始扫描
            if (g_bStartScanning)
            {
                hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_STOPBTNNORMAL));
                SelectObject(hdcMem, hBitmap);
                GetObject(hBitmap, sizeof(bmp), &bmp);
                BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);

                // 当用户点击按钮的时候
                if (lpDrawItemStruct->itemState & ODS_SELECTED)
                {
                    hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_STOPBTNDOWN));
                    SelectObject(hdcMem, hBitmap);
                    BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                    DeleteObject(hBitmap);
                }
            }
            else
            {
                hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_STARTBTNNORMAL));
                SelectObject(hdcMem, hBitmap);
                GetObject(hBitmap, sizeof(bmp), &bmp);
                BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);

                // 当用户点击按钮的时候
                if (lpDrawItemStruct->itemState & ODS_SELECTED)
                {
                    hBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_STARTBTNDOWN));
                    SelectObject(hdcMem, hBitmap);
                    BitBlt(lpDrawItemStruct->hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
                    DeleteObject(hBitmap);
                }
            }
            break;
        }
        DeleteDC(hdcMem);
        return TRUE;

    case WM_LBUTTONDOWN:
        // 按住鼠标左键可以拖动窗口
        SetCursor(LoadCursor(NULL, IDC_HAND));
        SendMessage(hwndDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        SetCursor(LoadCursor(NULL, IDC_ARROW));

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_HOME:
            ShellExecute(NULL, TEXT("open"), TEXT("http://www.WindowsChs.com"), NULL, NULL, SW_SHOW);
            break;

        case IDC_BTN_CLOSE:
            EndDialog(hwndDlg, 0);
            break;

        case IDC_BTN_BROWSE:
            // 浏览文件夹
            break;

        case IDC_BTN_STARTSTOP:
            if (g_bStartScanning)
            {
                g_bStartScanning = !g_bStartScanning;
                InvalidateRect(hwndBtnStartStop, NULL, TRUE);
                MessageBox(hwndDlg, TEXT("已经停止扫描"), TEXT("提示"), MB_OK);
            }
            else
            {
                g_bStartScanning = !g_bStartScanning;
                InvalidateRect(hwndBtnStartStop, NULL, TRUE);
                MessageBox(hwndDlg, TEXT("已经开始扫描"), TEXT("提示"), MB_OK);
            }
            break;
        }
        return TRUE;
    }

    return FALSE;
}