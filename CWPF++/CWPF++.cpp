#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "View.h"
#include "CommonWidgets.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    //const wchar_t CLASS_NAME[] = L"Sample Window Class";

    //WNDCLASS wc = { };

    //wc.lpfnWndProc = WindowProc;
    //wc.hInstance = hInstance;
    //wc.lpszClassName = CLASS_NAME;

    //RegisterClass(&wc);

    //// Create the window.

    //HWND hwnd = CreateWindowEx(
    //    0,                              // Optional window styles.
    //    CLASS_NAME,                     // Window class
    //    L"Learn to Program Windows",    // Window text
    //    WS_OVERLAPPEDWINDOW,            // Window style

    //    // Size and position
    //    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    //    NULL,       // Parent window    
    //    NULL,       // Menu
    //    hInstance,  // Instance handle
    //    NULL        // Additional application data
    //);

    //if (hwnd == NULL)
    //{
    //    return 0;
    //}

    //ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    CWPF::RegisterCommonWidgets();

    CWPF::View v(CWPF::ViewInitArgs{ L"MainWindow.xml" });
    v.Show();

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_CREATE:
    {
        HFONT hfDefault;
        HWND hEdit;

        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            0, 0, 100, 100, hwnd, NULL, GetModuleHandle(NULL), NULL);
    }
    break;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}