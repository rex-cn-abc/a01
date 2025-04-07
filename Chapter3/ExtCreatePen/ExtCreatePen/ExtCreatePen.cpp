#include <Windows.h>
#include <tchar.h>

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("ExtCreatePen");
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
    wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
        100, 100, 500, 330, NULL, NULL, hInstance, NULL);

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
    HPEN hPen;
    LOGBRUSH logBrush;
    int arrEnd[] = { PS_ENDCAP_ROUND , PS_ENDCAP_SQUARE , PS_ENDCAP_FLAT };
    int arrJoin[] = { PS_JOIN_BEVEL , PS_JOIN_MITER , PS_JOIN_ROUND };
    POINT arrPoint[] = { 50,50, 100,200, 150,50 };

    switch (uMsg)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        logBrush.lbStyle = BS_SOLID;
        logBrush.lbColor = RGB(0, 0, 0);
        logBrush.lbHatch = 0;
        for (int i = 0; i < 3; i++)
        {
            // 画黑色40宽的粗线，几何画笔
            hPen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | arrEnd[i] | arrJoin[i], 40, &logBrush, 0, NULL);
            SelectObject(hdc, hPen);
            for (int j = 0; j < _countof(arrPoint); j++)
            {
                if (i > 0)
                    arrPoint[j].x += 150;
            }
            Polyline(hdc, arrPoint, _countof(arrPoint));

            // 画白色细线作为对比，装饰画笔
            DeleteObject(SelectObject(hdc, GetStockObject(WHITE_PEN)));
            Polyline(hdc, arrPoint, _countof(arrPoint));
            SelectObject(hdc, GetStockObject(BLACK_PEN));
        }
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}