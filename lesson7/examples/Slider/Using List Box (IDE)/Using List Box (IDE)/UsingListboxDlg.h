#pragma once
#include "header.h"

class UsingListboxDlg
{
public:
	UsingListboxDlg(void);
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static UsingListboxDlg* ptr;
	HWND hDialog, hEdit1, hEdit2, hEdit3, hList1, hList2;
	std::vector<std::wstring> hListUsers1;
	std::vector<std::wstring> hListUsers2;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	
};
