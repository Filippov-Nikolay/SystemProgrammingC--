#include "ProgressControlDlg.h"

ProgressControlDlg* ProgressControlDlg::ptr = NULL;

ProgressControlDlg::ProgressControlDlg(void)
{
	ptr = this;
}

void ProgressControlDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL ProgressControlDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	srand(time(0));
	hDialog = hwnd;
	hProgress1 = GetDlgItem(hDialog, IDC_PROGRESS1);
	SendMessage(hProgress1, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); // Установка интервала для индикатора 
	SendMessage(hProgress1, PBM_SETSTEP, 15, 0); // Установка шага приращения  индикатора 
	SendMessage(hProgress1, PBM_SETPOS, 0, 0); // Установка текущей позиции индикатора
	SendMessage(hProgress1, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255))); // Установка цвета фона индикатора
	SendMessage(hProgress1, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0))); // Установка цвета заполняемых прямоугольников

	hProgress2 = GetDlgItem(hDialog, IDC_PROGRESS2);
	SendMessage(hProgress2, PBM_SETRANGE, 0, MAKELPARAM(0, 60));
	SendMessage(hProgress2, PBM_SETSTEP, 1, 0);
	SendMessage(hProgress2, PBM_SETPOS, 0, 0);
	SendMessage(hProgress2, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 255, 0)));
	SendMessage(hProgress2, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 0, 255))); 
	return TRUE;
}

void ProgressControlDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id == IDC_BUTTON1)
		SetTimer(hwnd, 1, 100, NULL);
}

void ProgressControlDlg::Cls_OnTimer(HWND hwnd, UINT id)
{
	SendMessage(hProgress1, PBM_STEPIT, 0, 0); // Изменение текущей позиции индикатора путём прибавления шага
	int n = rand()%60;
	SendMessage(hProgress2, PBM_SETPOS, WPARAM(n), 0); // Установка текущей позиции индикатора
}

BOOL CALLBACK ProgressControlDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}