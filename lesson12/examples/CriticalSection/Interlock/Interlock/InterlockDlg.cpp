#include "InterlockDlg.h"

CInterlockDlg* CInterlockDlg::ptr = NULL;

const int maxSize = 10;

LONG counter = 0;
LONG arrayNumbers[maxSize] = { 0 };

bool fFirstStart = true;

CInterlockDlg::CInterlockDlg(void) {
	ptr = this;
}

CInterlockDlg::~CInterlockDlg(void) {
	
}

void CInterlockDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

BOOL CInterlockDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	hList1 = GetDlgItem(hwnd, IDC_LIST1);

	return TRUE;
}

DWORD WINAPI Thread3(LPVOID lp) {
	HWND hWnd = HWND(lp);

	for (int i = 0; i < maxSize; i++) {
		arrayNumbers[counter] = rand() % 10 + 0;

		InterlockedIncrement(&counter);
	}

	return 0;
}

DWORD WINAPI Thread4(LPVOID lp) {
	HWND hWnd = HWND(lp);
	TCHAR str[MAX_PATH];

	for (int i = 0; i < maxSize; i++) {
		wsprintf(str, TEXT("%d"), arrayNumbers[counter]);
		SendMessage(hWnd, LB_ADDSTRING, 0, LPARAM(str));

		InterlockedDecrement(&counter); 
	}

	return 0;
}

void CInterlockDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	srand(time(NULL));

	if (id == IDC_BUTTON1) {
		EndDialog(hwnd, 0);
	}

	if (fFirstStart) {
		counter = 0;
		HANDLE hThread = CreateThread(NULL, 0, Thread3, hList1, 0, NULL);
		CloseHandle(hThread);

		counter = 0;
		hThread = CreateThread(NULL, 0, Thread4, hList1, 0, NULL);
		CloseHandle(hThread);
	}

	fFirstStart = false;
}

BOOL CALLBACK CInterlockDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}