#include "ContextMenuDlg.h"

CContextMenuDlg* CContextMenuDlg::ptr = NULL;

CContextMenuDlg::CContextMenuDlg(void)
{
	ptr = this;
	dwButtonState = dwEditState = dwCheckboxState = MF_CHECKED;
}

void CContextMenuDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CContextMenuDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	// ������� ����������� ��������� ����������
	hEdit1 = GetDlgItem(hDialog, IDC_EDIT1);
	hEdit2 = GetDlgItem(hDialog, IDC_EDIT2);
	hButton1 = GetDlgItem(hDialog, IDC_BUTTON1);
	hButton2 = GetDlgItem(hDialog, IDC_BUTTON2);
	hCheck1 = GetDlgItem(hDialog, IDC_CHECK1);
	hCheck2 = GetDlgItem(hDialog, IDC_CHECK2);

	// �������� ���� �� �������� ����������
	hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));

	// ������� ����������� �������
	hSubMenu = GetSubMenu(hMenu, 0);
	hSubMenu2 = GetSubMenu(hSubMenu, 0);
	hSubMenu3 = GetSubMenu(hSubMenu, 1);

	// ��������� ������� �� ������ ������ � �����-������ ��������� ���� 
	CheckMenuRadioItem(hSubMenu3, ID_GROUP2_BUTTON, ID_GROUP2_CHECKBOX, ID_GROUP2_BUTTON, MF_BYCOMMAND);
	return TRUE;
}

// ���������� ��������� WM_COMMAND ����� ������ ��� ������ ������ ����
void CContextMenuDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id >= ID_GROUP1_BUTTON && id <= ID_GROUP1_CHECKBOX)
	{
		switch(id)
		{
			case ID_GROUP1_BUTTON:
				// ������ ������� ��������� ������ ����
				if(dwButtonState == MF_CHECKED)
					EnableWindow(hButton1, FALSE);
				else
					EnableWindow(hButton1, TRUE);
				// ������� ��������� ������ ���� �� ���������������, 
				// �.�. ���� ����� ���� ��� �������, �� ������ ������� � ��������
				dwButtonState ^= MF_CHECKED;
				CheckMenuItem(hSubMenu2, ID_GROUP1_BUTTON, MF_BYCOMMAND | dwButtonState);
				break;
			case ID_GROUP1_EDIT:
				if(dwEditState == MF_CHECKED)
					EnableWindow(hEdit1, FALSE);
				else
					EnableWindow(hEdit1, TRUE);
				dwEditState ^= MF_CHECKED;
				CheckMenuItem(hSubMenu2, ID_GROUP1_EDIT, MF_BYCOMMAND | dwEditState);
				break;
			case ID_GROUP1_CHECKBOX:
				if(dwCheckboxState == MF_CHECKED)
					EnableWindow(hCheck1, FALSE);
				else
					EnableWindow(hCheck1, TRUE);
				dwCheckboxState ^= MF_CHECKED;
				CheckMenuItem(hSubMenu2, ID_GROUP1_CHECKBOX, MF_BYCOMMAND | dwCheckboxState);
				break;
		}
	}
	if(id >= ID_GROUP2_BUTTON && id <= ID_GROUP2_CHECKBOX)
	{
		// ��������� ������� �� ��������� ������ � �����-������ ��������� ���� 
		CheckMenuRadioItem(hSubMenu3, ID_GROUP2_BUTTON, ID_GROUP2_CHECKBOX, id, MF_BYCOMMAND);
		switch(id)
		{
			case ID_GROUP2_BUTTON:
					EnableWindow(hButton2, TRUE);
					EnableWindow(hEdit2, FALSE);
					EnableWindow(hCheck2, FALSE);
					break;
			case ID_GROUP2_EDIT:
					EnableWindow(hButton2, FALSE);
					EnableWindow(hEdit2, TRUE);
					EnableWindow(hCheck2, FALSE);
					break;
			case ID_GROUP2_CHECKBOX:
					EnableWindow(hButton2, FALSE);
					EnableWindow(hEdit2, FALSE);
					EnableWindow(hCheck2, TRUE);
					break;
		}
	}
}

// ���������� ��������� WM_CONTEXTMENU ����� ������ ��� ������ ������ ������� ���� � ����� ������� ����������� ����
void CContextMenuDlg::Cls_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
	// ��������� ����������� ����,
	//����� ������� ���� �������� ������������� � ����� �������� ��������� ������� ����
	TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, xPos, yPos, 0, hDialog, NULL);
}

BOOL CALLBACK CContextMenuDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_CONTEXTMENU, ptr->Cls_OnContextMenu);
	}
	return FALSE;
}