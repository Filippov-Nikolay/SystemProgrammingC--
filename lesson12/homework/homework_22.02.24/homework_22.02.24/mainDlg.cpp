#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;
CRITICAL_SECTION cs;
HANDLE hMutex;
HWND hEdit1;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {
	DeleteCriticalSection(&cs);
}

void mainDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

// Поток 1
DWORD WINAPI WriteToFilesThread(LPVOID lp) {
	// Создание крит. секции
	EnterCriticalSection(&cs);

	mainDlg* ptr = (mainDlg*)lp;

	// Чтение файла
	std::wifstream in(_TEXT("music.txt", std::ios::in));

	int N = 4;

	// Проверка на октрытие файла
	if (!in) {
		MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}

	wchar_t buff[254];

	// Создаю N кол-во копий
	for (int i = 1; i <= N; i++) {
		// Помещаем текст в buff
		wsprintf(buff, _TEXT("music_copy_%d.txt"), i);

		// MessageBox(0, buff, _T("0"), MB_OK);

		// Запись в файл
		std::wofstream out(buff, std::ios::out);

		// Перебираю открытый файл
		while (!in.eof()) {
			in.getline(buff, 254);

			out << buff << "\r\n";
		}

		// Сбрасываем флаги ошибок
		in.clear();
		// Возвращаем указатель чтения файла в начало
		in.seekg(0, std::ios::beg);
		// Закрываем файл
		out.close();
	}
	// Закрываем файл
	in.close();

	// Закрытие крит. секции
	LeaveCriticalSection(&cs);

	MessageBox(0, _T("Копии созданы!"), _T("Информация"), MB_OK);

	return 0;
}

// Поток 2
DWORD WINAPI ReadFromFilesThread(LPVOID lp) {
	// Создание крит. секции
	EnterCriticalSection(&cs);

	mainDlg* ptr = (mainDlg*)lp;

	// Запись в файл
	std::wofstream out("all_music.txt", std::ios::out);

	int N = 4;

	wchar_t buff[254];

	// Создаю N кол-во копий
	for (int i = 1; i <= N; i++) {
		wsprintf(buff, _TEXT("music_copy_%d.txt"), i);

		// MessageBox(0, buff, _T("0"), MB_OK);

		// Чтение файла
		std::wifstream in(buff, std::ios::in);

		// Проверка на октрытие файла
		if (!in) {
			MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
			return 1;
		}

		// Перебираю открытый файл
		while (!in.eof()) {
			in.getline(buff, 254);

			out << buff << "\n";
		}

		// Сбрасываем флаги ошибок
		in.clear();
		// Возвращаем указатель чтения файла в начало
		in.seekg(0, std::ios::beg);
	}
	// Закрываем файл
	out.close();

	// Закрытие крит. секции
	LeaveCriticalSection(&cs);

	MessageBox(0, _T("Запись завершена!"), _T("Информация"), MB_OK);

	return 0;
}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	hDialog = hwnd;

	InitializeCriticalSection(&cs);

	HANDLE h;
	h = CreateThread(NULL, 0, WriteToFilesThread, 0, 0, NULL);
	CloseHandle(h);
	h = CreateThread(NULL, 0, ReadFromFilesThread, 0, 0, NULL);
	CloseHandle(h);

	return TRUE;
}

void mainDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	if (id == IDOK || id == IDCANCEL)
		EndDialog(hwnd, 0);
}

BOOL CALLBACK mainDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}