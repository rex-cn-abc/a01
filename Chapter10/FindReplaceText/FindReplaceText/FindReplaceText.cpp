#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ȫ�ֱ���
HWND g_hwndFind;    // ��Ϣѭ���л��õ�������ȫ�ֱ���
HWND g_hwndReplace;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("FindReplaceText");
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
    wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if ((g_hwndFind == NULL || !IsDialogMessage(g_hwndFind, &msg)) &&
            (g_hwndReplace == NULL || !IsDialogMessage(g_hwndReplace, &msg)))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static UINT WM_FINDREPLACE;
    static TCHAR szFindWhat[80] = { 0 };
    static TCHAR szReplaceWith[80] = { 0 };
    static FINDREPLACE fr = { 0 };

    LPFINDREPLACE lpfr;
    TCHAR szBuf[256] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        // ��ȡ�����滻��Ϣ��ID
        WM_FINDREPLACE = RegisterWindowMessage(FINDMSGSTRING);

        // ��ʼ��FINDREPLACE�ṹ���ýṹֻӦ�ó�ʼ��һ��
        fr.lStructSize = sizeof(FINDREPLACE);
        fr.hwndOwner = hwnd;
        fr.lpstrFindWhat = szFindWhat;
        fr.lpstrReplaceWith = szReplaceWith;
        fr.wFindWhatLen = sizeof(szFindWhat);
        fr.wReplaceWithLen = sizeof(szReplaceWith);
        //fr.Flags = FR_DOWN | FR_MATCHCASE;
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_EDIT_FIND:
            if (g_hwndFind == NULL && g_hwndReplace == NULL)
            {
                // &���ϴιرնԻ���ʱ���õ�FR_DIALOGTERM��־ֵ
                fr.Flags &= ~FR_DIALOGTERM;
                g_hwndFind = FindText(&fr);
            }
            break;

        case ID_EDIT_REPLACE:
            if (g_hwndReplace == NULL && g_hwndFind == NULL)
            {
                // &���ϴιرնԻ���ʱ���õ�FR_DIALOGTERM��־ֵ
                fr.Flags &= ~FR_DIALOGTERM;
                g_hwndReplace = ReplaceText(&fr);
            }
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////
    if (uMsg == WM_FINDREPLACE)
    {
        lpfr = (LPFINDREPLACE)lParam;
        // �رնԻ���
        if (lpfr->Flags & FR_DIALOGTERM)
        {
            g_hwndFind = NULL;
            g_hwndReplace = NULL;
        }

        // ������һ��
        else if (lpfr->Flags & FR_FINDNEXT)
        {
            StringCchCopy(szBuf, _countof(szBuf), TEXT("�����ˡ�������һ������ť"));
            if (lpfr->Flags & FR_DOWN)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ����¡���ѡ��ť"));
            else
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ����ϡ���ѡ��ť"));
            if (lpfr->Flags & FR_MATCHCASE)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ�ƥ���Сд����ѡ��"));
            if (lpfr->Flags & FR_WHOLEWORD)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ�ȫ��ƥ�䡱��ѡ��"));

            StringCchCat(szBuf, _countof(szBuf), TEXT("\n\n�������ݣ�"));
            StringCchCat(szBuf, _countof(szBuf), fr.lpstrFindWhat);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
        }

        // �滻
        else if (lpfr->Flags & FR_REPLACE)
        {
            StringCchCopy(szBuf, _countof(szBuf), TEXT("�����ˡ��滻����ť"));
            if (lpfr->Flags & FR_DOWN)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ����¡���ѡ��ť"));
            else
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ����ϡ���ѡ��ť"));
            if (lpfr->Flags & FR_MATCHCASE)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ�ƥ���Сд����ѡ��"));
            if (lpfr->Flags & FR_WHOLEWORD)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ�ȫ��ƥ�䡱��ѡ��"));

            StringCchCat(szBuf, _countof(szBuf), TEXT("\n\n�������ݣ�"));
            StringCchCat(szBuf, _countof(szBuf), fr.lpstrFindWhat);
            StringCchCat(szBuf, _countof(szBuf), TEXT("\n�滻���ݣ�"));
            StringCchCat(szBuf, _countof(szBuf), fr.lpstrReplaceWith);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
        }

        // ȫ���滻
        else if (lpfr->Flags & FR_REPLACEALL)
        {
            StringCchCopy(szBuf, _countof(szBuf), TEXT("�����ˡ�ȫ���滻����ť"));
            if (lpfr->Flags & FR_DOWN)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ����¡���ѡ��ť"));
            else
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ����ϡ���ѡ��ť"));
            if (lpfr->Flags & FR_MATCHCASE)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ�ƥ���Сд����ѡ��"));
            if (lpfr->Flags & FR_WHOLEWORD)
                StringCchCat(szBuf, _countof(szBuf), TEXT("\nѡ���ˡ�ȫ��ƥ�䡱��ѡ��"));

            StringCchCat(szBuf, _countof(szBuf), TEXT("\n\n�������ݣ�"));
            StringCchCat(szBuf, _countof(szBuf), fr.lpstrFindWhat);
            StringCchCat(szBuf, _countof(szBuf), TEXT("\n�滻���ݣ�"));
            StringCchCat(szBuf, _countof(szBuf), fr.lpstrReplaceWith);
            MessageBox(hwnd, szBuf, TEXT("��ʾ"), MB_OK);
        }

        return 0;
    }
    //////////////////////////////////////////////////////////////////////////

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}