#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;
bool isSave = false;
bool fFitsr = true;
int numberCases = 0;
//std::vector<std::vector<int>> timerArray;
std::vector<int> timerArray;

// Глобальный вектор для хранения дескрипторов потоков
std::vector<HANDLE> threadHandles;

// Структура для таймера
struct ThreadParams {
	int timer;
	int index;
	HWND hList3;
};
ThreadParams* paramList = new ThreadParams;

mainDlg::mainDlg(void) {
	ptr = this;
	pNID = new NOTIFYICONDATA;
}

mainDlg::~mainDlg(void) {
	delete pNID;
}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// Получение дескриптора экземпляра приложения
	HINSTANCE hInst = GetModuleHandle(NULL);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDB_PNG1));

	// Устанавка иконки в главном окне приложения
	//SetClassLong(hwnd, GCL_HICON, LONG(hIcon));
	pNID->hIcon = hIcon;


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
	/*for (int i = 0; i < 3; i++) {
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
	}*/

	return TRUE;
}

void mainDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

DWORD WINAPI Thread_timer(LPVOID lp) {
	ThreadParams* params = (ThreadParams*)lp;

	int timer = params->timer;
	int index = params->index;
	HWND hList3 = params->hList3;

	/*
	_TCHAR buff[250];
	wsprintf(buff, _T("%d"), index);
	MessageBox(0, buff, 0, 0);*/

	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	SYSTEMTIME st;

	GetLocalTime(&st);

	st.wHour = timer / 3600;
	st.wMinute = (timer % 3600) / 60;
	st.wSecond = timer % 60;

	FILETIME ft;

	// Преобразуем структуру SYSTEMTIME в FILETIME
	SystemTimeToFileTime(&st, &ft);

	// Преобразуем местное время в UTC-время 
	LocalFileTimeToFileTime(&ft, &ft);

	// Устанавливаем таймер
	SetWaitableTimer(hTimer, (LARGE_INTEGER*)&ft, 0, NULL, NULL, FALSE);

	// Ожидание перехода таймера в сигнальное состояние
	if (WaitForSingleObject(hTimer, INFINITE) == WAIT_OBJECT_0) {
		MessageBox(0, _T("FFFFF"), 0, 0);

		// Удаление элемента по индексу
		SendMessage(hList3, LB_DELETESTRING, index, 0);

		// Вставка нового текста на тот же индекс
		SendMessage(hList3, LB_INSERTSTRING, index, LPARAM(_T("Time is up!")));
	}

	return 0;
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

		/*int* tempTimer = GetArrayFormatTime(buffTime);
		timerArray.push_back({ tempTimer[0], tempTimer[1], tempTimer[2] });*/


		/*if (isUpdate(&timerArray[numberCases++][0], timerArray.size(), timerArray[0].size())) {

		}*/

		int tempTimer = FormatTime(GetArrayFormatTime(buffTime));
		timerArray.push_back(tempTimer);
		int index = isUpdate(&timerArray[0], timerArray.size());

		paramList->timer = timerArray[index];
		paramList->index = numberCases++;
		paramList->hList3 = hList3;

		if (index != -1 || fFitsr) {
			fFitsr = false;
			// updateTimer(timerArray[resUpDate]);
			HANDLE hThread = CreateThread(NULL, 0, Thread_timer, paramList, 0, NULL);

			// Добавляем дескриптор потока в вектор
			threadHandles.push_back(hThread);

			timerArray.erase(timerArray.begin() + index);
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

		// Получение индекса выбранного пункта
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
		SendMessage(hList3, LB_DELETESTRING, index, 0);

		// Вставка текста по индексу
		SendMessage(hList1, LB_INSERTSTRING, index, LPARAM(buffTask));
		SendMessage(hList2, LB_INSERTSTRING, index, LPARAM(buffTime));
		SendMessage(hList3, LB_INSERTSTRING, index, LPARAM(_T("Not done")));

		// Изменение название кнопки на save
		SetWindowText(hButtons[3], _T("Edit"));

		// Очистка edit, после ввода
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));

		isSave = false;

		if (index != -1) {
			/*
			// Закрывает поток по дескриптору
			CloseHandle(threadHandles[index]);

			int tempTimer = FormatTime(GetArrayFormatTime(buffTime));
			timerArray.push_back(tempTimer);
			int indexList = isUpdate(&timerArray[0], timerArray.size());

			paramList->timer = timerArray[indexList];
			paramList->index = index;
			paramList->hList3 = hList3;

			HANDLE hThread = CreateThread(NULL, 0, Thread_timer, paramList, 0, NULL);

			// Добавляем дескриптор потока в вектор
			threadHandles[index] = hThread;
			*/
		}

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
			else if (i == 6 && (number > 59 || number < 0))
				return true;
		}
	}

	return false;
}

int mainDlg::isUpdate(const int* arrayTime, int size, int cols) {
	if (size == 1)
		return 0;

	_TCHAR buff[25];
	int minVal = 0;

	for (int i = 0; i < size; i++) {
		if (arrayTime[i] < arrayTime[minVal])
			minVal = i;
	}

	if (minVal == 0)
		return minVal;

	wsprintf(buff, _T("%d"), minVal);
	MessageBox(0, buff, 0, 0);

	return minVal;
}

/*
bool mainDlg::isUpdate(const int* arrayTime, int rows, int cols) {
	_TCHAR buffTask[255];

	int tempHours = arrayTime[0];
	int tempMinutes = arrayTime[1];
	int tempSeconds = arrayTime[2];

	int TimeInSeconds = tempHours * 3600 + tempMinutes * 60 + tempSeconds;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			
		}
	}

	return false;
}
*/

int mainDlg::FormatTime(int* timerArray) {	
	return timerArray[0] * 3600 + timerArray[1] * 60 + timerArray[2];
}

void mainDlg::updateSelected() {
	// Получение индекста
	int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

	// Сделать пункты активными по индексу
	SendMessage(hList1, LB_SETCURSEL, index, 0);
	SendMessage(hList2, LB_SETCURSEL, index, 0);
	SendMessage(hList3, LB_SETCURSEL, index, 0);
}

void mainDlg::updateTimer(int& timer) {
	// Создаем таймер синхронизации
	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	SYSTEMTIME st;

	st.wHour = timer / 3600;
	st.wMinute = (timer % 3600) / 60;
	st.wSecond = timer % 60;

	FILETIME ft;

	// Преобразуем структуру SYSTEMTIME в FILETIME
	SystemTimeToFileTime(&st, &ft); 

	// Преобразуем местное время в UTC-время 
	LocalFileTimeToFileTime(&ft, &ft); 
	
	// Устанавливаем таймер
	SetWaitableTimer(hTimer, (LARGE_INTEGER*)&ft, 0, NULL, NULL, FALSE); 

	MessageBox(0, _T("DO"), 0, 0);

	// Ожидание перехода таймера в сигнальное состояние
	if (WaitForSingleObject(hTimer, INFINITE) == WAIT_OBJECT_0) {
		MessageBox(0, _T("FFFFF"), 0, 0);
	}
}

int* mainDlg::GetArrayFormatTime(_TCHAR* text) {
	const int lengthText = _tcsclen(text);
	_TCHAR temp[3] = _T("");
	int* rez = new int[3];
	
	for (int i = 0; i < lengthText; i++) {
		// Проверка на корректный ввод чисел
		if (text[i] != _T(':')) {
			temp[0] = text[i];
			temp[1] = text[i + 1];
			temp[2] = _T('\0');

			int number = _tstoi(temp);

			if (i == 0) 
				rez[0] = number;
			else if (i == 3) 
				rez[1] = number;
			else if (i == 6)
				rez[2] = number;
		}
	}

	return rez;
}

BOOL CALLBACK mainDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}