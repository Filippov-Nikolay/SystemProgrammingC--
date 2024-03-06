#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {
}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

	return TRUE;
}

void ReadFromFile() {
	const int maxSize = 255;
	const int length = 3;

	_TCHAR buff[maxSize] = _T("");

	for (int i = 1; i <= length; i++) {
		char buffText[maxSize];
		
		wsprintf(buff, _T("file%d.txt"), i);
		std::fstream in(buff, std::ios::in);
		
		do {
			in.getline(buffText, maxSize);

			int len = MultiByteToWideChar(CP_UTF8, 0, buffText, -1, NULL, 0);
			wchar_t* wbuf = new wchar_t[len];
			MultiByteToWideChar(CP_UTF8, 0, buffText, -1, wbuf, len);

			/*Вывод wbuf*/
			
			
			delete[] wbuf;
		} while (in);

		

	}
	

	
}

void mainDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

void mainDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (id == IDOK) {

	}
	
	if (id == id) {
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK mainDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}