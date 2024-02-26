#include "mainDlg.h"
#include "signUpDlg.h"
#include "signInDlg.h"

FormDlg* FormDlg::ptr = NULL;

FormDlg::FormDlg(void) { ptr = this; }

FormDlg::~FormDlg() { }

void FormDlg::Cls_OnClose(HWND hwnd) {
    EndDialog(hwnd, IDCANCEL);
}

BOOL FormDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	for (int i = 0; i < numberButtons; i++) {
		hButtons[i] = GetDlgItem(hwnd, hButtonIds[i]);
	}

	return TRUE;
}

void FormDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (id == IDC_SIGNUP) {
		signUpDlg dlg;

		INT_PTR result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, signUpDlg::DlgProc);
	}
	else if (id == IDC_SIGNIN) {
		signInDlg dlg;

		INT_PTR result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, signInDlg::DlgProc);
	}
}

INT_PTR CALLBACK FormDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}
