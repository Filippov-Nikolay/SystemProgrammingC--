#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;
bool isSave = false;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {

}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	// ������� ���������� ���������� ����������
	HINSTANCE hInst = GetModuleHandle(NULL);

	// �������� ������
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDB_PNG1));

	// ������������� ������ � ������� ���� ����������
	SetClassLong(hwnd, GCL_HICON, LONG(hIcon));

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

	// ���������� � ������ ������
	for (int i = 0; i < 3; i++) {
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
	}

	return TRUE;
}

void mainDlg::Cls_OnClose(HWND hwnd) {
	EndDialog(hwnd, 0);
}

void mainDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	const int max_size = 250;
	_TCHAR buffTask[max_size] = _T("");
	_TCHAR buffTime[max_size] = _T("");
	
	if (id == IDC_BUTTON1) {
		// Add		

		// ��������� ������ �� edit
		GetWindowText(hEdit1, buffTask, max_size);
		GetWindowText(hEdit2, buffTime, max_size);

		if (_tcsclen(buffTask) == 0 || _tcsclen(buffTime) == 0) {
			MessageBox(hwnd, _T("�� ��������� �� ��� ����!"), _T("����������"), MB_OK);
			return;
		}

		if (isTimeFormat(buffTime)) {
			MessageBox(hwnd, _T("������� � �������: ����:������:�������!"), _T("����������"), MB_OK);
			return;
		}

		// ���������� � ������ ������
		SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buffTask));
		SendMessage(hList2, LB_ADDSTRING, 0, LPARAM(buffTime));
		SendMessage(hList3, LB_ADDSTRING, 0, LPARAM(_T("Not done")));

		// ������� edit, ����� �����
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));
	}
	else if (id == IDC_BUTTON2) {
		// Done

		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// �������� �������� �� �������
		SendMessage(hList3, LB_DELETESTRING, index, 0);

		// ������� ������ ������ �� ��� �� ������
		SendMessage(hList3, LB_INSERTSTRING, index, LPARAM(_T("Done")));

		// ��������� ��������� �����
		updateSelected();
	}
	else if (id == IDC_LIST1 && codeNotify == LBN_SELCHANGE || id == IDC_LIST2 && codeNotify == LBN_SELCHANGE || id == IDC_LIST3 && codeNotify == LBN_SELCHANGE) {
		
		// ��������� ��������� �����
		updateSelected();

		// ��������� ������ ���������������
		EnableWindow(hButtons[3], TRUE);
		EnableWindow(hButtons[2], TRUE);
		EnableWindow(hButtons[1], TRUE);
	}
	else if (id == IDC_BUTTON3) {
		// Delete

		// ��������� ��������
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// �������� ���������� �� �������
		SendMessage(hList1, LB_DELETESTRING, index, 0);
		SendMessage(hList2, LB_DELETESTRING, index, 0);
		SendMessage(hList3, LB_DELETESTRING, index, 0);
	}
	else if (id == IDC_BUTTON4 && !isSave) {
		// Edit

		// ��������� ��������
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// �������� ����� �� �������
		SendMessage(hList1, LB_GETTEXT, index, LPARAM(buffTask));
		SendMessage(hList2, LB_GETTEXT, index, LPARAM(buffTime));

		// ���������� ����� � edit
		SetWindowText(hEdit1, buffTask);
		SetWindowText(hEdit2, buffTime);

		// ��������� �������� ������ �� save
		SetWindowText(hButtons[3], _T("Save"));

		// ���������� ������ ����������
		EnableWindow(hButtons[0], FALSE);

		isSave = true;
	}
	else if (id == IDC_BUTTON4 && isSave) {
		// Save

		// ��������� ��������
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// ��������� ������ �� edit
		GetWindowText(hEdit1, buffTask, max_size);
		GetWindowText(hEdit2, buffTime, max_size);

		if (_tcsclen(buffTask) == 0 || _tcsclen(buffTime) == 0) {
			MessageBox(hwnd, _T("�� ��������� �� ��� ����!"), _T("����������"), MB_OK);
			return;
		}

		if (isTimeFormat(buffTime)) {
			MessageBox(hwnd, _T("������� � �������: ����:������:�������!"), _T("����������"), MB_OK);
			return;
		}

		// �������� ���������� �� �������
		SendMessage(hList1, LB_DELETESTRING, index, 0);
		SendMessage(hList2, LB_DELETESTRING, index, 0);

		// ������� ������ �� �������
		SendMessage(hList1, LB_INSERTSTRING, index, LPARAM(buffTask));
		SendMessage(hList2, LB_INSERTSTRING, index, LPARAM(buffTime));

		// ��������� �������� ������ �� save
		SetWindowText(hButtons[3], _T("Edit"));

		// ������� edit, ����� �����
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));

		isSave = false;

		// ��������� ��������� �����
		updateSelected();

		// ��������� ������ ����������
		EnableWindow(hButtons[0], TRUE);

	}
	else if (id == IDC_BUTTON5) {
		EndDialog(hwnd, 0);
	}
}

bool mainDlg::isTimeFormat(_TCHAR* text) {
	const int lengthText = _tcsclen(text);

	// �������� �� ���-�� ��������
	if (lengthText >= 9 || lengthText < 8)
		return true;

	// �������� �� ������
	if (text[2] != _T(':') || text[5] != _T(':'))
		return true;

	_TCHAR temp[3] = _T("");

	for (int i = 0; i < lengthText; i++) {
		// �������� �� ������� ����
		if (isalpha(text[i])) { 
			return true;
		}

		// �������� �� ���������� ���� �����
		if (text[i] != _T(':')) {
			temp[0] = text[i];
			temp[1] = text[i + 1];
			temp[2] = _T('\0');

			int number = _tstoi(temp);

			if (i == 0 && (number > 23 || number <= 0))
				return true;
			else if (i == 3 && (number > 59 || number < 0))
				return true;
			else if (i == 5 && (number > 59 || number < 0))
				return true;
		}
	}

	return false;
}

void mainDlg::updateSelected() {
	// ��������� ��������
	int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

	// ������� ������ ��������� �� �������
	SendMessage(hList1, LB_SETCURSEL, index, 0);
	SendMessage(hList2, LB_SETCURSEL, index, 0);
	SendMessage(hList3, LB_SETCURSEL, index, 0);
}

BOOL CALLBACK mainDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}

	return FALSE;
}