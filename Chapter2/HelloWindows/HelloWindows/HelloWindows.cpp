#include <Windows.h>
#include <tchar.h>                  // _tcslen������Ҫ��ͷ�ļ�

#pragma comment(lib, "Winmm.lib")   // ����������PlaySound������ҪWinmm�����

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;                        // RegisterClassEx�����õ�WNDCLASSEX�ṹ
    TCHAR szClassName[] = TEXT("MyWindow");     // RegisterClassEx����ע��Ĵ����������
    TCHAR szAppName[] = TEXT("HelloWindows");   // ���ڱ���
    HWND hwnd;                                  // CreateWindowEx���������Ĵ��ڵľ��
    MSG msg;                                    // ��Ϣѭ�����õ���Ϣ�ṹ��

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
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 180, NULL, NULL, hInstance, NULL);

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
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR szStr[] = TEXT("��ã�Windows�������");

    switch (uMsg)
    {
    case WM_CREATE:
        PlaySound(TEXT("�ɶ�(�����).wav"), NULL, SND_FILENAME | SND_ASYNC/* | SND_LOOP*/);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        TextOut(hdc, 10, 10, szStr, _tcslen(szStr));
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}