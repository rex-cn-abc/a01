#include <windows.h>
#include <Shlobj.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 全局变量
HINSTANCE g_hInstance;

// 函数声明
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    // 创建模态对话框
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndList;
    PIDLIST_ABSOLUTE pItemIdList;   // SHBrowseForFolder函数返回值
    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hwndDlg;
    bi.lpszTitle = TEXT("请选择一个文件夹");
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

    TCHAR szDir[MAX_PATH] = { 0 };  // SHGetPathFromIDList函数返回的目录名称的缓冲区
    HANDLE hFindFile;
    WIN32_FIND_DATA fd = { 0 };
    TCHAR szSearch[MAX_PATH] = { 0 };
    TCHAR szDirFile[MAX_PATH] = { 0 };

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hwndList = GetDlgItem(hwndDlg, IDC_LIST_FILENAME);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_BROWSE:
            pItemIdList = SHBrowseForFolder(&bi);
            if (pItemIdList)
            {
                // 用户选择的文件夹名称显示到静态控件中
                SHGetPathFromIDList(pItemIdList, szDir);
                SetDlgItemText(hwndDlg, IDC_STATIC_DIR, szDir);

                if (szDir[_tcslen(szDir) - 1] != TEXT('\\'))
                    StringCchCat(szDir, _countof(szDir), TEXT("\\"));
                // 拼接搜索字符串
                StringCchCopy(szSearch, _countof(szSearch), szDir);
                StringCchCat(szSearch, _countof(szSearch), TEXT("*.*"));
                // 遍历目录
                hFindFile = FindFirstFile(szSearch, &fd);
                if (hFindFile != INVALID_HANDLE_VALUE)
                {
                    // 先清空列表框
                    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

                    do
                    {
                        // 处理本次找到的各种文件，没有处理找到的目录
                        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            StringCchCopy(szDirFile, _countof(szDirFile), szDir);
                            StringCchCat(szDirFile, _countof(szDirFile), fd.cFileName);
                            SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)szDirFile);
                        }
                    } while (FindNextFile(hFindFile, &fd));

                    // 关闭查找句柄
                    FindClose(hFindFile);
                }
            }
            break;

        case IDCANCEL:
            EndDialog(hwndDlg, 0);
            break;
        }
        return TRUE;
    }

    return FALSE;
}