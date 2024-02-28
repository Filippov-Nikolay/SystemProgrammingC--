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

// ����� 1
DWORD WINAPI WriteToFilesThread(LPVOID lp) {
	// �������� ����. ������
	EnterCriticalSection(&cs);

	mainDlg* ptr = (mainDlg*)lp;

	// ������ �����
	std::wifstream in(_TEXT("music.txt", std::ios::in));

	int N = 4;

	// �������� �� �������� �����
	if (!in) {
		MessageBox(ptr->hDialog, TEXT("������ �������� �����!"), TEXT("�������"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}

	wchar_t buff[254];

	// ������ N ���-�� �����
	for (int i = 1; i <= N; i++) {
		// �������� ����� � buff
		wsprintf(buff, _TEXT("music_copy_%d.txt"), i);

		// MessageBox(0, buff, _T("0"), MB_OK);

		// ������ � ����
		std::wofstream out(buff, std::ios::out);

		// ��������� �������� ����
		while (!in.eof()) {
			in.getline(buff, 254);

			out << buff << "\r\n";
		}

		// ���������� ����� ������
		in.clear();
		// ���������� ��������� ������ ����� � ������
		in.seekg(0, std::ios::beg);
		// ��������� ����
		out.close();
	}
	// ��������� ����
	in.close();

	// �������� ����. ������
	LeaveCriticalSection(&cs);

	MessageBox(0, _T("����� �������!"), _T("����������"), MB_OK);

	return 0;
}

// ����� 2
DWORD WINAPI ReadFromFilesThread(LPVOID lp) {
	// �������� ����. ������
	EnterCriticalSection(&cs);

	mainDlg* ptr = (mainDlg*)lp;

	// ������ � ����
	std::wofstream out("all_music.txt", std::ios::out);

	int N = 4;

	wchar_t buff[254];

	// ������ N ���-�� �����
	for (int i = 1; i <= N; i++) {
		wsprintf(buff, _TEXT("music_copy_%d.txt"), i);

		// MessageBox(0, buff, _T("0"), MB_OK);

		// ������ �����
		std::wifstream in(buff, std::ios::in);

		// �������� �� �������� �����
		if (!in) {
			MessageBox(ptr->hDialog, TEXT("������ �������� �����!"), TEXT("�������"), MB_OK | MB_ICONINFORMATION);
			return 1;
		}

		// ��������� �������� ����
		while (!in.eof()) {
			in.getline(buff, 254);

			out << buff << "\n";
		}

		// ���������� ����� ������
		in.clear();
		// ���������� ��������� ������ ����� � ������
		in.seekg(0, std::ios::beg);
	}
	// ��������� ����
	out.close();

	// �������� ����. ������
	LeaveCriticalSection(&cs);

	MessageBox(0, _T("������ ���������!"), _T("����������"), MB_OK);

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