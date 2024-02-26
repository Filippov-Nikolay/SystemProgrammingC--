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
	char buf[4096];
	ifstream in(TEXT("music.txt"), ios::in);
	if(!in)
	{
		MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{043CD612-77B6-45DF-AFE8-11C6EC2461DC}"));
	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);
	/*if(dwAnswer == WAIT_OBJECT_0)
	{*/
		ofstream out("coding.txt", ios::out | ios::trunc);
		const int KEY = 100;
		while(!in.eof())
		{
			in.read(buf, 100);			
			out.write(buf, 100);
		}
		out.close();
		in.close();
		ReleaseMutex(hMutex);
		MessageBox(ptr->hDialog, TEXT("Запись данных в файл coding.txt завершена!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
	/*}*/
	
	return 0;
}

BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	CreateThread(NULL, 0, Coding_Thread, this, 0, NULL); 
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