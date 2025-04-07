#include <Windows.h>
#include <tchar.h>

// �������������ڹ���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wndclass;
    TCHAR szAppName[] = TEXT("����ռ䵽ҳ��ռ�ı任");
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
    // ��ͼ��ģʽ����Ϊ�߼�ͼ��ģʽ
    SetGraphicsMode(hdc, GM_ADVANCED);
    // ��ӳ��ģʽ����ΪMM_LOMETRIC����0.1����Ϊ��λ����������Ժ����
    SetMapMode(hdc, MM_LOMETRIC);

    switch (iTransform)
    {
    case NORMAL:      // ���
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case TRANSLATE:   // ����ƽ��
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)200.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case SCALE:        // ���ŵ�ԭʼ��С��1/2
        xForm.eM11 = (FLOAT)0.5;
        xForm.eM12 = (FLOAT)0.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)0.5;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case ROTATE:      // ��ʱ����ת30��
        xForm.eM11 = (FLOAT)0.8660;
        xForm.eM12 = (FLOAT)0.5000;
        xForm.eM21 = (FLOAT)-0.5000;
        xForm.eM22 = (FLOAT)0.8660;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case SHEAR:       // ��б����
        xForm.eM11 = (FLOAT)1.0;
        xForm.eM12 = (FLOAT)1.0;
        xForm.eM21 = (FLOAT)0.0;
        xForm.eM22 = (FLOAT)1.0;
        xForm.eDx = (FLOAT)0.0;
        xForm.eDy = (FLOAT)0.0;
        SetWorldTransform(hdc, &xForm);
        break;

    case REFLECT:     // ��X�᾵��
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
    // �豸����ת��Ϊ�߼����꣬�˴��߼���λΪ0.1����
    DPtoLP(hdc, (LPPOINT)&rect, 2);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // ���ǵðɣ�����Ļ�ͼ������ʹ���߼���λ
    // ����Բ
    Ellipse(hdc, (rect.right / 2 - 300), (rect.bottom / 2 + 300),
        (rect.right / 2 + 300), (rect.bottom / 2 - 300));
    // ����Բ
    Ellipse(hdc, (rect.right / 2 - 270), (rect.bottom / 2 + 270),
        (rect.right / 2 + 270), (rect.bottom / 2 - 270));

    // ��С����
    Rectangle(hdc, (rect.right / 2 - 20), (rect.bottom / 2 + 360),
        (rect.right / 2 + 20), (rect.bottom / 2 + 210));

    // ��ˮƽ��
    MoveToEx(hdc, (rect.right / 2 - 400), (rect.bottom / 2 + 0), NULL);
    LineTo(hdc, (rect.right / 2 + 400), (rect.bottom / 2 + 0));

    // ����ֱ��
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
        // �����β��ԣ�NORMAL, TRANSLATE, SCALE, ROTATE, SHEAR, REFLECT
        TransformAndDraw(NORMAL, hwnd);
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}