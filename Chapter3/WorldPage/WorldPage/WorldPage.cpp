#include <Windows.h>
#include <tchar.h>

// 函数声明，窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("世界空间到页面空间的变换");
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
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = NULL;
    RegisterClassEx(&wndclass);

    hwnd = CreateWindowEx(0, szAppName, szAppName, WS_OVERLAPPEDWINDOW,
        100, 100, 500, 400, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

enum MyEnum
{
    NORMAL, TRANSLATE, SCALE, ROTATE, SHEAR, REFLECT,
};

void TransformAndDraw(int iTransform, HWND hWnd)
{
    HDC hdc;
    XFORM xForm;
    RECT rect;

    hdc = GetDC(hWnd);
    // 将图形模式设置为高级图形模式
    SetGraphicsMode(hdc, GM_ADVANCED);
    // 将映射模式设置为MM_LOMETRIC，以0.1毫米为单位，这个函数稍后介绍
    SetMapMode(hdc, MM_LOMETRIC);

    switch (iTransform)
    {
    case NORMAL:      // 相等
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case TRANSLATE:   // 向右平移
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)200.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case SCALE:        // 缩放到原始大小的1/2
        xForm.eM11 = (FLOAT)0.5;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)0.5;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case ROTATE:      // 逆时针旋转30度
        xForm.eM11 = (FLOAT)0.8660;
        xForm.eM12 = (FLOAT)0.5000;
        xForm.eM21 = (FLOAT)-0.5000;
        xForm.eM22 = (FLOAT)0.8660;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case SHEAR:       // 倾斜变形
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)1.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case REFLECT:     // 沿X轴镜像
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)-1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;
    }

    GetClientRect(hWnd, (LPRECT)&rect);
    // 设备坐标转换为逻辑坐标，此处逻辑单位为0.1毫米
    DPtoLP(hdc, (LPPOINT)&rect, 2);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // 还记得吧，下面的绘图函数均使用逻辑单位
    // 画外圆
    Ellipse(hdc, (rect.right / 2 - 300), (rect.bottom / 2 + 300),
        (rect.right / 2 + 300), (rect.bottom / 2 - 300));
    // 画内圆
    Ellipse(hdc, (rect.right / 2 - 270), (rect.bottom / 2 + 270),
        (rect.right / 2 + 270), (rect.bottom / 2 - 270));

    // 画小矩形
    Rectangle(hdc, (rect.right / 2 - 20), (rect.bottom / 2 + 360),
        (rect.right / 2 + 20), (rect.bottom / 2 + 210));

    // 画水平线
    MoveToEx(hdc, (rect.right / 2 - 400), (rect.bottom / 2 + 0), NULL);
    LineTo(hdc, (rect.right / 2 + 400), (rect.bottom / 2 + 0));

    // 画垂直线
    MoveToEx(hdc, (rect.right / 2 + 0), (rect.bottom / 2 + 400), NULL);
    LineTo(hdc, (rect.right / 2 + 0), (rect.bottom / 2 - 400));
    ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // 请依次测试：NORMAL, TRANSLATE, SCALE, ROTATE, SHEAR, REFLECT
        TransformAndDraw(NORMAL, hwnd);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}