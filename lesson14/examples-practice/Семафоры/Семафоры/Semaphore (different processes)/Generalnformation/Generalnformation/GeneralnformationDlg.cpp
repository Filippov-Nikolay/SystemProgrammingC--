#include "GeneralnformationDlg.h"

bool firstStep = true;
HWND hList1;

GeneralnformationDlg* GeneralnformationDlg::ptr = NULL;

GeneralnformationDlg::GeneralnformationDlg(void) {
	ptr = this;
}

GeneralnformationDlg::~GeneralnformationDlg(void) {

}

BOOL GeneralnformationDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	hList1 = GetDlgItem(hwnd, IDC_EDIT1);


	char buff[250]{ 0 };
	int numberOfCopies = 3;
	int numberOfCharacters = 0;
	int numberOfCharactersTemp = 0;
	double copySize = 0;

	_TCHAR str[MAX_PATH];
	_TCHAR numberTemp[15];

	TCHAR szTime[100];
	time_t t = time(0);   // Получаем текущее время
	struct tm now;
	localtime_s(&now, &t); // Преобразуем в структуру tm
	wsprintf(szTime, TEXT("%d.%d.%d"), now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);

	// _tcscpy_s(str, GetCommandLine());

	/*TCHAR seps[] = TEXT(" ");
	TCHAR* token, * last;
	token = _tcstok(str, seps);
	while (token != NULL) {
		token = _tcstok(NULL, seps);
		if (token)
			last = token;
	}*/

	if (firstStep) {
		firstStep = false;

		for (int i = 1; i <= numberOfCopies; i++) {
			wsprintf(str, TEXT("copymusic_%d.txt"), i);
			// MessageBox(hwnd, str, TEXT("Чтение файла"), MB_OK | MB_ICONINFORMATION);
			std::ifstream in(str, std::ios::in);

			if (!in) {
				MessageBox(hwnd, TEXT("Ошибка открытия файла!"), TEXT("Чтение файла"), MB_OK | MB_ICONINFORMATION);
			}

			do {
				in.getline(buff, 250);

				numberOfCharacters = numberOfCharactersTemp += strlen(buff) + 1;
			} while (in);

			copySize += ((double)numberOfCharactersTemp / (1024 * 1024));
			_stprintf_s(numberTemp, TEXT("%.2f"), copySize);

			numberOfCharactersTemp = 0;

			//wsprintf(str, TEXT("Кол-во копий: %d\nКол-во символов: %d\nРазмер всех файлов: %s MB\nДата: %s"), numberOfCopies, numberOfCharacters, numberTemp, szTime);
			//MessageBox(hwnd, str, TEXT("Чтение файла"), MB_OK | MB_ICONINFORMATION);
		}

		wsprintf(str, TEXT("Кол-во копий: %d\r\nКол-во символов: %d\r\nРазмер всех файлов: %s MB\r\nДата: %s"), numberOfCopies, numberOfCharacters, numberTemp, szTime);
		SendMessage(hList1, WM_SETTEXT, 0, LPARAM(str));
	}

	return TRUE;
}

void GeneralnformationDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

void GeneralnformationDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (id == IDC_BUTTON1) {
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK GeneralnformationDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}