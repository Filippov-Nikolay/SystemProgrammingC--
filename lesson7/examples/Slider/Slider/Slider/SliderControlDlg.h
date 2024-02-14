#pragma once
#include "header.h"

class CSliderControlDlg
{
public:
	CSliderControlDlg(void);
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CSliderControlDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
	void Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
	void Cls_OnClose(HWND hwnd);
	HWND hDialog, hHorizontalSlider, hVerticalSlider;
};
