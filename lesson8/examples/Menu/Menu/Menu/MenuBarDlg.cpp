#include "MenuBarDlg.h"

CMenuBarDlg* CMenuBarDlg::ptr = NULL;

CMenuBarDlg::CMenuBarDlg(void)
{
	ptr = this;
	bShowStatusBar = TRUE;
}

void CMenuBarDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CMenuBarDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	// ������� ���������� ���������� ����
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	// �������� ������ ���������
	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS | SBARS_SIZEGRIP, 0, hDialog, WM_USER);
	// �������� ���� �� �������� ����������
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
	// ����������� ���� � �������� ���� ����������
	SetMenu(hDialog, hMenu);
	return TRUE;
}

// ���������� ��������� WM_COMMAND ����� ������ ��� ������ ������ ����
void CMenuBarDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
	case ID_UNDO:
		// ������� ��������� ��������
		SendMessage(hEdit, WM_UNDO, 0, 0); 
		break;
	case ID_CUT:
		// ������ ���������� �������� ������ � ����� ������
		SendMessage(hEdit, WM_CUT, 0, 0); 
		break;
	case ID_COPY:
		// ��������� ���������� �������� ������ � ����� ������
		SendMessage(hEdit, WM_COPY, 0, 0); 
		break;
	case ID_PASTE:
		// ������� ����� � Edit Control �� ������ ������
		SendMessage(hEdit, WM_PASTE, 0, 0); 
		break;
	case ID_DEL:
		// ������ ���������� �������� ������
		SendMessage(hEdit, WM_CLEAR, 0, 0); 
		break;
	case ID_SELECTALL:
		// ������� ���� ����� � Edit Control
		SendMessage(hEdit, EM_SETSEL, 0, -1); 
		break;
	case ID_STATUSBAR:
		
		RECT rect1, rect2={0};
		// ������� ���������� ���������� ������� �������� ����
		GetClientRect(hDialog, &rect1);
		// ���� ���� ����� TRUE, �� ������ ��������� ����������
		if(bShowStatusBar) 
		{
			// ������� ���������� �������� ����
			HMENU hMenu = GetMenu(hDialog); 
			// ������ ������� � ������ ���� "������ ���������"
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_UNCHECKED);
			// ������ ������ ���������
			ShowWindow(hStatus, SW_HIDE); 
		}
		else
		{
			// ������� ���������� �������� ����
			HMENU hMenu = GetMenu(hDialog);
			// ��������� ������� �� ������ ���� "������ ���������"
			CheckMenuItem(hMenu,ID_STATUSBAR, MF_BYCOMMAND | MF_CHECKED);
			SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2); 			
			// ��������� ������ ���������
			ShowWindow(hStatus, SW_SHOW); 
		}
		// ��������� ����� ������� ���������� ����
		MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
		bShowStatusBar = !bShowStatusBar;
	}
}

// ���������� ��������� WM_SIZE ����� ������ ��� ��������� �������� �������� ����
// ���� ��� ������������/�������������� �������� ����
void CMenuBarDlg::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rect1, rect2 = {0};
	// ������� ���������� ���������� ������� �������� ����
	GetClientRect(hDialog, &rect1);
	if(bShowStatusBar) 
		// ������� ���������� ������������ ������ ������ ���������
		SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2); 
	// ��������� ����� ������� ���������� ����
	MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
	// ��������� ������ ������ ���������, 
	// ������ ������ ���������� ������� �������� ����
	SendMessage(hStatus, WM_SIZE, 0, 0);
}

// ���������� WM_INITMENUPOPUP ����� ������ ��������������� 
// ����� ������������ ������������ ����
void CMenuBarDlg::Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu) 
{
	if(item == 0) // �������������� ����� ���� "������"
	{
		// ������� ������� ��������� ������
		DWORD dwPosition = SendMessage(hEdit, EM_GETSEL, 0, 0); 
		WORD wBeginPosition = LOWORD(dwPosition);
		WORD wEndPosition = HIWORD(dwPosition);

		if(wEndPosition != wBeginPosition) // ������� �� �����?
		{
			// ���� ������� ���������� �����, 
			// �� ������� ������������ ������ ���� "����������", "��������" � "�������"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_ENABLED );
		}
		else
		{
			// ���� ����������� ���������� �����, 
			// �� ������� ������������ ������ ���� "����������", "��������" � "�������"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_GRAYED);
		}

		if(IsClipboardFormatAvailable(CF_TEXT)) // ������� �� ����� � ������ ������?
			// ���� ������� ����� � ������ ������, 
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_ENABLED);
		else 
			// ���� ����������� ����� � ������ ������, 
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_GRAYED);

		// ���������� �� ����������� ������ ���������� ��������?
		if(SendMessage(hEdit, EM_CANUNDO, 0, 0)) 
			// ���� ���������� ����������� ������ ���������� ��������,
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_ENABLED);
		else 
			// ���� ����������� ����������� ������ ���������� ��������,
			// �� ������� ����������� ����� ���� "��������"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_GRAYED);

		// ��������� ����� ������ � Edit Control
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0); 
		// ������� �� ���� ����� � Edit Control?
		if(length != wEndPosition - wBeginPosition) 
			//���� �� ���� ����� ������� � Edit Control,
			// �� ������� ����������� ����� ���� "�������� ��"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_ENABLED);
		else 
			// ���� ������� ���� ����� � Edit Control,
			// �� ������� ����������� ����� ���� "�������� ��"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_GRAYED);
	}
}

void CMenuBarDlg::Cls_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	if(flags & MF_POPUP) // ��������, �������� �� ���������� ����� ���� ���������� ����������� �������?
	{
		// ���������� ����� ���� �������� ���������� ����������� �������
		SendMessage(hStatus, SB_SETTEXT, 0, 0); // ������� ����� �� ������ ���������
	}
	else
	{
		// ���������� ����� ���� �������� �������� ������� (����� ���� "�������")
		TCHAR buf[200];
		// ������� ���������� �������� ���������� ����������
		HINSTANCE hInstance = GetModuleHandle(NULL);
		// ������� ������ �� ������� �����, ������������� � �������� ����������
		// ��� ���� ������������� ����������� ������ ������ ������������� �������������� ����������� ������ ����
		LoadString(hInstance, item, buf, 200);
		// ������� � ������ ��������� ����������� �������, ��������������� ����������� ������ ����
		SendMessage(hStatus, SB_SETTEXT, 0, LPARAM(buf)); 
	}
}

BOOL CALLBACK CMenuBarDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
		HANDLE_MSG(hwnd, WM_INITMENUPOPUP, ptr->Cls_OnInitMenuPopup);
		HANDLE_MSG(hwnd, WM_MENUSELECT, ptr->Cls_OnMenuSelect);
	}
	return FALSE;
}