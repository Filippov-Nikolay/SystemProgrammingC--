#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"
#include <shellapi.h>

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND dlgButtons;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE: {
			EndDialog(hwnd, 0);

			return TRUE;
		}
		case WM_INITDIALOG: {
			// Устанавливаем заголовок
			SetWindowText(hwnd, _TEXT("HEHEHE"));

			// Инициализиия кнопки
			dlgButtons = GetDlgItem(hwnd, IDC_BUTTON1);

			return TRUE;
		}
		case WM_COMMAND: {
			if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
				// Запускаем таймер
				SetTimer(hwnd, 1, 100, NULL);
			}

			return TRUE;
		}
		case WM_TIMER: {
			// MessageBox(hwnd, _TEXT("HEHEHE"), _TEXT("HEHEHE"), MB_ICONINFORMATION);
			ShellExecute(0, 0, L"https://youtu.be/dQw4w9WgXcQ?si=qIK0NIq-brTpR0WF", 0, 0, SW_SHOW);

			return TRUE;
		}
	}

	return FALSE;
}