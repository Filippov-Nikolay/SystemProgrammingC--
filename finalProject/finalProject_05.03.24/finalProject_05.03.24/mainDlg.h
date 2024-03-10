#pragma once

#include "header.h"

class mainDlg {
public:
	mainDlg(void);
	~mainDlg(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	static mainDlg* ptr;
	void Cls_OnClose(HWND hwnd);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	HWND hEdit1, hEdit2, hList1, hButton1, hButton2, hButton3, hProgress1, hDialog;
	HANDLE hThread;
	

	// Массив слов
	tstring* arrayWords = nullptr;

	// Массив запрещённых слов
	tstring* forbiddenWords = nullptr;

	// Счётчик кол-во слов
	int wordCount[3];
};