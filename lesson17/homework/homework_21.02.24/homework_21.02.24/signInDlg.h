#pragma once
#include "header.h"

const int numberOfInputsSignIn = 2;

class signInDlg {
public:
	int inputIdsSignIn[numberOfInputsSignIn] = { IDC_LOGIN, IDC_PASSWORD };
	HWND hEditSignIn[numberOfInputsSignIn] = { 0 };
	HWND hButtonSignIn;

	signInDlg(void);
	signInDlg(LPCTSTR);
	~signInDlg();

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static signInDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	bool Authorization(HWND hwnd, const _TCHAR* login, const _TCHAR* password);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
};
