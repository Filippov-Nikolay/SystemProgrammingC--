#include "mainDlg.h"

mainDlg* mainDlg::ptr = NULL;
bool isSave = false;

mainDlg::mainDlg(void) {
	ptr = this;
}

mainDlg::~mainDlg(void) {

}

BOOL mainDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
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
	}
	else if (id == IDC_LIST1 && codeNotify == LBN_SELCHANGE || id == IDC_LIST2 && codeNotify == LBN_SELCHANGE || id == IDC_LIST3 && codeNotify == LBN_SELCHANGE) {
		// ��������� ��������
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// ������� ������ ��������� �� �������
		SendMessage(hList1, LB_SETCURSEL, index, 0);
		SendMessage(hList2, LB_SETCURSEL, index, 0);
		SendMessage(hList3, LB_SETCURSEL, index, 0);

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

		isSave = true;
	}
	else if (id == IDC_BUTTON4 && isSave) {
		// Save

		// ��������� ��������
		int index = SendMessage(hList1, LB_GETCURSEL, 0, 0);

		// ��������� ������ �� edit
		GetWindowText(hEdit1, buffTask, max_size);
		GetWindowText(hEdit2, buffTime, max_size);

		// �������� ���������� �� �������
		SendMessage(hList1, LB_DELETESTRING, index, 0);
		SendMessage(hList2, LB_DELETESTRING, index, 0);
		SendMessage(hList3, LB_DELETESTRING, index, 0);

		// ������� ������ �� �������
		SendMessage(hList1, LB_INSERTSTRING, index, LPARAM(buffTask));
		SendMessage(hList2, LB_INSERTSTRING, index, LPARAM(buffTime));

		// ��������� �������� ������ �� save
		SetWindowText(hButtons[3], _T("Edit"));

		// ������� edit, ����� �����
		SetWindowText(hEdit1, _T(""));
		SetWindowText(hEdit2, _T(""));

		isSave = false;

	}
	else if (id == IDC_BUTTON5) {
		EndDialog(hwnd, 0);
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