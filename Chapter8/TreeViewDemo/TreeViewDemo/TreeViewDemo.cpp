#include <Windows.h>
#include <CommCtrl.h>
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
    static HWND hwndTreeView;               // ����ͼ�ؼ�
    static HTREEITEM htrBook, htrWorld;     // ���ڵ㣺ͼ�顢����
    static HTREEITEM htrAmerican, htrChina; // ���磺�������й�

    static HIMAGELIST hImagList;
    HBITMAP    hbmImage;

    TVINSERTSTRUCT tvi = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // ��������ͼ�ؼ�
        hwndTreeView = CreateWindowEx(0, TEXT("SysTreeView32"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | 
            TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |
            TVS_EDITLABELS | TVS_SHOWSELALWAYS | TVS_TRACKSELECT,
            10, 0, 200, 200, hwnd, (HMENU)IDC_TREEVIEW, hInstance, NULL);

        // ͼ���б�
        hImagList = ImageList_Create(24, 24, ILC_COLOR32, 8, 0);
        for (int i = 0; i < 8; i++)
        {
            // ��Դ�ļ��ж�����8��λͼ
            hbmImage = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BMP_BOOK + i),
                IMAGE_BITMAP, 24, 24, 0);
            ImageList_Add(hImagList, hbmImage, NULL);
            DeleteObject(hbmImage);
        }
        SendMessage(hwndTreeView, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImagList);

        // Ϊ����ͼ�ؼ������Ŀ
        // ���ڵ㣺ͼ�顢����
        tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.hInsertAfter = TVI_LAST;

        tvi.hParent = TVI_ROOT;
        tvi.item.pszText = TEXT("ͼ��"); tvi.item.iImage = tvi.item.iSelectedImage = 0;
        htrBook = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("����"); tvi.item.iImage = tvi.item.iSelectedImage = 1;
        htrWorld = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // ͼ��(ƽ����ơ��������)
        tvi.hParent = htrBook;
        tvi.item.pszText = TEXT("ƽ�����"); tvi.item.iImage = tvi.item.iSelectedImage = 2;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("�������"); tvi.item.iImage = tvi.item.iSelectedImage = 3;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // ����(�������й�)
        tvi.hParent = htrWorld;
        tvi.item.pszText = TEXT("����"); tvi.item.iImage = tvi.item.iSelectedImage = 4;
        htrAmerican = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("�й�"); tvi.item.iImage = tvi.item.iSelectedImage = 5;
        htrChina = (HTREEITEM)SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);

        // �й�(�����С��Ϻ���)
        tvi.hParent = htrChina;
        tvi.item.pszText = TEXT("������"); tvi.item.iImage = tvi.item.iSelectedImage = 6;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        tvi.item.pszText = TEXT("�Ϻ���"); tvi.item.iImage = tvi.item.iSelectedImage = 7;
        SendMessage(hwndTreeView, TVM_INSERTITEM, 0, (LPARAM)&tvi);
        return 0;

    case WM_DESTROY:
        ImageList_Destroy(hImagList);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}