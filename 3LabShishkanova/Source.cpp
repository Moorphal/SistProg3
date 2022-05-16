#pragma comment(lib, "d2d1")
#include <windows.h>
#include <math.h>
#include <d2d1.h>
#include "resource.h"

#define NUM    1000
#define TWOPI  (2 * 3.14159)
#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}

int menuChoice = 0;

ID2D1Factory* g_pD2DFactory = NULL; 
ID2D1HwndRenderTarget* g_pRenderTarget = NULL; 
ID2D1SolidColorBrush* g_pBlackBrush = NULL; 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void CreateD2DResource(HWND hWnd)
{
    if (!g_pRenderTarget)
    {
        HRESULT hr;

        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);

        RECT rc;
        GetClientRect(hWnd, &rc);

        hr = g_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hWnd,
                D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
            ),
            &g_pRenderTarget
        );

        hr = g_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &g_pBlackBrush
        );
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("SineWave");
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);

    hwnd = CreateWindow(szAppName, TEXT("Sine Wave Using Polyline"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    CreateD2DResource(hwnd);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int  cxClient, cyClient;
    HDC         hdc;
    int         i;
    PAINTSTRUCT ps;
    POINT       apt[NUM];

    switch (message)
    {
    case WM_SIZE:
    {
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        return 0;
    }
    case WM_COMMAND:
    {
        InvalidateRect(hwnd, NULL, TRUE);
        int wmid = LOWORD(wParam);
        switch (wmid)
        {
        case ID_FILE_EXIT:
            DestroyWindow(hwnd);
            break;
        case ID_FUNCTIONS_SIN:
            menuChoice = 1;
            break;
        case ID_FUNCTIONS_COS:
            menuChoice = 2;
            break;
        case ID_FUNCTIONS_TAN:
            menuChoice = 3;
            break;
        case ID_FUNCTIONS_CTG:
            menuChoice = 4;
            break;
        }
        UpdateWindow(hwnd);
        break;
    }



    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);

        g_pRenderTarget->BeginDraw();

        g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        g_pRenderTarget->DrawLine(
            D2D1::Point2F(0, cyClient / 2),
            D2D1::Point2F(cxClient, cyClient / 2),
            g_pBlackBrush, 0.5f
        );

        switch (menuChoice)
        {
        case 1:
            for (i = 0; i < NUM; i++)
            {
                g_pRenderTarget->DrawLine(
                    D2D1::Point2F(i * cxClient / NUM, cyClient / 2 * (1 - sin(TWOPI * i / NUM))),
                    D2D1::Point2F((i + 1) * cxClient / NUM, cyClient / 2 * (1 - sin(TWOPI * (i + 1) / NUM))),
                    g_pBlackBrush, 0.5f
                );
            }
            g_pRenderTarget->EndDraw();
            break;
        case 2:
            for (i = 0; i < NUM; i++)
            {
                g_pRenderTarget->DrawLine(
                    D2D1::Point2F(i * cxClient / NUM, cyClient / 2 * (1 - cos(TWOPI * i / NUM))),
                    D2D1::Point2F((i + 1) * cxClient / NUM, cyClient / 2 * (1 - cos(TWOPI * (i + 1) / NUM))),
                    g_pBlackBrush, 0.5f
                );
            }
            g_pRenderTarget->EndDraw();
            break;
        case 3:
            for (i = 0; i < NUM; i++)
            {
                g_pRenderTarget->DrawLine(
                    D2D1::Point2F(i * cxClient / NUM, cyClient / 2 * (1 - tan(TWOPI * i / NUM))),
                    D2D1::Point2F((i + 1) * cxClient / NUM, cyClient / 2 * (1 - tan(TWOPI * (i + 1) / NUM))),
                    g_pBlackBrush, 0.5f
                );
            }
            g_pRenderTarget->EndDraw();
            break;
        case 4:
            for (i = 0; i < NUM; i++)
            {
                g_pRenderTarget->DrawLine(
                    D2D1::Point2F(i * cxClient / NUM, cyClient / 2 * (1 - 1 / tan(TWOPI * i / NUM))),
                    D2D1::Point2F((i + 1) * cxClient / NUM, cyClient / 2 * (1 - 1 / tan(TWOPI * (i + 1) / NUM))),
                    g_pBlackBrush, 0.5f
                );
            }
            g_pRenderTarget->EndDraw();
            break;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}