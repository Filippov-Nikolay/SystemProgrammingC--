#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;
bool isSave = false;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {

}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// Получим дескриптор экземпляра приложения
	HINSTANCE hInst = GetModuleHandle(NULL);

	// Загрузка иконки
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDB_PNG1));

	// устанавливаем иконку в главном окне приложения
	SetClassLong(hwnd, GCL_HICON, LONG(hIcon));

	// Buttons
	for (int i = 0; i < numberOfButtons; i++) {
		hButtons[i] = GetDlgItem(hwnd, IdsButtons[i]);
	}

	// Lists
	hList1 = GetDlgItem(hwnd, IDC_LIST1);
	hList2 = GetDlgItem(hwnd, IDC_LIST2);
	hList3 = GetDlgItem(hwnd, IDC_LIST3);

	// Edits
	hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
	
	_TCHAR buff[25] = _T("");

	// Добавление в список пункты
	for (int i = 0; i < 3; i++) {
		wsprintf(buff, _T("Text %d"), i);
		SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));

		if (i == 0)
			wsprintf(buff, _T("22:02:15"));
		else if (i == 1)
			wsprintf(buff, _T("02:32:15"));
		else
			wsprintf(buff, _T("12:09:15"));

		SendMessage(hList2, LB_ADDSTRING, 0, LPARAM(buff));
		SendMessage(hList3, LB_ADDSTRING, 0, LPARAM(_T("Not done")));
	}

	return TRUE;
}

void mainDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

void mainDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	const int max_size = 250;
	_TCHAR buffTask[max_size] = _T("");
	_TCHAR buffTime[max_size] = _T("");
	
	if (id == IDC_BUTTON1) {
		// Add		

		// Получение текста из edit
		GetWindowText(hEdit1, buffTask, max_size);
		GetWindowText(hEdit2, buffTime, max_size);

		if (_tcsclen(buffTask) == 0 || _tcsclen(buffTime) == 0) {
			MessageBox(hwnd, _T("Вы заполнили не все поля!"), _T("Информация"), MB_OK);
			return;
		}

		if (isTimeFormat(buffTime)) {
			MessageBox(hwnd, _T("Введите в формате: часы:минуты:секунды!"), _T("Информация"), MB_OK);
			return;
		}

		// Добавление в список пункты
		SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buffTask));
		SendMessage(hList2, LB_ADDSTRING, 0, LPARAM(buffTime));
		SendMessage(hList3, LB_ADDSTRING, 0, LPARAM(_T("Not done")));

		// Очистка edit, после ввода
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));
	}
	else if (id == IDC_BUTTON2) {
		// Done

		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// Удаление элемента по индексу
		SendMessage(hList3, LB_DELETESTRING, index, 0);

		// Вставка нового текста на тот же индекс
		SendMessage(hList3, LB_INSERTSTRING, index, LPARAM(_T("Done")));

		// Обновляем выбранный пункт
		updateSelected();
	}
	else if (id == IDC_LIST1 && codeNotify == LBN_SELCHANGE || id == IDC_LIST2 && codeNotify == LBN_SELCHANGE || id == IDC_LIST3 && codeNotify == LBN_SELCHANGE) {
		
		// Обновляем выбранный пункт
		updateSelected();

		// Включение кнопок редаактирования
		EnableWindow(hButtons[3], TRUE);
		EnableWindow(hButtons[2], TRUE);
		EnableWindow(hButtons[1], TRUE);
	}
	else if (id == IDC_BUTTON3) {
		// Delete

		// Получение индекста
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// Очистить содержимое по индексу
		SendMessage(hList1, LB_DELETESTRING, index, 0);
		SendMessage(hList2, LB_DELETESTRING, index, 0);
		SendMessage(hList3, LB_DELETESTRING, index, 0);
	}
	else if (id == IDC_BUTTON4 && !isSave) {
		// Edit

		// Получение индекста
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// Получить текст по индексу
		SendMessage(hList1, LB_GETTEXT, index, LPARAM(buffTask));
		SendMessage(hList2, LB_GETTEXT, index, LPARAM(buffTime));

		// Установить текст в edit
		SetWindowText(hEdit1, buffTask);
		SetWindowText(hEdit2, buffTime);

		// Изменение название кнопки на save
		SetWindowText(hButtons[3], _T("Save"));

		// Выключение кнопок добавления
		EnableWindow(hButtons[0], FALSE);

		isSave = true;
	}
	else if (id == IDC_BUTTON4 && isSave) {
		// Save

		// Получение индекста
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// Получение текста из edit
		GetWindowText(hEdit1, buffTask, max_size);
		GetWindowText(hEdit2, buffTime, max_size);

		if (_tcsclen(buffTask) == 0 || _tcsclen(buffTime) == 0) {
			MessageBox(hwnd, _T("Вы заполнили не все поля!"), _T("Информация"), MB_OK);
			return;
		}

		if (isTimeFormat(buffTime)) {
			MessageBox(hwnd, _T("Введите в формате: часы:минуты:секунды!"), _T("Информация"), MB_OK);
			return;
		}

		// Очистить содержимое по индексу
		SendMessage(hList1, LB_DELETESTRING, index, 0);
		SendMessage(hList2, LB_DELETESTRING, index, 0);

		// Вставка текста по индексу
		SendMessage(hList1, LB_INSERTSTRING, index, LPARAM(buffTask));
		SendMessage(hList2, LB_INSERTSTRING, index, LPARAM(buffTime));

		// Изменение название кнопки на save
		SetWindowText(hButtons[3], _T("Edit"));

		// Очистка edit, после ввода
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));

		isSave = false;

		// Обновляем выбранный пункт
		updateSelected();

		// Включение кнопок добавления
		EnableWindow(hButtons[0], TRUE);

	}
	else if (id == IDC_BUTTON5) {
		EndDialog(hwnd, 0);
	}
}

bool mainDlg::isTimeFormat(_TCHAR* text) {
	const int lengthText = _tcsclen(text);

	// Првоерка на кол-во символов
	if (lengthText >= 9 || lengthText < 8)
		return true;

	// Проверка на формат
	if (text[2] != _T(':') || text[5] != _T(':'))
		return true;

	_TCHAR temp[3] = _T("");

	for (int i = 0; i < lengthText; i++) {
		// Проверка на наличие букв
		if (isalpha(text[i])) { 
			return true;
		}

		// Проверка на корректный ввод чисел
		if (text[i] != _T(':')) {
			temp[0] = text[i];
			temp[1] = text[i + 1];
			temp[2] = _T('\0');

			int number = _tstoi(temp);

			if (i == 0 && (number > 23 || number <= 0))
				return true;
			else if (i == 3 && (number > 59 || number < 0))
				return true;
			else if (i == 5 && (number > 59 || number < 0))
				return true;
		}
	}

	return false;
}

void mainDlg::updateSelected() {
	// Получение индекста
	int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

	// Сделать пункты активными по индексу
	SendMessage(hList1, LB_SETCURSEL, index, 0);
	SendMessage(hList2, LB_SETCURSEL, index, 0);
	SendMessage(hList3, LB_SETCURSEL, index, 0);
}

BOOL CALLBACK mainDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}