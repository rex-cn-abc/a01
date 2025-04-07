#include <windows.h>
#include <Shlobj.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ȫ�ֱ���
HINSTANCE g_hInstance;

// ��������
INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    // ����ģ̬�Ի���
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, NULL);
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndList;
    PIDLIST_ABSOLUTE pItemIdList;   // SHBrowseForFolder��������ֵ
    BROWSEINFO bi = { 0 };
    bi.hwndOwner = hwndDlg;
    bi.lpszTitle = TEXT("��ѡ��һ���ļ���");
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

    TCHAR szDir[MAX_PATH] = { 0 };  // SHGetPathFromIDList�������ص�Ŀ¼���ƵĻ�����
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
                // �û�ѡ����ļ���������ʾ����̬�ؼ���
                SHGetPathFromIDList(pItemIdList, szDir);
                SetDlgItemText(hwndDlg, IDC_STATIC_DIR, szDir);

                if (szDir[_tcslen(szDir) - 1] != TEXT('\\'))
                    StringCchCat(szDir, _countof(szDir), TEXT("\\"));
                // ƴ�������ַ���
                StringCchCopy(szSearch, _countof(szSearch), szDir);
                StringCchCat(szSearch, _countof(szSearch), TEXT("*.*"));
                // ����Ŀ¼
                hFindFile = FindFirstFile(szSearch, &fd);
                if (hFindFile != INVALID_HANDLE_VALUE)
                {
                    // ������б��
                    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

                    do
                    {
                        // �������ҵ��ĸ����ļ���û�д����ҵ���Ŀ¼
                        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            StringCchCopy(szDirFile, _countof(szDirFile), szDir);
                            StringCchCat(szDirFile, _countof(szDirFile), fd.cFileName);
                            SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)szDirFile);
                        }
                    } while (FindNextFile(hFindFile, &fd));

                    // �رղ��Ҿ��
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