#include "SeasonsDlg.h"

SeasonsDlg* SeasonsDlg::ptr = NULL;

SeasonsDlg::SeasonsDlg(void)
{
	ptr = this;
}

void SeasonsDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL SeasonsDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	// Получим дескрипторы элементов управления
	hCombo = GetDlgItem(hwnd, IDC_SEASONS);
	hList = GetDlgItem(hwnd, IDC_MONTH);
	hEdit = GetDlgItem(hwnd, IDC_DAYS);
	hCheck = GetDlgItem(hwnd, IDC_LEAF);

	// Добавляем времена года в ComboBox
	SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Зима")));
	SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Весна")));
	SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Лето")));
	SendMessage(hCombo, CB_ADDSTRING, 0, LPARAM(TEXT("Осень")));
	return TRUE;
}

void SeasonsDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// событие CBN_SELCHANGE выпадающего списка происходит в тот момент, когда выбирается новый элемент в списке
	if(id == IDC_SEASONS && codeNotify == CBN_SELCHANGE)
	{
		// Очистим ListBox
		SendMessage(hList, LB_RESETCONTENT, 0, 0);
		// Получим индекс выбранного элемента ComboBox
		int index = SendMessage(hCombo, CB_GETCURSEL, 0, 0); 
		switch(index)
		{
		case 0:
			// Добавляем в ListBox название месяца и запоминаем индекс добавленного элемента
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Декабрь")));
			// Ставим элементу списка в соответствие число, определяющее количество дней в месяце
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Январь")));
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Февраль")));
			SendMessage(hList, LB_SETITEMDATA, index, 28);
			break;

		case 1:
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Март")));
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Апрель")));
			SendMessage(hList, LB_SETITEMDATA, index, 30);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Май")));
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			break;

		case 2:
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Июнь")));
			SendMessage(hList, LB_SETITEMDATA, index, 30);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Июль")));
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Август")));
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			break;

		case 3:
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Сентябрь")));
			SendMessage(hList, LB_SETITEMDATA, index, 30);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Октябрь")));
			SendMessage(hList, LB_SETITEMDATA, index, 31);
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(TEXT("Ноябрь")));
			SendMessage(hList, LB_SETITEMDATA, index, 30);
			break;
		}

	}

	// событие LBN_SELCHANGE списка происходит в тот момент, когда выбирается новый элемент в списке
	if(id == IDC_MONTH && codeNotify == LBN_SELCHANGE)
	{
		// Получим индекс выбранного элемента ListBox
		int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
		// Получим значение, связанное с выбранным элементом списка
		int day = SendMessage(hList, LB_GETITEMDATA, index, 0);
		TCHAR buffer[10];
		// Снимем текст с выбранного элемента списка
		SendMessage(hList, LB_GETTEXT, index, LPARAM(buffer));
		if(!lstrcmp(buffer, TEXT("Февраль"))) // Выбранный месяц - Февраль?
		{
			// Проверяем, високосный год или нет
			int status = SendMessage(hCheck, BM_GETCHECK, 0, 0);
			if(status == BST_CHECKED)
				day++;
		}
		wsprintf(buffer, TEXT("%d"), day);
		// Устанавливаем количество дней в EditControl
		SendMessage(hEdit, WM_SETTEXT, 0, LPARAM(buffer));
	}
}

BOOL CALLBACK SeasonsDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}