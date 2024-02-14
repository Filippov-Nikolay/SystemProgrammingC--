#include "UsingListboxDlg.h"

UsingListboxDlg* UsingListboxDlg::ptr = NULL;

UsingListboxDlg::UsingListboxDlg(void)
{
	ptr = this;
}

void UsingListboxDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL UsingListboxDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	// получим дескрипторы элементов управления
	hList1 = GetDlgItem(hwnd, IDC_LIST1);
	hList2 = GetDlgItem(hwnd, IDC_LIST2);
	hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
	hEdit3 = GetDlgItem(hwnd, IDC_EDIT3);

	hListUsers1 = {
		_TEXT("Милан Италия"),
		_TEXT("Ливерпуль Англия"),
		_TEXT("Бавария Германия"),
		_TEXT("Барселона Испания"),
		_TEXT("Арсенал Англия"),
		_TEXT("Лион Франция"),
		_TEXT("Интер Италия"),
		_TEXT("Реал Испания")
	};

	// добавим 8 элементов в список с единичным выбором
	for (int i = 0; i < hListUsers1.size(); ++i) {
		SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(hListUsers1[i].c_str()));
	}

	// установим ширину колонки для списка с множественным выбором
	SendMessage(hList2, LB_SETCOLUMNWIDTH, 170, 0);

	hListUsers2 = {
		_TEXT("Рома Италия"),
		_TEXT("Манчестер Юнайтед Англия"),
		_TEXT("Вердер Германия"),
		_TEXT("Барселона Испания"),
		_TEXT("Валенсия Испания"),
		_TEXT("Челси Англия"),
		_TEXT("Марсель Франция"),
		_TEXT("Ювентус Италия"),
		_TEXT("Атлетико Испания")
	};

	// добавим 8 элементов в список с множественным выбором
	for (int i = 0; i < hListUsers2.size(); ++i) {
		SendMessage(hList2, LB_ADDSTRING, 0, LPARAM(hListUsers2[i].c_str()));
	}

	return TRUE;
}

void UsingListboxDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
	case IDC_ADD: 
		{
			// определим длину текста (названия клуба), введённого в текстовое поле
			int length = SendMessage(hEdit1, WM_GETTEXTLENGTH, 0, 0);
			// выделим память необходимого размера
			TCHAR *pBuffer = new TCHAR[length + 1];
			// в выделенную память скопируем текст, введённый в тестовое поле
			GetWindowText(hEdit1, pBuffer, length + 1);
			if(lstrlen(pBuffer))
			{
				// уточним, имеется ли уже в списке введённое название клуба 
				int index = SendMessage(hList1, LB_FINDSTRINGEXACT, -1, LPARAM(pBuffer));
				if(index == LB_ERR)
					// добавим название клуба в список
					SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(pBuffer));
				else
					MessageBox(hwnd, TEXT("Такой клуб уже существует!"), TEXT("Добавление клуба"), MB_OK | MB_ICONSTOP);
			}
			delete[] pBuffer;
		}
		break;
	case IDC_DEL:
		{
			// получим индекс выбранного элемента списка
			int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
			if(index != LB_ERR) // выбран ли элемент списка?
			{
				// определим длину названия выбранного клуба
				int length = SendMessage(hList1, LB_GETTEXTLEN, index, 0);
				// выделим память необходимого размера
				TCHAR *pBuffer = new TCHAR[length + 1];
				// скопируем название клуба в выделенную память
				SendMessage(hList1, LB_GETTEXT, index, LPARAM(pBuffer));
				MessageBox(hwnd, pBuffer, TEXT("Удаление клуба"), MB_OK | MB_ICONINFORMATION);
				// удалим строку из списка
				SendMessage(hList1, LB_DELETESTRING, index, 0);
				delete[] pBuffer;
			}
			else
				MessageBox(hwnd, TEXT("Клуб не выбран!"), TEXT("Удаление клуба"), MB_OK | MB_ICONSTOP);
		}
		break;
	case IDC_FIND:
		{
			// определим длину искомого названия, введённого в текстовое поле
			int length = SendMessage(hEdit2, WM_GETTEXTLENGTH, 0, 0);
			// выделим память необходимого размера
			TCHAR *pBuffer = new TCHAR[length + 1];
			// скопируем название клуба в выделенную память
			GetWindowText(hEdit2, pBuffer, length + 1);
			if(lstrlen(pBuffer))
			{
				// определим, имеется ли в списке искомое название клуба
				int index = SendMessage(hList1, LB_SELECTSTRING, -1, LPARAM(pBuffer));
				if(index == LB_ERR)
					MessageBox(hwnd, TEXT("Клуб не найден!"), TEXT("Поиск клуба"), MB_OK | MB_ICONSTOP);
			}
			delete[] pBuffer;
		}
		break;
	case IDC_DELALL:
		// очистим содержимое списка
		SendMessage(hList1, LB_RESETCONTENT, 0, 0);
		break;
	case IDC_GETSELITEMS:
		{
			// определим количество выбранных элементов в списке с множественным выбором
			int nCount = SendMessage(hList2, LB_GETSELCOUNT, 0, 0);
			// выделим память необходимого размера для хранения индексов выбранных элементов списка
			int *p = new int[nCount];
			// Заполним динамический массив индексами выделенных элементов списка 
			SendMessage(hList2, LB_GETSELITEMS, nCount, LPARAM(p));
			for(int i = 0; i < nCount; i++)
			{
				// определим размер текста элемента списка
				int length = SendMessage(hList2, LB_GETTEXTLEN, p[i], 0);
				// выделим память необходимого размера
				TCHAR *pBuffer = new TCHAR[length + 1];
				// в выделенную память скопируем текст выбранного элемента списка
				SendMessage(hList2, LB_GETTEXT, p[i], LPARAM(pBuffer));
				MessageBox(hwnd, pBuffer, TEXT("Список с множественным выбором"), MB_OK | MB_ICONINFORMATION);
				delete[] pBuffer;
			}
		}
	case IDC_LIST1: {
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		if (index != LB_ERR) {
			int length = SendMessage(hList1, LB_GETTEXTLEN, index, 0);
			TCHAR* pBuffer = new TCHAR[length + 1];

			SendMessage(hList1, LB_GETTEXT, index, LPARAM(pBuffer));
			SendMessage(hEdit3, WM_SETTEXT, index, LPARAM(pBuffer));
		}
		
		break;
	}
	case IDC_SAVE: {
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		if (index != LB_ERR) {
			int length = SendMessage(hEdit3, WM_GETTEXTLENGTH, 0, 0);
			TCHAR* pBuffer = new TCHAR[length + 1];

			GetWindowText(hEdit3, pBuffer, length + 1);
			SendMessage(hList1, LB_DELETESTRING, index, 0);
			SendMessage(hList1, LB_INSERTSTRING, index, LPARAM(pBuffer));
			SendMessage(hList2, LB_DELETESTRING, index, 0);
			SendMessage(hList2, LB_INSERTSTRING, index, LPARAM(pBuffer));

			delete[] pBuffer;
		}
		break;
	}
	}
	// в списке с единичным выбором текущий выбор был изменён
	if(id == IDC_LIST1 && codeNotify == LBN_SELCHANGE)
	{
		// получим индекс выбранного элемента списка
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);
		if(index != LB_ERR) // выбран ли элемент списка?
		{
			// определим длину названия клуба
			int length = SendMessage(hList1, LB_GETTEXTLEN, index, 0);
			// выделим память необходимого размера
			TCHAR *pBuffer = new TCHAR[length + 1];
			// в выделенную память скопируем текст выбранного элемента списка
			SendMessage(hList1, LB_GETTEXT, index, LPARAM(pBuffer));
			// установим текст в заголовок главного окна
			SetWindowText(hwnd, pBuffer);
			delete[] pBuffer;
		}
	}
}

BOOL CALLBACK UsingListboxDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}