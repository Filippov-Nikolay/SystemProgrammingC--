#include "signInDlg.h"

signInDlg* signInDlg::ptr = NULL;
HHOOK hHook  = NULL;
HWND hButtonSignIn;

_TCHAR buffLogin[256] = _T("");
_TCHAR buffPassword[256] = _T("");
bool buttonPressed = false;

signInDlg::signInDlg(void) {
	ptr = this;
}
signInDlg::signInDlg(LPCTSTR lpStr) {
	ptr = this;
	//_tcscpy(text, lpStr);
}

signInDlg::~signInDlg() {
	UnhookWindowsHookEx(hHook);
}

void signInDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, IDCANCEL);
}

LRESULT CALLBACK WriteToFile(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION && lParam && buttonPressed) {
		buttonPressed = false;

		CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;

		std::wfstream out("Authorization.txt", std::ios::app);

		if (!out.is_open()) {
			MessageBox(0, _T("Не удалось открыть файл!"), _T("Ошибка!"), MB_OK);
			return CallNextHookEx(hHook, nCode, wParam, lParam);
		}

		out << buffLogin << "\n";
		out << buffPassword << "\n\n";

		out.close();
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

BOOL signInDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// Инициализация полей
	for (int i = 0; i < numberOfInputsSignIn; i++) {
		hEditSignIn[i] = GetDlgItem(hwnd, inputIdsSignIn[i]);
	}

	// Инициализация кнопки
	hButtonSignIn = GetDlgItem(hwnd, IDC_BUTTON_AUTHO);

	hHook = SetWindowsHookEx(WH_CALLWNDPROC, WriteToFile, NULL /* для собственного потока */, GetCurrentThreadId());

	return TRUE;
}

void signInDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (IDC_BUTTON_AUTHO == id) {
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

		if (Authorization(hwnd, buffLogin, buffPassword)) {
			int id = MessageBox(hwnd, _T("Вход - разрешен!"), _T("Вход!"), MB_OK);
			buttonPressed = true;
			/*if (id == 1) {
				UnhookWindowsHookEx(hHook);
			}*/
		}
		else {
			MessageBox(hwnd, _T("Вход - запрещён!\nНеверно введён логин или пароль!"), _T("Вход!"), MB_OK);
		}
	}
}

bool signInDlg::Authorization(HWND hwnd, const _TCHAR* login, const _TCHAR* password) {
	std::ifstream in("Database.txt");

	const int max_size = 255;
	int counter = 0;

	tstring temp[5];
	tstring tempChar;

	char buff[max_size];

	wchar_t* b = new wchar_t[255];

	do {
		in.getline(buff, max_size);

		int len = MultiByteToWideChar(CP_UTF8, 0, buff, -1, NULL, 0);
		wchar_t* wbuff = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, buff, -1, wbuff, len);

		for (int i = 0; i < len; i++) {
			if (wbuff[i] == ']') {
				wbuff[i] = ']';
				wbuff[i + 1] = '\0';

				for (int j = 0; j < len; j++) {
					if (wbuff[j] != ' ' && wbuff[j] != '[' && wbuff[j] != '|' && wbuff[j] != ']') {
						tempChar += wbuff[j];
					}
					else if (wbuff[j] == '|' || wbuff[j] == ']') {
						temp[counter++] = tempChar;
						tempChar = _T("");
					}
				}

				if (_tcscmp(login, temp[3].c_str()) == 0 && _tcscmp(password, temp[4].c_str()) == 0)
					return true;

				tempChar = _T("");
				counter = 0;
			}
		}	

		delete[] wbuff;

	} while (in);

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
