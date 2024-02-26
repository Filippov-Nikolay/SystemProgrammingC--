#include "signInDlg.h"

signInDlg* signInDlg::ptr = NULL;

signInDlg::signInDlg(void) {
	ptr = this;
}
signInDlg::signInDlg(LPCTSTR lpStr) {
	ptr = this;
	//_tcscpy(text, lpStr);
}

signInDlg::~signInDlg() {
}

void signInDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, IDCANCEL);
}

BOOL signInDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// Инициализация полей
	for (int i = 0; i < numberOfInputsSignIn; i++) {
		hEditSignIn[i] = GetDlgItem(hwnd, inputIdsSignIn[i]);
	}

	// Инициализация кнопки
	hButtonSignIn = GetDlgItem(hwnd, IDC_BUTTON_AUTHO);

	return TRUE;
}

void signInDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (IDC_BUTTON_AUTHO == id) {
		_TCHAR buffLogin[256] = _T("");
		_TCHAR buffPassword[256] = _T("");
		_TCHAR buff[256] = _T("");

		int length = 0;

		for (int i = 0; i < numberOfInputsSignIn; i++) {
			length = SendMessage(hEditSignIn[i], WM_GETTEXTLENGTH, 0, 0);

			if (length == 0) {
				MessageBox(hwnd, _T("Заполните все поля!"), _T("Предупреждение!"), 0);
				break;
			}

			i == 0 ?
				GetWindowText(hEditSignIn[0], buffLogin, length + 1) :
				GetWindowText(hEditSignIn[1], buffPassword, length + 1);
		}

		wsprintf(buff, _T(".%s, .%s."), buffLogin, buffPassword);
		MessageBox(hwnd, buff, _T("Test"), 0);

		if (Authorization(hwnd, buffLogin, buffPassword)) {
			MessageBox(hwnd, _T("Вход - разрешен!"), _T("Вход!"), 0);
		}
		else {
			MessageBox(hwnd, _T("Вход - запрещён!\nНеверно введён логин или пароль!"), _T("Вход!"), MB_OK);
		}
	}
}

bool signInDlg::Authorization(HWND hwnd, const _TCHAR* login, const _TCHAR* password) {
	tifstream in("Database.txt");

	const int max_size = 255;
	int counter = 0;

	tstring temp[5];
	tstring tempChar;
	int index = 0;

	_TCHAR buff[max_size] = _T("");

	do {
		in >> buff[index++];

		if (buff[index] == ']') {
			buff[index + 1] = '\0';

			for (int j = 0; j < _tcslen(buff); j++) {
				if (buff[j] != ' ' && buff[j] != '[' && buff[j] != '|' && buff[j] != ']') {
					tempChar += buff[j];
				}
				else if (buff[j] == '|') {
					temp[counter++] = tempChar;
					tempChar = _T("");
				}
			}

			tempChar = _T("");
			counter = 0;
			index = 0;
		}

		if (_tcscmp(login, temp[3].c_str()) == 0 && _tcscmp(password, temp[4].c_str()) == 0)
			return true;
	} while (in);

	/*wsprintf(buff, _T("%s"), buff);
	MessageBox(hwnd, buff, _T("Test"), 0);*/

	return false;
}

INT_PTR CALLBACK signInDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}
