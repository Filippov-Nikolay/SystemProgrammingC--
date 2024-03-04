#pragma once
#include "header.h"

const int numberButtons = 2;

class FormDlg {
public:
	int hButtonIds[numberButtons] = { IDC_SIGNUP, IDC_SIGNIN };
	HWND hButtons[numberButtons] = { 0 };

	FormDlg(void);
	~FormDlg();

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static FormDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
};