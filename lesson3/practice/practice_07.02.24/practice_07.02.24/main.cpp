#include "Windows.h"
#include "tchar.h"
#include <string>
using std::wstring;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void KeyUpHandler(HWND hwnd, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    WCHAR className[] = L"FirstWindow";

    WNDCLASSEX wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wndClass.hInstance = hInstance;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hIconSm = NULL;
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = className;

    wndClass.lpfnWndProc = WindowProc;

    if (!RegisterClassEx(&wndClass)) {
        return 0;
    }

    HWND hWnd = CreateWindowEx(
        0,
        className,
        L"My first window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, //x,y
        CW_USEDEFAULT, CW_USEDEFAULT, //width, height
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYUP:
        KeyUpHandler(hwnd, wParam, lParam);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int counterV = 0;
int counterH = 0;
int sides = 0;
void KeyUpHandler(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    int vkey = wParam;

    bool f = true;
    bool v = true;
    bool h = false;

    int sizeWidthWindow = 300;
    int sizeHeightWindow = 300;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (vkey == VK_RETURN) {
        do {
            if (counterV != (screenWidth - sizeWidthWindow) && sides == 0) {
                SetWindowPos(hwnd, NULL, counterV++, counterH, sizeWidthWindow, sizeHeightWindow, SWP_NOZORDER);
            }
            else if (counterV == (screenWidth - sizeWidthWindow) && sides == 0) {
                sides++;
            }


            if (counterH != (screenHeight - sizeHeightWindow) - 40 && sides == 1) {
                SetWindowPos(hwnd, NULL, counterV, counterH++, sizeWidthWindow, sizeHeightWindow, SWP_NOZORDER);
            }
            else if (counterH == (screenHeight - sizeHeightWindow) - 40 && sides == 1) {
                sides++;
            }


            if (counterV != 0 && sides == 2) {
                SetWindowPos(hwnd, NULL, counterV--, counterH, sizeWidthWindow, sizeHeightWindow, SWP_NOZORDER);
            }
            else if (counterV == 0 && sides == 2) {
                sides++;
            }

            if (counterH != 0 && sides == 3) {
                SetWindowPos(hwnd, NULL, counterV, counterH--, sizeWidthWindow, sizeHeightWindow, SWP_NOZORDER);
            }
            else if (counterV == 0 && sides == 3) {
                sides = 0;
            }

            if (GetAsyncKeyState(VK_ESCAPE)) {
                f = false; // Выход из цикла при нажатии VK_ESCAPE
            }
        } while (f);
    }
}
