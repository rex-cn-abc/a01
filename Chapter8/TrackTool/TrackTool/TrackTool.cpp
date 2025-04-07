#include <Windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ��������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("TrackTool");
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
    HINSTANCE hInstance;
    static HWND hwndTip;
    static BOOL bTracking = FALSE;
    static int oldX, oldY;
    int newX, newY;
    static TOOLINFO ti = { sizeof(TOOLINFO) };
    TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
    POINT pt;
    TCHAR szBuf[24] = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        // ����������ʾ�ؼ�
        hwndTip = CreateWindowEx(WS_EX_TOPMOST, TEXT("tooltips_class32"), NULL,
            WS_POPUP | TTS_ALWAYSTIP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            hwnd, NULL, hInstance, NULL);

        // ��Ӹ��ٹ��ߣ��ͻ�����
        ti.uFlags = TTF_TRACK | TTF_ABSOLUTE;
        ti.hwnd = hwnd;
        ti.uId = 1001;
        GetClientRect(hwnd, &ti.rect);
        SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&ti);
        return 0;

    case WM_MOUSEMOVE:
        // �������ͨ������TrackMouseEvent������ϵͳ��������������Ϣ��
        // ���������ͣ�ڿͻ���һ��ʱ�����WM_MOUSEHOVER��Ϣ��
        // ������뿪�ͻ���ʱ����WM_MOUSELEAVE��Ϣ��
        if (!bTracking)
        {
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);

            bTracking = TRUE;
        }

        // ������ٹ�����ʾ
        SendMessage(hwndTip, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&ti);

        newX = GET_X_LPARAM(lParam);
        newY = GET_Y_LPARAM(lParam);
        if ((newX != oldX) || (newY != oldY))
        {
            oldX = newX;
            oldY = newY;

            // ���ġ����ߡ�����ʾ�ı�
            wsprintf(szBuf, TEXT("���Ŀͻ������꣺%d, %d"), newX, newY);
            ti.lpszText = szBuf;
            SendMessage(hwndTip, TTM_SETTOOLINFO, 0, (LPARAM)&ti);

            // �ƶ����ٹ�����ʾ��λ��
            pt = { newX, newY };
            ClientToScreen(hwnd, &pt);
            SendMessage(hwndTip, TTM_TRACKPOSITION, 0, MAKELPARAM(pt.x, pt.y));
        }
        return 0;

    case WM_MOUSELEAVE:
        // ͣ�ø��ٹ�����ʾ
        SendMessage(hwndTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);
        bTracking = FALSE;
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}