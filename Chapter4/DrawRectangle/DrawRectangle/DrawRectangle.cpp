#include <Windows.h>
#include <Windowsx.h>

// 函数声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID DrawFrame(HWND hwnd, POINT ptLeftTop, POINT ptRightBottom);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szClassName[] = TEXT("MyWindow");
    TCHAR szAppName[] = TEXT("DrawRectangle");
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
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szClassName, szAppName, WS_OVERLAPPEDWINDOW,
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
    HDC hdc;
    PAINTSTRUCT ps;
    static POINT ptLeftTop, ptRightBottom;  // 矩形的左上角和右下角坐标
    static BOOL bStarting;

    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        //SetCapture(hwnd);
        SetCursor(LoadCursor(NULL, IDC_CROSS));

        // 初始化新矩形的左上角和右下角坐标
        ptLeftTop.x = ptRightBottom.x = GET_X_LPARAM(lParam);
        ptLeftTop.y = ptRightBottom.y = GET_Y_LPARAM(lParam);
        bStarting = TRUE;
        return 0;

    case WM_MOUSEMOVE:
        if (bStarting)
        {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            // 先擦除上一次WM_MOUSEMOVE消息所画的矩形，WM_MOUSEMOVE消息会不断产生
            DrawFrame(hwnd, ptLeftTop, ptRightBottom);

            // 新矩形的右下角坐标
            ptRightBottom.x = GET_X_LPARAM(lParam);
            ptRightBottom.y = GET_Y_LPARAM(lParam);
            // 绘制新矩形
            DrawFrame(hwnd, ptLeftTop, ptRightBottom);
        }
        return 0;

    case WM_LBUTTONUP:
        if (bStarting)
        {
            // 擦除WM_MOUSEMOVE消息中所画的最后一个矩形
            DrawFrame(hwnd, ptLeftTop, ptRightBottom);

            // 最终矩形的右下角坐标
            ptRightBottom.x = GET_X_LPARAM(lParam);
            ptRightBottom.y = GET_Y_LPARAM(lParam);

            SetCursor(LoadCursor(NULL, IDC_ARROW));
            bStarting = FALSE;
            //ReleaseCapture();

            // 绘制最终的矩形
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        Rectangle(hdc, ptLeftTop.x, ptLeftTop.y, ptRightBottom.x, ptRightBottom.y);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

VOID DrawFrame(HWND hwnd, POINT ptLeftTop, POINT ptRightBottom)
{
    HDC hdc = GetDC(hwnd);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    SetROP2(hdc, R2_NOT);
    Rectangle(hdc, ptLeftTop.x, ptLeftTop.y, ptRightBottom.x, ptRightBottom.y);
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    ReleaseDC(hwnd, hdc);
}