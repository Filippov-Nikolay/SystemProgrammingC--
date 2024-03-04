#pragma once
#include "header.h"

const int numberOfInputsSignUp = 5;

class signUpDlg {
public:
	int inputIdsSignUp[numberOfInputsSignUp] = { IDC_LASTNAME, IDC_NAME, IDC_AGE, IDC_LOGIN, IDC_PASSWORD };
	HWND hEditSignUp[numberOfInputsSignUp] = { 0 };
	HWND hButtonSignUp;

	signUpDlg(void);
	signUpDlg(LPCTSTR);
	~signUpDlg();
	bool isLoginExist(HWND hwnd, const _TCHAR* login);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static signUpDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
};