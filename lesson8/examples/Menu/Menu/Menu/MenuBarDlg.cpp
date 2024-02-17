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
	// Получим дескриптор текстового поля
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	// Создадим строку состояния
	hStatus = CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_TOOLTIPS | SBARS_SIZEGRIP, 0, hDialog, WM_USER);
	// Загрузим меню из ресурсов приложения
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
	// Присоединим меню к главному окну приложения
	SetMenu(hDialog, hMenu);
	return TRUE;
}

// Обработчик сообщения WM_COMMAND будет вызван при выборе пункта меню
void CMenuBarDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch(id)
	{
	case ID_UNDO:
		// Отменим последнее действие
		SendMessage(hEdit, WM_UNDO, 0, 0); 
		break;
	case ID_CUT:
		// Удалим выделенный фрагмент текста в буфер обмена
		SendMessage(hEdit, WM_CUT, 0, 0); 
		break;
	case ID_COPY:
		// Скопируем выделенный фрагмент текста в буфер обмена
		SendMessage(hEdit, WM_COPY, 0, 0); 
		break;
	case ID_PASTE:
		// Вставим текст в Edit Control из буфера обмена
		SendMessage(hEdit, WM_PASTE, 0, 0); 
		break;
	case ID_DEL:
		// Удалим выделенный фрагмент текста
		SendMessage(hEdit, WM_CLEAR, 0, 0); 
		break;
	case ID_SELECTALL:
		// Выделим весь текст в Edit Control
		SendMessage(hEdit, EM_SETSEL, 0, -1); 
		break;
	case ID_STATUSBAR:
		
		RECT rect1, rect2={0};
		// Получим координаты клиентской области главного окна
		GetClientRect(hDialog, &rect1);
		// Если флаг равен TRUE, то строка состояния отображена
		if(bShowStatusBar) 
		{
			// Получим дескриптор главного меню
			HMENU hMenu = GetMenu(hDialog); 
			// Снимем отметку с пункта меню "Строка состояния"
			CheckMenuItem(hMenu, ID_STATUSBAR, MF_BYCOMMAND | MF_UNCHECKED);
			// Скроем строку состояния
			ShowWindow(hStatus, SW_HIDE); 
		}
		else
		{
			// Получим дескриптор главного меню
			HMENU hMenu = GetMenu(hDialog);
			// Установим отметку на пункте меню "Строка состояния"
			CheckMenuItem(hMenu,ID_STATUSBAR, MF_BYCOMMAND | MF_CHECKED);
			SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2); 			
			// Отобразим строку состояния
			ShowWindow(hStatus, SW_SHOW); 
		}
		// Установим новые размеры текстового поля
		MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
		bShowStatusBar = !bShowStatusBar;
	}
}

// Обработчик сообщения WM_SIZE будет вызван при изменении размеров главного окна
// либо при сворачивании/восстановлении главного окна
void CMenuBarDlg::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT rect1, rect2 = {0};
	// Получим координаты клиентской области главного окна
	GetClientRect(hDialog, &rect1);
	if(bShowStatusBar) 
		// Получим координаты единственной секции строки состояния
		SendMessage(hStatus, SB_GETRECT, 0, (LPARAM)&rect2); 
	// Установим новые размеры текстового поля
	MoveWindow(hEdit, rect1.left, rect1.top, rect1.right, rect1.bottom - (rect2.bottom - rect2.top), 1);
	// Установим размер строки состояния, 
	// равный ширине клиентской области главного окна
	SendMessage(hStatus, WM_SIZE, 0, 0);
}

// Обработчик WM_INITMENUPOPUP будет вызван непосредственно 
// перед активизацией всплывающего меню
void CMenuBarDlg::Cls_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu) 
{
	if(item == 0) // Активизируется пункт меню "Правка"
	{
		// Получим границы выделения текста
		DWORD dwPosition = SendMessage(hEdit, EM_GETSEL, 0, 0); 
		WORD wBeginPosition = LOWORD(dwPosition);
		WORD wEndPosition = HIWORD(dwPosition);

		if(wEndPosition != wBeginPosition) // Выделен ли текст?
		{
			// Если имеется выделенный текст, 
			// то сделаем разрешёнными пункты меню "Копировать", "Вырезать" и "Удалить"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_ENABLED );
		}
		else
		{
			// Если отсутствует выделенный текст, 
			// то сделаем недоступными пункты меню "Копировать", "Вырезать" и "Удалить"
			EnableMenuItem(hMenu, ID_COPY, MF_BYCOMMAND | MF_GRAYED );
			EnableMenuItem(hMenu, ID_CUT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_DEL, MF_BYCOMMAND | MF_GRAYED);
		}

		if(IsClipboardFormatAvailable(CF_TEXT)) // Имеется ли текст в буфере обмена?
			// Если имеется текст в буфере обмена, 
			// то сделаем разрешённым пункт меню "Вставить"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_ENABLED);
		else 
			// Если отсутствует текст в буфере обмена, 
			// то сделаем недоступным пункт меню "Вставить"
			EnableMenuItem(hMenu, ID_PASTE, MF_BYCOMMAND | MF_GRAYED);

		// Существует ли возможность отмены последнего действия?
		if(SendMessage(hEdit, EM_CANUNDO, 0, 0)) 
			// Если существует возможность отмены последнего действия,
			// то сделаем разрешённым пункт меню "Отменить"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_ENABLED);
		else 
			// Если отсутствует возможность отмены последнего действия,
			// то сделаем недоступным пункт меню "Отменить"
			EnableMenuItem(hMenu, ID_UNDO, MF_BYCOMMAND | MF_GRAYED);

		// Определим длину текста в Edit Control
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0); 
		// Выделен ли весь текст в Edit Control?
		if(length != wEndPosition - wBeginPosition) 
			//Если не весь текст выделен в Edit Control,
			// то сделаем разрешённым пункт меню "Выделить всё"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_ENABLED);
		else 
			// Если выделен весь текст в Edit Control,
			// то сделаем недоступным пункт меню "Выделить всё"
			EnableMenuItem(hMenu, ID_SELECTALL, MF_BYCOMMAND | MF_GRAYED);
	}
}

void CMenuBarDlg::Cls_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	if(flags & MF_POPUP) // Проверим, является ли выделенный пункт меню заголовком выпадающего подменю?
	{
		// Выделенный пункт меню является заголовком выпадающего подменю
		SendMessage(hStatus, SB_SETTEXT, 0, 0); // Убираем текст со строки состояния
	}
	else
	{
		// Выделенный пункт меню является конечным пунктом (пункт меню "команда")
		TCHAR buf[200];
		// Получим дескриптор текущего экземпляра приложения
		HINSTANCE hInstance = GetModuleHandle(NULL);
		// Зарузим строку из таблицы строк, расположенной в ресурсах приложения
		// При этом идентификатор загружаемой строки строго соответствует идентификатору выделенного пункта меню
		LoadString(hInstance, item, buf, 200);
		// Выводим в строку состояния контекстную справку, соответствующую выделенному пункту меню
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