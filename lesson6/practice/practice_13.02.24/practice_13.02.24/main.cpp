#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"
using namespace std;

#define IDC_CHECK3 (WM_USER + 1)

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	const int TITLE_MAX_SIZE = 100;

	switch (uMsg) {
		case WM_CLOSE: {
			EndDialog(hwnd, 0);

			return TRUE;
		}
		case WM_COMMAND: {
			if (LOWORD(wParam) == IDCANCEL && HIWORD(wParam) == BN_CLICKED || 
				LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
				EndDialog(hwnd, 0);

				return TRUE;
			}
			else if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
				_TCHAR a[250] = _TEXT("");
				_TCHAR b[250] = _TEXT("");

				HWND hEdit1 = GetDlgItem(hwnd, IDC_EDIT1); // Получаем статик IDC_EDIT1
				HWND hEdit2 = GetDlgItem(hwnd, IDC_EDIT2); // Получаем статик IDC_EDIT2
				HWND hEdit3 = GetDlgItem(hwnd, IDC_EDIT3); // Получаем статик IDC_EDIT3

				GetDlgItemText(hwnd, IDC_EDIT1, a, 250); // Получаем текст из IDC_EDIT1
				GetDlgItemText(hwnd, IDC_EDIT2, b, 250); // Получаем текст из IDC_EDIT2

				int ia = _ttoi(a); // Преобразование из текста в число
				int ib = _ttoi(b); // Преобразование из текста в число

				if (_tcslen(a) == 0 || _tcslen(b) == 0 || (_tcscmp(b, _T("0")) == 0 && IsDlgButtonChecked(hwnd, IDC_RADIO4) == BST_CHECKED)) { // Проверка на пустату и на ноль (b)
					SendMessage(hEdit3, WM_SETTEXT, 0, (LPARAM)_TEXT("NULL"));
				}
				else {
					if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED) {
						wsprintf(a, _T("%d"), ia + ib);
						SendMessage(hEdit3, WM_SETTEXT, 0, (LPARAM)a); // В IDC_EDIT3 помещаем текст из buff(IDC_EDIT1)
					}
					else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED) {
						wsprintf(a, _T("%d"), ia - ib);
						SendMessage(hEdit3, WM_SETTEXT, 0, (LPARAM)a); // В IDC_EDIT3 помещаем текст из buff(IDC_EDIT1)
					}
					else if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_CHECKED) {
						wsprintf(a, _T("%d"), ia * ib);
						SendMessage(hEdit3, WM_SETTEXT, 0, (LPARAM)a); // В IDC_EDIT3 помещаем текст из buff(IDC_EDIT1)
					}
					else if (IsDlgButtonChecked(hwnd, IDC_RADIO4) == BST_CHECKED) {
						wsprintf(a, _T("%d"), ia / ib);
						SendMessage(hEdit3, WM_SETTEXT, 0, (LPARAM)a); // В IDC_EDIT3 помещаем текст из buff(IDC_EDIT1)
					}
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}