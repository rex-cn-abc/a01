#include <windows.h>
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
    TCHAR szFile[MAX_PATH * 512] = { 0 };   // 返回用户选择的文件名的缓冲区大一点，本程序允许多选
    TCHAR szFileTitle[MAX_PATH] = { 0 };    // 返回用户所选文件的文件名和扩展名的缓冲区

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndDlg;
    ofn.lpstrFilter =
        TEXT("文本文件(*.txt, *.doc, *.docx)\0*.txt;*.doc;*.docx\0All(*.*)\0*.*\0");
    ofn.nFilterIndex = 1;                       // 默认选择第1个过滤器
    ofn.lpstrFile = szFile;                     // 返回用户选择的文件名的缓冲区
    ofn.lpstrFile[0] = NULL;                    // 不需要初始化文件名编辑控件
    ofn.nMaxFile = _countof(szFile);
    ofn.lpstrFileTitle = szFileTitle;	        // 返回用户选择的文件的文件名和扩展名的缓冲区
    ofn.nMaxFileTitle = _countof(szFileTitle);
    ofn.lpstrInitialDir = TEXT("C:\\");         // 初始目录

    LPTSTR lpStr;
    TCHAR szDir[MAX_PATH] = { 0 };
    TCHAR szBuf[MAX_PATH] = { 0 };

    switch (uMsg)
    {
    case WM_INITDIALOG:
        hwndList = GetDlgItem(hwndDlg, IDC_LIST_FILENAME);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BTN_OPEN:
            ofn.lpstrTitle = TEXT("请选择要打开的文件");// 对话框标题栏中显示的字符串
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT;
            if (GetOpenFileName(&ofn))
            {
                // 先清空列表框
                SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

                lpStr = ofn.lpstrFile + _tcslen(ofn.lpstrFile) + 1;
                if (lpStr[0] == NULL)
                {
                    // 用户只选择了一个文件
                    SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)ofn.lpstrFile);
                }
                else
                {
                    // 用户选择了多个文件
                    StringCchCopy(szDir, _countof(szDir), ofn.lpstrFile);
                    if (szDir[_tcslen(szDir) - 1] != TEXT('\\'))
                        StringCchCat(szDir, _countof(szDir), TEXT("\\"));
                    while (lpStr[0] != NULL)
                    {
                        StringCchCopy(szBuf, _countof(szBuf), szDir);
                        StringCchCat(szBuf, _countof(szBuf), lpStr);
                        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)szBuf);
                        lpStr += _tcslen(lpStr) + 1;
                    }
                }
            }
            break;

        case IDC_BTN_SAVE:
            ofn.lpstrTitle = TEXT("请选择要保存的文件名");  // 对话框标题栏中显示的字符串
            ofn.lpstrDefExt = TEXT("txt");                  // 默认扩展名
            ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;
            if (GetSaveFileName(&ofn))
            {
                SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
                SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)ofn.lpstrFile);
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