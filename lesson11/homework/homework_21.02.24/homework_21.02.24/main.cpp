
/*
#include "header.h"

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitializationRegistration();

// Форма
const int numberButtons = 2;

int hButtonIds[numberButtons] = { IDC_SIGNUP, IDC_SIGNIN };
HWND hButtons[numberButtons] = { 0 };


// Регистрация
const int numberOfInputsSignUp = 5;

int inputIdsSignUp[numberOfInputsSignUp] = { IDC_LASTNAME, IDC_NAME, IDC_AGE, IDC_LOGIN, IDC_PASSWORD };
HWND hEditSignUp[numberOfInputsSignUp] = { 0 };


// Вход
const int numberOfInputsSignIn = 2;


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG: {

		// Инициализация полей в окне формы
		for (int i = 0; i < numberButtons; i++) {
			hButtons[i] = GetDlgItem(hwnd, hButtonIds[i]);
		}

		// Инициализация полей в окне регистрации
		for (int i = 0; i < numberOfInputsSignUp; i++) {
			hEditSignUp[i] = GetDlgItem(hwnd, inputIdsSignUp[i]);
		}

		return TRUE;
	}
	case WM_COMMAND: {
		
		return TRUE;
	}
	case WM_CLOSE: {
		EndDialog(hwnd, 0);

		return TRUE;
	}
	}

	return FALSE;
}
*/

#include "mainDlg.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) {
	FormDlg dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, FormDlg::DlgProc);
}