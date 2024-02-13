#include <Windows.h>
#include <tchar.h>
#include "resource.h"

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HWND hwndStatic = GetDlgItem(hwnd, IDOK);
	_TCHAR buff[256];

	switch (uMsg) {
	case WM_INITDIALOG: {
		

		return TRUE;
	}

	case WM_MOUSEMOVE: {
		POINTS points = MAKEPOINTS(lParam);
		POINT point;

		RECT rect;
		POINT pt;

		point.x = points.x;
		point.y = points.y;

		GetWindowRect(hwndStatic, &rect);

		int xLeft = pt.x = rect.left;
		int yTop = pt.y = rect.top;
		int xRight = pt.x = rect.right;
		int yBottom = pt.y = rect.bottom;

		int range = 10;

		ScreenToClient(hwnd, &pt);
		

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		wsprintf(buff, _TEXT("x: %d, y: %d. RxLeft: %d, RyTop: %d. RxRight: %d, RyBottom: %d"), point.x, point.y, (xLeft - range), (yTop - range), (xRight + range), (yBottom + range));
		//wsprintf(buff, _TEXT("x %d , y %d"), width, height);
		SetWindowText(hwnd, buff);

		/*if ((xLeft - range) >= point.x && (yTop - range) >= point.y ||
			(xRight + range) >= point.x && (yBottom + range) >= point.y) {
			DestroyWindow(hwndStatic);
		}*/

		return TRUE;
	}
	case WM_LBUTTONDOWN: {
		
		return TRUE;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);

		return TRUE;
	}

	return FALSE;
}