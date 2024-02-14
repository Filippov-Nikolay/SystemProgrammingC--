#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"
#include <time.h>

using namespace std;

#define IDC_CHECK3 (WM_USER + 1)

HINSTANCE hInstance;
const int numberButtons = 9;
int countMove = 0;
bool fFirstMove = true;
bool arrayMove[numberButtons] = {true};
int arrayMovePc[numberButtons] = {-1};
HWND gameFieldButtons[numberButtons]{ 0 };
HBITMAP hBitmapCross;


INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
			if (LOWORD(wParam) == IDC_BUTTON10 && HIWORD(wParam) == BN_CLICKED) {
				for (int i = 0; i < numberButtons; i++) {
					EnableWindow(gameFieldButtons[i], TRUE);

					//if (i % 2 == 0) {
					//	SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("X"));
					//	//SendMessage(gameFieldButtons[i], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapCross);
					//}
					//else {
					//	SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("0"));
					//}
				}
			}
			else {
				for (int i = 0; i < numberButtons; i++) {
					if (LOWORD(wParam) == IDC_BUTTON1 + i && HIWORD(wParam) == BN_CLICKED) {
						SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("X"));
						EnableWindow(gameFieldButtons[i], FALSE);
						arrayMove[i] = false;
					}
				}

				_TCHAR buff[250] = _TEXT("");

				

				time(NULL);
				int move = 0;
				bool f = true;

				// true, true, true, true
				// true, true, false, true

				do {
					move = rand() % 8 + 0;
					arrayMove[move] = false;
					arrayMovePc[countMove] = move;

					if (countMove != 0) {
						for (int i = 0; i < countMove; i++) {
							if (move == arrayMovePc[i] && arrayMove[arrayMovePc[i]]) {
								f = true;
								break;
							}
							else {
								f = false;
								countMove++;
							}
						}
					}
				} while (f);

				/*while (f) {
					move = rand() % 8 + 0;
					arrayMove[move] = false;
					arrayMovePc[countMove] = move;

					if (!arrayMove[move] && arrayMove[arrayMovePc[countMove]] != move) {
						countMove++;
						break;
					}
				}*/
				

				wsprintf(buff, _TEXT("%d"), move);
				SetWindowText(hwnd, buff);

				
				for (int i = 0; i < numberButtons; i++) {
					if (move == IDC_BUTTON1 + i && HIWORD(wParam) == BN_CLICKED) {
						SendMessage(gameFieldButtons[i], WM_SETTEXT, 0, (LPARAM)_TEXT("O"));
						EnableWindow(gameFieldButtons[i], FALSE);
					}
				}
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