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
    TCHAR szAppName[] = TEXT("TreeViewDemo");
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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 280, NULL, NULL, hInstance, NULL);

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
    static HWND hwndTreeView;               // 树视图控件
    static HTREEITEM htrBook, htrWorld;     // 根节点：图书、世界
    static HTREEITEM htrAmerican, htrChina; // 世界：美国、中国
    HWND hwndStatic;
    static HFONT hFont;

    static HIMAGELIST hImagList;
    HBITMAP    hbmImage;

    TVINSERTSTRUCT tvi = { 0 };

    LPNMTREEVIEW pnmTV;
    TCHAR szBuf[64] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // 创建树视图控件
        hwndTreeView = CreateWindowEx(0, TEXT("SysTreeView32"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | 
            TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |
            TVS_EDITLABELS | TVS_SHOWSELALWAYS | TVS_TRACKSELECT,
            10, 0, 200, 200, hwnd, (HMENU)IDC_TREEVIEW, hInstance, NULL);

        // 静态控件
        hwndStatic = CreateWindowEx(0, TEXT("Static"), TEXT("已选中："),
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            10, 210, 200, 20, hwnd, (HMENU)IDC_STATICSELECTED, hInstance, NULL);

        // 图像列表
        hImagList = ImageList_Create(24, 24, ILC_COLOR32, 8, 0);
        for (int i = 0; i < 8; i++)
        {
            // 资源文件中定义了8个位图
            hbmImage = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_BOOK + i),
                IMAGE_BITMAP, 24, 24, 0);
            ImageList_Add(hImagList, hbmImage, NULL);
            DeleteObject(hbmImage);
        }
        SendMessage(hwndTreeView, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImagList);

        // 为树视图控件添加项目
        // 根节点：图书、世界
        tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.hInsertAfter = TVI_LAST;

        tvi.hParent = TVI_ROOT;
        tvi.item.pszText = TEXT("图书"); tvi.item.iImage = tvi.item.iSelectedImage = 0;
        htrBook = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("世界"); tvi.item.iImage = tvi.item.iSelectedImage = 1;
        htrWorld = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // 图书(平面设计、程序设计)
        tvi.hParent = htrBook;
        tvi.item.pszText = TEXT("平面设计"); tvi.item.iImage = tvi.item.iSelectedImage = 2;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("程序设计"); tvi.item.iImage = tvi.item.iSelectedImage = 3;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // 世界(美国、中国)
        tvi.hParent = htrWorld;
        tvi.item.pszText = TEXT("美国"); tvi.item.iImage = tvi.item.iSelectedImage = 4;
        htrAmerican = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("中国"); tvi.item.iImage = tvi.item.iSelectedImage = 5;
        htrChina = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // 中国(北京市、上海市)
        tvi.hParent = htrChina;
        tvi.item.pszText = TEXT("北京市"); tvi.item.iImage = tvi.item.iSelectedImage = 6;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("上海市"); tvi.item.iImage = tvi.item.iSelectedImage = 7;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // 设置静态控件字体
        hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));
        SendMessage(hwndStatic, WM_SETFONT, (WPARAM)hFont, FALSE);
        return 0;

    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case TVN_ENDLABELEDIT:
            return TRUE;

        case TVN_SELCHANGED:
            pnmTV = (LPNMTREEVIEW)lParam;
            wsprintf(szBuf, TEXT("项目句柄：0x%X"), (INT)(pnmTV->itemNew.hItem));
            SetDlgItemText(hwnd, IDC_STATICSELECTED, szBuf);// 选中的项目句柄显示到静态控件中
            break;

        case NM_RCLICK:
            MessageBox(hwnd, TEXT("用户鼠标右键单击列表项"), TEXT("提示"), MB_OK);
            break;
        }
        return 0;

    case WM_CTLCOLORSTATIC:
        return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

    case WM_DESTROY:
        ImageList_Destroy(hImagList);
        DeleteObject(hFont);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}