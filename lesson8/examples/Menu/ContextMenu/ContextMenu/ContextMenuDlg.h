#pragma once
#include "header.h"

class CContextMenuDlg
{
public:
	CContextMenuDlg(void);
public:
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CContextMenuDlg* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);
	HWND hDialog, hButton1, hButton2, hEdit1, hEdit2, hCheck1, hCheck2;
	HMENU hMenu, hSubMenu, hSubMenu2, hSubMenu3;
	DWORD dwButtonState, dwEditState, dwCheckboxState;
};
