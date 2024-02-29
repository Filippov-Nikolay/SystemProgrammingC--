#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;
bool isSave = false;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {

}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
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
	}
	else if (id == IDC_LIST1 && codeNotify == LBN_SELCHANGE || id == IDC_LIST2 && codeNotify == LBN_SELCHANGE || id == IDC_LIST3 && codeNotify == LBN_SELCHANGE) {
		// Получение индекста
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// Сделать пункты активными по индексу
		SendMessage(hList1, LB_SETCURSEL, index, 0);
		SendMessage(hList2, LB_SETCURSEL, index, 0);
		SendMessage(hList3, LB_SETCURSEL, index, 0);

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

		isSave = true;
	}
	else if (id == IDC_BUTTON4 && isSave) {
		// Save

		// Получение индекста
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// Получение текста из edit
		GetWindowText(hEdit1, buffTask, max_size);
		GetWindowText(hEdit2, buffTime, max_size);

		// Очистить содержимое по индексу
		SendMessage(hList1, LB_DELETESTRING, index, 0);
		SendMessage(hList2, LB_DELETESTRING, index, 0);
		SendMessage(hList3, LB_DELETESTRING, index, 0);

		// Вставка текста по индексу
		SendMessage(hList1, LB_INSERTSTRING, index, LPARAM(buffTask));
		SendMessage(hList2, LB_INSERTSTRING, index, LPARAM(buffTime));

		// Изменение название кнопки на save
		SetWindowText(hButtons[3], _T("Edit"));

		// Очистка edit, после ввода
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));

		isSave = false;

	}
	else if (id == IDC_BUTTON5) {
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK mainDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}