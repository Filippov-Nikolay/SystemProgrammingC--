#pragma once

#include "header.h"

const int numberOfButtons = 5;

class mainDlg {
public:
	mainDlg(void);
	~mainDlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	static mainDlg* ptr;
	void Cls_OnClose(HWND hwnd);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	bool isTimeFormat(_TCHAR* text);
	void updateSelected();

	int IdsButtons[numberOfButtons]{ IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4, IDC_BUTTON5 };

	HWND hList1, hList2, hList3;
	HWND hEdit1, hEdit2;
	HWND hButtons[numberOfButtons];

	HICON hIcon;
};