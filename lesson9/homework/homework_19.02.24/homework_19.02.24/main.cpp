#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// index
const int maxIndex = 200000;

// Amount
const int numberList = 3;
const int numberButton = 6;

// List
int hListIds[numberList] = { IDC_LIST1, IDC_LIST2, IDC_LIST3 };
HWND hLists[numberList] = {};

// Button
int hButtonIds[numberButton] = { IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4, IDC_BUTTON5, IDC_BUTTON6 };
HWND hButtons[numberButton] = {};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG: {
			// Инициализация списков
			for (int i = 0; i < numberList; i++) {
				hLists[i] = GetDlgItem(hwnd, hListIds[i]);
			}

			// Инициализация кнопок
			for (int i = 0; i < numberButton; i++) {
				hButtons[i] = GetDlgItem(hwnd, hButtonIds[i]);
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

void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (IDC_REALTIME == id) {
		HANDLE hThread = CreateThread(NULL, 0, Thread1, hEdit1, 0, NULL);
		CloseHandle(hThread);
	}
	else if (IDC_NORMAL == id) {
		HANDLE hThread = CreateThread(NULL, 0, Thread2, hEdit2, 0, NULL);
		CloseHandle(hThread);
	}
	else if (IDC_IDLE == id) {
		HANDLE hThread = CreateThread(NULL, 0, Thread3, hEdit3, 0, NULL);
		CloseHandle(hThread);
	}
}

// Потоки
DWORD WINAPI Thread1(LPVOID lp) {
	DWORD pr = GetPriorityClass(GetCurrentProcess());

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	HWND hWnd = HWND(lp);

	for (int i = 0; i <= maxIndex; i++) {
		TCHAR str[10];

		wsprintf(str, TEXT("%d"), i);
		SetWindowText(hWnd, str);
	}

	SetPriorityClass(GetCurrentProcess(), pr);

	return 0;
}

DWORD WINAPI Thread2(LPVOID lp) {
	DWORD pr = GetPriorityClass(GetCurrentProcess());

	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	HWND hWnd = HWND(lp);

	for (int i = 0; i <= maxIndex; i++) {
		TCHAR str[10];

		wsprintf(str, TEXT("%d"), i);
		SetWindowText(hWnd, str);
	}

	SetPriorityClass(GetCurrentProcess(), pr);

	return 0;
}

DWORD WINAPI Thread3(LPVOID lp) {
	DWORD pr = GetPriorityClass(GetCurrentProcess());

	SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);

	HWND hWnd = HWND(lp);

	for (int i = 0; i <= maxIndex; i++) {
		TCHAR str[10];

		wsprintf(str, TEXT("%d"), i);
		SetWindowText(hWnd, str);
	}

	SetPriorityClass(GetCurrentProcess(), pr);

	return 0;
}