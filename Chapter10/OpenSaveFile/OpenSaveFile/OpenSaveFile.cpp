#include <windows.h>
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
    TCHAR szFile[MAX_PATH * 512] = { 0 };   // �����û�ѡ����ļ����Ļ�������һ�㣬�����������ѡ
    TCHAR szFileTitle[MAX_PATH] = { 0 };    // �����û���ѡ�ļ����ļ�������չ���Ļ�����

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndDlg;
    ofn.lpstrFilter =
        TEXT("�ı��ļ�(*.txt, *.doc, *.docx)\0*.txt;*.doc;*.docx\0All(*.*)\0*.*\0");
    ofn.nFilterIndex = 1;                       // Ĭ��ѡ���1��������
    ofn.lpstrFile = szFile;                     // �����û�ѡ����ļ����Ļ�����
    ofn.lpstrFile[0] = NULL;                    // ����Ҫ��ʼ���ļ����༭�ؼ�
    ofn.nMaxFile = _countof(szFile);
    ofn.lpstrFileTitle = szFileTitle;	        // �����û�ѡ����ļ����ļ�������չ���Ļ�����
    ofn.nMaxFileTitle = _countof(szFileTitle);
    ofn.lpstrInitialDir = TEXT("C:\\");         // ��ʼĿ¼

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
            ofn.lpstrTitle = TEXT("��ѡ��Ҫ�򿪵��ļ�");// �Ի������������ʾ���ַ���
            ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT;
            if (GetOpenFileName(&ofn))
            {
                // ������б��
                SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

                lpStr = ofn.lpstrFile + _tcslen(ofn.lpstrFile) + 1;
                if (lpStr[0] == NULL)
                {
                    // �û�ֻѡ����һ���ļ�
                    SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)ofn.lpstrFile);
                }
                else
                {
                    // �û�ѡ���˶���ļ�
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
            ofn.lpstrTitle = TEXT("��ѡ��Ҫ������ļ���");  // �Ի������������ʾ���ַ���
            ofn.lpstrDefExt = TEXT("txt");                  // Ĭ����չ��
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