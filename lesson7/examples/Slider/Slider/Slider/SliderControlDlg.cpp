#include "SliderControlDlg.h"

CSliderControlDlg* CSliderControlDlg::ptr = NULL;

CSliderControlDlg::CSliderControlDlg(void)
{
	ptr = this;
}

void CSliderControlDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CSliderControlDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	hHorizontalSlider = GetDlgItem(hDialog, IDC_SLIDER1);
	hVerticalSlider = GetDlgItem(hDialog, IDC_SLIDER2);
	SendMessage(hHorizontalSlider, TBM_SETRANGE, TRUE, MAKELPARAM(MIN, MAX));
	SendMessage(hVerticalSlider, TBM_SETRANGE, TRUE, MAKELPARAM(MIN, MAX));
	return TRUE;
}

void CSliderControlDlg::Cls_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	static int nOldPosition = 0;
	int nCurrrentPosition = SendMessage(hwndCtl, TBM_GETPOS, 0, 0);
	RECT rect;
	GetWindowRect(hwnd, &rect);
	switch(code)
	{
		case TB_BOTTOM:
			// Нажата клавиша «End» (VK_END) — ползунок  устанавливается в крайнее правое положение
			rect.right += MAX - nOldPosition;
			break;
		case TB_TOP:
			// Нажата клавиша «Home» (VK_HOME) — ползунок устанавливается в крайнее левое положение
			rect.right -= nOldPosition - MIN;
			break;
		case TB_LINEUP: 
			// Нажата клавиша «стрелка влево» (VK_LEFT) 
			if(nCurrrentPosition > MIN) rect.right--;
			break;
		case TB_LINEDOWN:
			// Нажата клавиша «стрелка вправо» (VK_RIGHT) 
			if(nCurrrentPosition < MAX) rect.right++;
			break;
		case TB_PAGEDOWN:
		case TB_PAGEUP:
		case TB_THUMBPOSITION:
			// Отпущена кнопка мыши после перемещения ползунка 
		case TB_THUMBTRACK:
			// Ползунок перемещается с помощью мыши
			rect.right += nCurrrentPosition - nOldPosition;
			break;
	}
	nOldPosition = nCurrrentPosition;
	MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
}

void CSliderControlDlg::Cls_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
	static int nOldPosition = 0;
	int nCurrrentPosition = SendMessage(hwndCtl, TBM_GETPOS, TRUE, MAKELPARAM(MIN, MAX));
	RECT rect;
	GetWindowRect(hwnd, &rect);
	switch(code)
	{
	case TB_BOTTOM:
		// Нажата клавиша «End» (VK_END) — ползунок  устанавливается в крайнее нижнее положение
		rect.bottom += MAX - nOldPosition;
		break;
	case TB_TOP:
		// Нажата клавиша «Home» (VK_HOME) — ползунок устанавливается в крайнее верхнее положение
		rect.bottom -= nOldPosition - MIN;
		break;
	case TB_LINEUP: 
		// Нажата клавиша «стрелка вверх» (VK_UP) 
		if(nCurrrentPosition > MIN) rect.bottom--;
		break;
	case TB_LINEDOWN:
		// Нажата клавиша «стрелка вниз» (VK_DOWN) 
		if(nCurrrentPosition < MAX) rect.bottom++;
		break;
	case TB_PAGEDOWN:
	case TB_PAGEUP:
	case TB_THUMBPOSITION:
		// Отпущена кнопка мыши после перемещения ползунка 
	case TB_THUMBTRACK:
		// Ползунок перемещается с помощью мыши
		rect.bottom += nCurrrentPosition - nOldPosition;
		break;
	}
	nOldPosition = nCurrrentPosition;
	MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
}


BOOL CALLBACK CSliderControlDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_HSCROLL, ptr->Cls_OnHScroll);
		HANDLE_MSG(hwnd, WM_VSCROLL, ptr->Cls_OnVScroll);
	}
	return FALSE;
}