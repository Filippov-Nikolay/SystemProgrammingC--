#include <Windows.h>
#include "resource.h"

#include <tchar.h>
#include <Tlhelp32.h>
#include <locale.h>
#include <conio.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HINSTANCE hInstance;

HWND hList1, hEdit1, hButton1, hButton2, hButton3;

void ProcessList();
void SearchList(HWND hwnd);
void KillProc();
bool AddProc(TCHAR* processName);
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG: {
		hList1 = GetDlgItem(hwnd, IDC_LIST1);
		hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
		hButton1 = GetDlgItem(hwnd, IDC_BUTTON1);
		hButton2 = GetDlgItem(hwnd, IDC_BUTTON2);
		hButton3 = GetDlgItem(hwnd, IDC_BUTTON3);

		ProcessList();

		// Запускаем таймер
		SetTimer(hwnd, 1, 10000, NULL);

		return TRUE;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_BUTTON3 && HIWORD(wParam) == BN_CLICKED) {
			SearchList(hwnd);
		}
		else if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
			_TCHAR processName[MAX_PATH] = _TEXT("");

			SendMessage(hEdit1, WM_GETTEXT, sizeof(processName) / sizeof(TCHAR), LPARAM(processName));

			AddProc(processName);
			Sleep(1000);
			ProcessList();
		}
		else if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
			KillProc();
			ProcessList();
		}

		return TRUE;
	}
	case WM_TIMER: {	
		// Обновляем список
		ProcessList();

		return TRUE;
	}
	case WM_CLOSE: {
		//Останавливаем таймер
		KillTimer(hwnd, 1);

		EndDialog(hwnd, 0);

		return TRUE;
	}
	}

	return FALSE;
}

void ProcessList() {
	_TCHAR buff[250] = _TEXT("");
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32;

	memset(&pe32, 0, sizeof(PROCESSENTRY32));
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Очищаем список
	SendMessage(hList1, LB_RESETCONTENT, 0, 0);

	if (Process32First(hSnapShot, &pe32)) {
		wsprintf(buff, _T("%s"), pe32.szExeFile);
		SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));

		while (Process32Next(hSnapShot, &pe32)) {
			wsprintf(buff, _T("%s"), pe32.szExeFile);
			SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));
		}
	}
}
void SearchList(HWND hwnd) {
	int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

	if (index != LB_ERR) {
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 pe32;

		memset(&pe32, 0, sizeof(PROCESSENTRY32));
		pe32.dwSize = sizeof(PROCESSENTRY32);

		TCHAR processName[MAX_PATH]{ 0 };
		SendMessage(hList1, LB_GETTEXT, index, (LPARAM)processName);

		if (Process32First(hSnapShot, &pe32)) {
			do {
				if (_tcscmp(pe32.szExeFile, processName) == 0) {
					_TCHAR buff[MAX_PATH] = _TEXT("");

					wsprintf(buff, _T("ID: %4u\nThreads: %5u\npriority: %2u\nname: %s"), pe32.th32ProcessID, pe32.cntThreads, pe32.pcPriClassBase, pe32.szExeFile);

					MessageBox(hwnd, buff, _TEXT("Info Process"), MB_OK);

					break;
				}
			} while (Process32Next(hSnapShot, &pe32));
		}
	}
}
void KillProc() {
	int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

	if (index != LB_ERR) {
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 pe32;

		memset(&pe32, 0, sizeof(PROCESSENTRY32));
		pe32.dwSize = sizeof(PROCESSENTRY32);

		TCHAR processName[MAX_PATH]{ 0 };
		SendMessage(hList1, LB_GETTEXT, index, (LPARAM)processName);

		if (Process32First(hSnapShot, &pe32)) {
			do {
				if (_tcscmp(pe32.szExeFile, processName) == 0) {
					HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);

					TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);

					break;
				}
			} while (Process32Next(hSnapShot, &pe32));
		}
	}
}
bool AddProc(TCHAR* processName) {
	STARTUPINFO s = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;

	if (!CreateProcess(NULL, (LPTSTR)processName, NULL, NULL, FALSE, 0, NULL, NULL, &s, &pi)) {
		MessageBox(NULL, L"CreateProcess failed", L"Error", MB_OK);
		return false;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return true;
}