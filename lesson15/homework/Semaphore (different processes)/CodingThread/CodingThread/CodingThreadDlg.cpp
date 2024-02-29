#include "CodingThreadDlg.h"

CodingThreadDlg* CodingThreadDlg::ptr = NULL;
CodingThreadDlg::CodingThreadDlg(void)
{
	ptr = this;
}

CodingThreadDlg::~CodingThreadDlg(void)
{
	
}

void CodingThreadDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Coding_Thread(LPVOID lp)
{
	CodingThreadDlg *ptr = (CodingThreadDlg *)lp;

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));

	char buf[4096];
	ifstream in(TEXT("music.txt"), ios::binary | ios::in);
	if(!in)
	{
		MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Семафор"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}
	
	if(WaitForSingleObject(hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		/*MessageBox(0, _T("JHDGFSDF"), _T("DJKDHSF"), 0);*/

		ofstream out("coding.txt", ios::binary | ios::out | ios::trunc);
		const int KEY = 100;
		while(!in.eof())
		{
			in.read(buf, 4096);
			int n = in.gcount();
			for(int i = 0; i < n; i++)
			{
				buf[i] ^= KEY;
			}
			out.write(buf, n);
		}
		out.close();
		MessageBox(ptr->hDialog, TEXT("Запись данных в файл coding.txt завершена!"), TEXT("Семафор"), MB_OK | MB_ICONINFORMATION);
	}
	in.close();

	SetEvent(hEvent);

	return 0;
}

BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;

	HANDLE hEvent = CreateEvent(NULL, FALSE /*автоматический сброс события */, FALSE /* несигнальное состояние */, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));
	
	CreateThread(NULL, 0, Coding_Thread, this, 0, NULL);

	HANDLE h = OpenEvent(EVENT_ALL_ACCESS, 0, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));
	SetEvent(h);

	return TRUE;
}


BOOL CALLBACK CodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
	}
	return FALSE;
}