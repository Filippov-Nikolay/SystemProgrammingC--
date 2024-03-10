#include "mainDlg.h"

// Переменные

// Кол-во скопированных файлов
int copyCounter = 0;

// Кол-во заменённых слов
int numberReplacedWords = 0;

// Создан ли поток
bool isCreate = false;

// Функция 
tstring* getWordArray(_TCHAR* text, int& size) {
	const int lengthText = _tcsclen(text);
	int arrayCount = 0;
	int wordCount = 1;

	// Считаем кол-во слов
	for (int i = 0; i < lengthText; i++) {
		if (text[i] == ' ')
			wordCount++;
	}

	// Массив который соответствует кол-во слов
	tstring* arrayWords = new tstring[wordCount];

	// Добавляем по символьно слова в массив
	for (int i = 0; i < lengthText; i++) {
		if (text[i] != ' ')
			arrayWords[arrayCount] += text[i];
		else
			arrayCount++;
	}

	size = wordCount;

	/*
	for (int i = 0; i < wordCount; i++) {
		MessageBox(0, arrayWords[i].c_str(), _T("getWordArray"), 0);
	}
	*/

	return arrayWords;
}

void stand() {
}

mainDlg* mainDlg::ptr = NULL;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {
}

// Поток копирования
DWORD WINAPI Thread(LPVOID lp) {
	mainDlg* ptr = (mainDlg*)lp;

	const int maxSize = 1000;
	const int length = 5;
	int numberFiles[length]{ 0 };
	bool oneStep = true;
	
	// НАЧАЛО - объеденение запрещённых слов
	// Массив запрещённых слов
	tstring* forbiddenWords = nullptr;

	// Буффер запрещённых слов
	_TCHAR buffForbiddenWords[maxSize] = _T("");

	// Получаем запрещённые слова из hEdit1
	GetWindowText(ptr->hEdit1, buffForbiddenWords, GetWindowTextLength(ptr->hEdit1) + 1);

	forbiddenWords = getWordArray(buffForbiddenWords, ptr->wordCount[1]);

	// Результат запрещённых слов
	int numberBannedWords = ptr->wordCount[0] + ptr->wordCount[1];
	tstring* resultForbiddenWords = new tstring[numberBannedWords];

	// Объеденение запрещённых слов
	memcpy(resultForbiddenWords, ptr->forbiddenWords, ptr->wordCount[0] * sizeof(tstring));
	memcpy(resultForbiddenWords + ptr->wordCount[0], forbiddenWords, ptr->wordCount[1] * sizeof(tstring));

	/*for (int i = 0; i < numberBannedWords; i++) {
		MessageBox(0, resultForbiddenWords[i].c_str(), _T("result Forbidden Words"), 0);
	}*/
	// КОНЕЦ - объеденение запрещённых слов


	// НАЧАЛО - таскбар
	// Устанавливаем прогресс в ноль
	SendMessage(ptr->hProgress1, PBM_SETPOS, (WPARAM)0, 0);

	// Установка кол-во шагов
	if (oneStep) {
		oneStep = false;

		// Вычисляем остаток
		int remainder = 100 % length;

		// Добавляем остаток к индикатору выполнения
		SendMessage(ptr->hProgress1, PBM_SETSTEP, remainder, 0);
		SendMessage(ptr->hProgress1, PBM_STEPIT, 0, 0);

		// Устанавливаем шаг инкремента равным length
		SendMessage(ptr->hProgress1, PBM_SETSTEP, 100 / length, 0);
	}
	// КОНЕЦ - таскбар
	
	tstring* arrayWords = nullptr;
	bool fWrite = false;
	_TCHAR buff[maxSize] = _T("");

	for (int n = 1; n <= length; n++) {
		char buffText[maxSize];

		// Чтение файла
		wsprintf(buff, _T("C:\\Users\\filip\\Desktop\\temp\\SystemProgrammingC--\\finalProject\\finalProject_05.03.24\\finalProject_05.03.24\\file%d.txt"), n);
		std::fstream in(buff, std::ios::in);

		while (!in.eof()) {
			// Получение строки из файла
			in.getline(buffText, maxSize);

			int len = MultiByteToWideChar(CP_UTF8, 0, buffText, -1, NULL, 0);
			wchar_t* wbuf = new wchar_t[len];
			MultiByteToWideChar(CP_UTF8, 0, buffText, -1, wbuf, len);

			arrayWords = getWordArray(wbuf, ptr->wordCount[2]);
			
			if (!fWrite) {
				// Цикл по кол-во запрещённых слов
				for (int i = 0; i < numberBannedWords; i++) {
					// Цикл по кол-во слов
					for (int j = 0; j < ptr->wordCount[2]; j++) {
						_TCHAR temp[maxSize];

						for (int k = 0, m = 0; k < arrayWords[j].size(); k++) {
							if (!ispunct(arrayWords[j][k]))
								temp[m++] = arrayWords[j][k];

							if (k == arrayWords[j].size() - 1)
								temp[m] = '\0';
						}

						if (resultForbiddenWords[i] == temp) {
							fWrite = true;
							break;
						}
					}

					if (fWrite) {
						// Запись в файл
						wsprintf(buff, _T("C:\\Users\\filip\\Desktop\\temp\\SystemProgrammingC--\\finalProject\\finalProject_05.03.24\\finalProject_05.03.24\\Folder\\file%d.txt"), n);

						// Счётчик скопированных файлов
						numberFiles[copyCounter++] = n;

						break;
					}
				}
			}
			
			if (fWrite) {
				std::ofstream out(buff, std::ios::app);

				// Цикл по кол-во запрещённых слов
				for (int i = 0; i < numberBannedWords; i++) {
					// Цикл по кол-во слов
					for (int j = 0; j < ptr->wordCount[2]; j++) {
						_TCHAR temp[maxSize] = _T("");

						for (int k = 0, m = 0; k < arrayWords[j].size(); k++) {
							if (!ispunct(arrayWords[j][k]))
								temp[m++] = arrayWords[j][k];

							if (k == arrayWords[j].size() - 1)
								temp[m] = '\0';
						}
						
						if (resultForbiddenWords[i] == temp) {
							arrayWords[j] = _T("*******");
							numberReplacedWords++;
						}
					}
				}

				for (int i = 0; i < ptr->wordCount[2]; i++) {
					_TCHAR* temp = new _TCHAR[maxSize];

					_tcscpy_s(temp, maxSize, arrayWords[i].c_str());

					int mbSize = WideCharToMultiByte(CP_UTF8, 0, temp, -1, nullptr, 0, nullptr, nullptr);
					char* mbBuffer = new char[mbSize];
					WideCharToMultiByte(CP_UTF8, 0, temp, -1, mbBuffer, mbSize, nullptr, nullptr);

					out << mbBuffer << " ";
				}

				out << '\n';
			}
		}
		// Изменение текущей позиции индикатора путём прибавления шага
		SendMessage(ptr->hProgress1, PBM_STEPIT, 0, 0);

		fWrite = false;
	}

	// IDC_LIST1
	// Очистка списка перед выводом
	SendMessage(ptr->hList1, LB_RESETCONTENT, 0, 0);

	// Длина горизонтального скролла
	SendMessage(ptr->hList1, LB_SETHORIZONTALEXTENT, 200, 0);

	wsprintf(buff, _T("Кол-во скопированных файлов: %d"), copyCounter);
	SendMessage(ptr->hList1, LB_ADDSTRING, 0, LPARAM(buff));

	wsprintf(buff, _T("Кол-во заменённых слов: %d"), numberReplacedWords);
	SendMessage(ptr->hList1, LB_ADDSTRING, 0, LPARAM(buff));

	wsprintf(buff, _T("Измененные файлы:"));
	SendMessage(ptr->hList1, LB_ADDSTRING, 0, LPARAM(buff));

	for (int i = 0; i < copyCounter; i++) {
		wsprintf(buff, _T("file%d.txt"), numberFiles[i]);
		SendMessage(ptr->hList1, LB_ADDSTRING, 0, LPARAM(buff));
	}

	// Обнуление переменных
	copyCounter = 0;
	numberReplacedWords = 0;

	// Устанавливаем изначальный текст кнопки
	SetWindowText(ptr->hButton2, TEXT("Запустить"));
	EnableWindow(ptr->hButton3, FALSE);

	// Разрешаем создать поток
	isCreate = false;

	// Получаем текущее значение прогресса
	int pos = SendMessage(ptr->hProgress1, PBM_GETPOS, 0, 0);

	// Получаем максимальное значение прогресса
	int max = SendMessage(ptr->hProgress1, PBM_GETRANGE, FALSE, 0);

	if (pos == max)
		MessageBox(ptr->hDialog, _T("Замена слов - завершена!"), _T("Информация"), MB_OK);

	return 0;
}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// Инициализация поля
	hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);

	// Инициализация листа
	hList1 = GetDlgItem(hwnd, IDC_LIST1);

	// Инициализация кнопок
	hButton1 = GetDlgItem(hwnd, IDC_BUTTON_DOWNLOAD);
	hButton2 = GetDlgItem(hwnd, IDC_BUTTON_START);
	hButton3 = GetDlgItem(hwnd, IDC_BUTTON_END);

	// Инициализация прогресс-бара
	hProgress1 = GetDlgItem(hwnd, IDC_PROGRESS1);
	SendMessage(hProgress1, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); // Установка интервала для индикатора 
	SendMessage(hProgress1, PBM_SETPOS, 0, 0); // Установка текущей позиции индикатора

	// дескриптор окна
	hDialog = hwnd;

	return TRUE;
}

void mainDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

void mainDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (id == IDC_BUTTON_START) {
		/*CopyingFiles(); */

		EnableWindow(hButton3, TRUE);

		if (!isCreate) {
			// Запускаем таймер
			SetTimer(hwnd, 1, 1000, NULL);

			// Запрещаем создавать поток
			isCreate = true;

			// Запуск потока
			hThread = CreateThread(NULL, 0, Thread, this, NULL, NULL);
			SetWindowText(hButton2, TEXT("Пауза"));
		}
		else {
			static BOOL flag = TRUE;
			if (flag) {
				SuspendThread(hThread);
				SetWindowText(hButton2, TEXT("Старт"));
			}
			else {
				ResumeThread(hThread);
				SetWindowText(hButton2, TEXT("Пауза"));
			}
			flag = !flag;
		}
	}
	else if (id == IDC_BUTTON_END) {
		TerminateThread(hThread, 0);
		CloseHandle(hThread);
		EnableWindow(hButton3, FALSE);

		// Устанавливаем изначальный текст кнопки
		SetWindowText(hButton2, TEXT("Запустить"));
		EnableWindow(hButton2, FALSE);
	}
	else if (id == IDC_BUTTON_DOWNLOAD) {
		_TCHAR FullPath[MAX_PATH] = { 0 };
		OPENFILENAME open = { sizeof(OPENFILENAME) };
	
		open.hwndOwner = hwnd;
		open.lpstrFilter = _TEXT("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0");
		open.lpstrFile = FullPath;
		open.nMaxFile = MAX_PATH;
		open.lpstrInitialDir = _TEXT("C:\\Users\\filip\\Desktop\\temp\\SystemProgrammingC--\\finalProject\\finalProject_05.03.24");
		open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST;

		if (GetOpenFileName(&open)) {
			_TCHAR str[300] = _TEXT("Вы выбрали для открытия файл:\n");
			_TCHAR temp[300];

			int count = 0;
			int tempCount = 0;

			_tcscat_s(str, FullPath);
			MessageBox(hwnd, str, _TEXT("Диалог \"Открыть\""), MB_OK);

			for (int i = 0; i < _tcslen(FullPath); i++) {
				if (FullPath[i] == '\\')
					count++;
			}
			for (int i = 0, j = 0; i < _tcslen(FullPath); i++) {
				if (FullPath[i] == '\\')
					tempCount++;

				if (tempCount >= count)
					temp[j++] = FullPath[i];
		
				if (i == _tcslen(FullPath) - 1)
					temp[j] = '\0';
				
			}

			SetWindowText(hEdit2, temp);
		}

		char buffText[MAX_PATH];

		// Чтение файла
		std::fstream in(FullPath, std::ios::in);

		// Получение строки из файла
		in.getline(buffText, MAX_PATH);

		int len = MultiByteToWideChar(CP_UTF8, 0, buffText, -1, NULL, 0);
		wchar_t* wbuf = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, buffText, -1, wbuf, len);

		forbiddenWords = getWordArray(wbuf, wordCount[0]);

		in.close();
		delete[] wbuf;
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