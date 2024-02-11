#include <windows.h>
#include <vector>
#include "windowsx.h"
#include <tchar.h>

#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hStatic1, hStatic2;
HINSTANCE hInst;
TCHAR szCoordinates[20];


UINT_PTR timerId; // Идентификатор таймера
POINT startCoord; // Начальные координаты при первом клике
POINT endCoord;   // Конечные координаты при отпускании кнопки мыши

bool f = true;
int idStatic = 0;

std::vector<std::vector<int>> staticArray = {};

void UpdateCoordinates(HWND);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
    hInst = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE: {
            EndDialog(hWnd, 0);

            return TRUE;
        }
        case WM_INITDIALOG: {
            hStatic1 = GetDlgItem(hWnd, IDC_STATIC);
            SetWindowText(hWnd, _TEXT("Diolog"));

            return TRUE;
        }
        case WM_LBUTTONDOWN: {
            timerId = SetTimer(hWnd, 1, 10, NULL); // Устанавливаем таймер с интервалом в 10 мс
            startCoord.x = GET_X_LPARAM(lParam);
            startCoord.y = GET_Y_LPARAM(lParam);

            return TRUE;
        }

        /*
        case WM_RBUTTONDOWN: {
            POINTS points = MAKEPOINTS(lParam);
            POINT point;

            point.x = points.x;
            point.y = points.y;

            HWND hClickedStatic = ChildWindowFromPoint(hWnd, point);

            if (hClickedStatic != NULL && hClickedStatic != hWnd) {       
                int uniqueNumber = GetWindowLongPtr(hClickedStatic, GWLP_USERDATA);
                _TCHAR buff[256];

                wsprintf(buff, _TEXT("id: %d. start x: %d, start y: %d. w: %d, h: %d"), uniqueNumber,
                    staticArray[uniqueNumber][0], staticArray[uniqueNumber][1],
                    staticArray[uniqueNumber][2], staticArray[uniqueNumber][3]);
                SetWindowText(hWnd, buff);
            }
            return TRUE;
        }*/
        
        /*
        case WM_RBUTTONDOWN: {
            POINTS points = MAKEPOINTS(lParam);
            POINT point;

            point.x = points.x;
            point.y = points.y;

            HWND hClickedStatic = NULL;
            int maxId = -1;

            // Перебираем все дочерние окна, которые содержат указанную точку
            HWND hChild = ChildWindowFromPointEx(hWnd, point, CWP_ALL);
            while (hChild != NULL && hChild != hWnd) {
                int uniqueNumber = GetWindowLongPtr(hChild, GWLP_USERDATA);
                if (uniqueNumber > maxId) {
                    maxId = uniqueNumber;
                    hClickedStatic = hChild;
                }
                hChild = GetNextWindow(hChild, GW_HWNDNEXT);
            }

            if (hClickedStatic != NULL) {
                _TCHAR buff[256];

                wsprintf(buff, _TEXT("id: %d. start x: %d, start y: %d. w: %d, h: %d"), maxId,
                    staticArray[maxId][0], staticArray[maxId][1],
                    staticArray[maxId][2], staticArray[maxId][3]);
                SetWindowText(hWnd, buff);
            }
            return TRUE;
        }
        */

        case WM_RBUTTONDOWN: {
            POINTS points = MAKEPOINTS(lParam);
            POINT point;

            _TCHAR buff[256];

            point.x = points.x;
            point.y = points.y;

            HWND hClickedStatic = ChildWindowFromPoint(hWnd, point);

            if (hClickedStatic != NULL && hClickedStatic != hWnd) {
                HWND hChild = ChildWindowFromPointEx(hWnd, point, CWP_ALL);
                int uniqueNumber = GetWindowLongPtr(hChild, GWLP_USERDATA);

                wsprintf(buff, _TEXT("id: %d. start x: %d, start y: %d. w: %d, h: %d"), uniqueNumber,
                    staticArray[uniqueNumber][0], staticArray[uniqueNumber][1],
                    staticArray[uniqueNumber][2], staticArray[uniqueNumber][3]);
                SetWindowText(hWnd, buff);
            }
            

            return TRUE;
        }

        case WM_LBUTTONUP: {
            KillTimer(hWnd, timerId); // Убираем таймер
            endCoord.x = GET_X_LPARAM(lParam);
            endCoord.y = GET_Y_LPARAM(lParam);
            
            if (endCoord.x - startCoord.x < 10 || endCoord.y - startCoord.y < 10) {
                SetWindowText(hWnd, _TEXT("Minimum size 10 by 10!"));
                /*MessageBox(0,
                    ,
                    _TEXT("Information"),
                    MB_OK | MB_ICONINFORMATION);*/
            }
            else {
                hStatic2 = CreateWindowEx(0, TEXT("STATIC"), 0,
                    WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
                    (endCoord.x - startCoord.x < 0 ? endCoord.x : startCoord.x),
                    (endCoord.y - startCoord.y < 0 ? endCoord.y : startCoord.y),
                    (endCoord.x - startCoord.x < 0 ? startCoord.x - endCoord.x : endCoord.x - startCoord.x),
                    (endCoord.y - startCoord.y < 0 ? startCoord.y - endCoord.y : endCoord.y - startCoord.y),
                    hWnd, 0, hInst, 0);

                SetWindowLongPtr(hStatic2, GWLP_USERDATA, idStatic++);
                SetWindowText(hStatic2, 0);
                    
                                        // x, y, w, h
                staticArray.push_back({ endCoord.x - startCoord.x < 0 ? endCoord.x : startCoord.x, endCoord.y - startCoord.y < 0 ? endCoord.y : startCoord.y,
                    endCoord.x - startCoord.x < 0 ? startCoord.x - endCoord.x : endCoord.x - startCoord.x, 
                    endCoord.y - startCoord.y < 0 ? startCoord.y - endCoord.y : endCoord.y - startCoord.y });
            }

            return TRUE;
        }
        case WM_LBUTTONDBLCLK: {
            POINTS points = MAKEPOINTS(lParam);
            POINT point;

            point.x = points.x;
            point.y = points.y;

            HWND hClickedStatic = ChildWindowFromPoint(hWnd, point);

            DestroyWindow(hClickedStatic);

            return TRUE;
        }
        case WM_TIMER: {            
            UpdateCoordinates(hWnd); // Вызываем функцию обновления координат по таймеру только при удерживании кнопки
            
            return TRUE;
        }
    }

    return FALSE;
}

void UpdateCoordinates(HWND hWnd) {
    POINT pt;
    WCHAR str[20];

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);    
    
    wsprintf(str, L"x: %d, y: %d", pt.x, pt.y);
    SetWindowText(hStatic1, str);
}
