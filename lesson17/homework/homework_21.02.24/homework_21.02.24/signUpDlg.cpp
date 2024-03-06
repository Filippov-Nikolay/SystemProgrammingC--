#include "signUpDlg.h"
#include "signInDlg.h"

signUpDlg* signUpDlg::ptr = NULL;

signUpDlg::signUpDlg(void) {
	ptr = this;
}
signUpDlg::signUpDlg(LPCTSTR lpStr) {
	ptr = this;
	//_tcscpy(text, lpStr);
}

signUpDlg::~signUpDlg() {
}

void signUpDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, IDCANCEL);
}

BOOL signUpDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// Инициализация полей
	for (int i = 0; i < numberOfInputsSignUp; i++) {
		hEditSignUp[i] = GetDlgItem(hwnd, inputIdsSignUp[i]);
	}

	// Инициализация кнопки
	hButtonSignUp = GetDlgItem(hwnd, IDC_BUTTON_REG);

	return TRUE;
}

void signUpDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (IDC_BUTTON_REG == id) {
		_TCHAR buff[256] = _T("");
		int length = SendMessage(hEditSignUp[3], WM_GETTEXTLENGTH, 0, 0);
		GetWindowText(hEditSignUp[3], buff, length + 1);

		if (!isLoginExist(hwnd, buff)) {
			int index = MessageBox(hwnd, _T("Хотите авторизоваться?"), _T("Такой логин уже есть!"), MB_YESNO);

			if (index == IDYES) {
				signInDlg dlg;
				INT_PTR result = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, signInDlg::DlgProc);
			}

			return;
		}

		tfstream out(TEXT("Database.txt"), std::ios::app);

		for (int i = 0; i < numberOfInputsSignUp; i++) {
			length = SendMessage(hEditSignUp[i], WM_GETTEXTLENGTH, 0, 0);
			
			if (length == 0) {
				MessageBox(hwnd, _T("Заполните все поля!"), _T("Предупреждение!"), 0);
				break;
			}
			
			GetWindowText(hEditSignUp[i], buff, length + 1);

			i == 0 ? out << "[ " << buff << " | " :
				i != numberOfInputsSignUp - 1 ? out << buff << " | " :
				out << buff << " ]\n";
		}
	}
}

INT_PTR CALLBACK signUpDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}

bool signUpDlg::isLoginExist(HWND hwnd, const _TCHAR* login) {
	tifstream in("Database.txt");

	const int max_size = 255;
	int counter = 0;

	tstring temp[5];
	tstring tempChar;
	int index = 0;

	_TCHAR buff[max_size] = _T("");

	do {
		in >> buff[index];

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
		else {
			index++;
		}

		if (_tcscmp(login, temp[3].c_str()) == 0)
			return false;
	} while (in);

	return true;
}

/*
string LogUs::GetLogin(string login, string password) const {
	std::ifstream in("LogUs.txt");

	string temp[8];
	int index = 0;

	do {
		char buff[300]{};

		in.getline(buff, 300);

		for (int i = 0; i < strlen(buff); i++)
			if (buff[i] == '|')
				for (int j = (i + 2); j < strlen(buff); j++) {
					if (buff[j] == ' ') {
						index++;
						break;
					}
					temp[index] += buff[j];
				}

		if (temp[3] == login && temp[4] == password)
			return temp[3];

		for (int i = 0; i < index; i++)
			temp[i] = "";

		index = 0;
	} while (in);
}
*/