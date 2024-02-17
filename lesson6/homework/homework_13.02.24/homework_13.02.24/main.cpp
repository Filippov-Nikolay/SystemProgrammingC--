#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"
#include <time.h>

using namespace std;

#define IDC_CHECK3 (WM_USER + 1)

HINSTANCE hInstance;
const int numberButtons = 9;
int gameResult = 0;
int countMove = numberButtons;
bool fFirstMove = true;
bool fStartPlayBtn = true;
bool stopPc = false;
bool stopPlayer = false;
bool arrayMove[numberButtons] = { true, true, true,
								  true, true, true,
								  true, true, true };
//int arrayMovePc[numberButtons] = {-1};

// player = 1
// PC = 0
int arrayField[numberButtons] = { -1, -1, -1,
								  -1, -1, -1,
								  -1, -1, -1 };
HWND gameFieldButtons[numberButtons]{ 0 };
HBITMAP hBitmapCross;


INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void ComputerMove(HWND);
int WhoHasWon(HWND, bool);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	// hBitmapCross = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PNG1));
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE: {
			EndDialog(hwnd, 0);

			return TRUE;
		}
		case WM_INITDIALOG: {
			SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, WPARAM(BST_CHECKED), 0);

			gameFieldButtons[0] = GetDlgItem(hwnd, IDC_BUTTON1);
			gameFieldButtons[1] = GetDlgItem(hwnd, IDC_BUTTON2);
			gameFieldButtons[2] = GetDlgItem(hwnd, IDC_BUTTON3);
			gameFieldButtons[3] = GetDlgItem(hwnd, IDC_BUTTON4);
			gameFieldButtons[4] = GetDlgItem(hwnd, IDC_BUTTON5);
			gameFieldButtons[5] = GetDlgItem(hwnd, IDC_BUTTON6);
			gameFieldButtons[6] = GetDlgItem(hwnd, IDC_BUTTON7);
			gameFieldButtons[7] = GetDlgItem(hwnd, IDC_BUTTON8);
			gameFieldButtons[8] = GetDlgItem(hwnd, IDC_BUTTON9);

			for (int i = 0; i < numberButtons; i++) {
				EnableWindow(gameFieldButtons[i], FALSE);
			}

			// SendMessage(gameFieldButtons[1], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapCross);
			// SendDlgItemMessage(hwnd, IDC_BUTTON2, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hBitmapCross);
		}
		case WM_COMMAND: {
			if (LOWORD(wParam) == IDC_BUTTON10 && HIWORD(wParam) == BN_CLICKED && fStartPlayBtn) {
				for (int i = 0; i < numberButtons; i++) {
					EnableWindow(gameFieldButtons[i], TRUE);
					SendMessage(gameFieldButtons[i], WM_CLEAR, 0, 0);

					//if (i % 2 == 0) {
					//	SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("X"));
					//	//SendMessage(gameFieldButtons[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapCross);
					//}
					//else {
					//	SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("0"));
					//}
				}
				LRESULT result = SendDlgItemMessage(hwnd, IDC_RADIO1, BM_GETCHECK, 0, 0);

				result == BST_CHECKED ? fFirstMove = true : fFirstMove = false;

				if (!fFirstMove)
					ComputerMove(hwnd);

				fStartPlayBtn = false;
			}
			else if (LOWORD(wParam) <= IDC_BUTTON9 && HIWORD(wParam) == BN_CLICKED) {
				if (!stopPlayer) {
					for (int i = 0; i < numberButtons; i++) {
						if (LOWORD(wParam) == IDC_BUTTON1 + i && HIWORD(wParam) == BN_CLICKED) {
							arrayMove[i] = false;
							arrayField[i] = 1;
							countMove--;
							SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("X"));
							EnableWindow(gameFieldButtons[i], FALSE);
						}
					}
					gameResult = WhoHasWon(hwnd, true);
					gameResult == -1 ? NULL : gameResult == 1 ? stopPc = true : gameResult == 0 ? stopPlayer = false : NULL;
				}
					
				if (!stopPc) {
					ComputerMove(hwnd);
					gameResult = WhoHasWon(hwnd, false);
					gameResult == -1 ? NULL : gameResult == 1 ? stopPc = false : gameResult == 0 ? stopPlayer = true : NULL;
				}


				if (stopPlayer || stopPc) {
					for (int i = 0; i < numberButtons; i++) {
						EnableWindow(gameFieldButtons[i], FALSE);
					}
				}
				
				if (countMove == 0 || stopPlayer || stopPc) {
					int msgboxID = MessageBox(
						NULL,
						(LPCWSTR)_TEXT("Хотите сыграть ещё?"),
						(LPCWSTR)_TEXT("Хотите сыграть ещё?"),
						MB_ICONQUESTION | MB_YESNO);

					if (msgboxID == IDYES) {
						stopPlayer = stopPc = false;

						for (int i = 0; i < numberButtons; i++) {
							SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT(""));
							EnableWindow(gameFieldButtons[i], FALSE);
							fStartPlayBtn = true;
							countMove = numberButtons;
							gameResult = 0;

							for (int j = 0; j < numberButtons; j++) {
								arrayField[j] = -1;
								arrayMove[j] = true;
							}
						}
					}
					else {
						EndDialog(hwnd, 0);
					}
				}
				

				

				// true, true, true, true
				// 
				// arrayMove = {
				// true, true, true
				// true, true, true
				// true, true, true }


				

				/*if (countMove != numberButtons) {
					do {
						move = rand() % numberButtons;
						f = false;

						for (int j = 0; j < countMove; j++) {
							if (arrayMovePc[j] == move) {
								f = true;
								break;
							}
						}
					} while (f);

					arrayMovePc[countMove++] = move;
				}*/	
			}



			/*else if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
				SendMessage(gameFieldButtons[0], WM_SETTEXT, 0, (LPARAM)_TEXT("X"));
			}
			else if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {
				SendMessage(gameFieldButtons[1], WM_SETTEXT, 0, (LPARAM)_TEXT("X"));
			}*/

			return TRUE;
		}
	}

	return FALSE;
}

int WhoHasWon(HWND hwnd, bool isPlayer) {
	// player = 1
	// PC = 0

	bool fWin = false;
	int countFigure = -1;
	/*bool isPlayer = true;*/

	for (int i = 0; i < 3; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;

	for (int i = 3; i < 6; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;

	for (int i = 6; i < 9; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;



	for (int i = 0; i < 7; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0) && (i == 0 || i == 3 || i == 6)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;

	for (int i = 1; i < 8; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0) && (i == 1 || i == 4 || i == 7)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;

	for (int i = 2; i < 9; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0) && (i == 2 || i == 5 || i == 8)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;



	for (int i = 0; i < 9; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0) && (i == 0 || i == 4 || i == 8)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;

	for (int i = 0; i < 9; i++) {
		if (arrayField[i] == (isPlayer ? 1 : 0) && (i == 2 || i == 4 || i == 6)) {
			countFigure++;
		}

		if (countFigure == 2) {
			MessageBox(hwnd, _TEXT("WIN"), isPlayer ? _TEXT("Player") : _TEXT("PC"), 0);
			fWin = true;

			if (fWin) {
				return isPlayer ? 1 : 0;
			}
		}
	}
	countFigure = -1;

	if (countMove == 0)
		MessageBox(hwnd, _TEXT("DEAD HEAT"), _TEXT("Player & PC"), 0);

	return -1;
	/*for (int i = 0; i < 3; i++) {
		if (arrayField[i] == 1 && i == 2) {
			MessageBox(hwnd, _TEXT("WIN"), _TEXT("Player"), 0);
		}
		else
			return;
	}*/
	
}
void ComputerMove(HWND hwnd) {
	_TCHAR buff[250] = _TEXT("");

	srand(time(NULL));
	int move = 0;
	bool f = true;
	countMove--;
	/*do {
		move = rand() % 9 + 0;

		if (arrayMove[move]) {
			f = false;
			arrayMove[move] = false;
		}
		for (int i = 0; i < numberButtons; i++) {
			if (!arrayMove[i] && numberButtons == i)
				fAll = false;
		}
	} while (f && fAll);*/

	if (countMove >= 0) {
		do {
			move = rand() % 9;

			if (arrayMove[move]) {
				arrayMove[move] = false;
				arrayField[move] = 0;
				f = false;
			}


		} while (f);

		for (int i = 0; i < numberButtons; i++) {
			if (move == i) {
				SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("O"));
				EnableWindow(gameFieldButtons[i], FALSE);
			}
		}
	}
	

	/*wsprintf(buff, _TEXT("%d. %d, %d, %d, %d, %d, %d, %d, %d, %d"), move,
		arrayMove[0],
		arrayMove[1],
		arrayMove[2],
		arrayMove[3],
		arrayMove[4],
		arrayMove[5],
		arrayMove[6],
		arrayMove[7],
		arrayMove[8]);
	SetWindowText(hwnd, buff);*/
}