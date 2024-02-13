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
		RECT rect;
		GetWindowRect(hwndStatic, &rect);

		POINT ptTopLeft = { rect.left, rect.top };
		POINT ptBottomRight = { rect.right, rect.bottom };
		POINTS points = MAKEPOINTS(lParam);
		POINT point;

		point.x = points.x;
		point.y = points.y;

		ScreenToClient(hwnd, &ptTopLeft);
		ScreenToClient(hwnd, &ptBottomRight);

		int yTop = ptTopLeft.y;
		int xLeft = ptTopLeft.x;
		int yBottom = ptBottomRight.y;
		int xRight = ptBottomRight.x;

		int range = 10;

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		wsprintf(buff, _TEXT("x: %d, y: %d. top: %d, left: %d, bottom: %d, right: %d"), 
					point.x, point.y, yTop - range, xLeft - range, yBottom + range, xRight + range);
		SetWindowText(hwnd, buff);

		if ((xLeft - range) <= point.x && (yTop - range) <= point.y &&
			(xRight + range) >= point.x && (yBottom + range) >= point.y) {
			// DestroyWindow(hwndStatic);
			
			int count = rand() % 5 + 0;

			if (count == 0)
				SetWindowPos(hwndStatic, NULL, 100, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (count == 1)
				SetWindowPos(hwndStatic, NULL, 50, 10, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (count == 2)
				SetWindowPos(hwndStatic, NULL, 90, 60, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (count == 3)
				SetWindowPos(hwndStatic, NULL, 320, 173, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (count == 4)
				SetWindowPos(hwndStatic, NULL, 221, 89, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			else if (count == 5)
				SetWindowPos(hwndStatic, NULL, 349, 39, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

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