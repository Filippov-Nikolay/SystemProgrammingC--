#include <Windows.h>
#include "resource.h"
#include <string>
#include "tchar.h"
#include <time.h>
#include <algorithm>
#include <vector>
#include <CommCtrl.h> // UDM_SETBUDDY

// style
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


const int numberButtons = 16;
int countMove = 0;
int timer = 0;
int tempTimer = 0;
int countTimer = 0;
int remainderTimer = 0;
bool oneStep = true;
// idButtons
int buttonsId[numberButtons] = { IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4, 
							   IDC_BUTTON5, IDC_BUTTON6, IDC_BUTTON7, IDC_BUTTON8, 
							   IDC_BUTTON9, IDC_BUTTON10, IDC_BUTTON11, IDC_BUTTON12,
							   IDC_BUTTON13, IDC_BUTTON14, IDC_BUTTON15, IDC_BUTTON16
							 };
std::vector<int> numbers = {};

HWND hSpin1, hEdit1, hList1, hProgress1;
HWND dlgButtons[numberButtons] = {};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CLOSE: {
			EndDialog(hwnd, 0);

			return TRUE;
		}
		case WM_INITDIALOG: {
			// Устанавливаем заголовок
			SetWindowText(hwnd, _TEXT("GAME"));

			// Инициализируем кнопки
			for (int i = 0; i < numberButtons; i++) {
				dlgButtons[i] = GetDlgItem(hwnd, buttonsId[i]);
				SendMessage(dlgButtons[i], WM_SETTEXT, 0, LPARAM(_TEXT("")));

				// Отключаем кнопки
				EnableWindow(dlgButtons[i], FALSE);
			}

			hSpin1 = GetDlgItem(hwnd, IDC_SPIN1);
			hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
			hList1 = GetDlgItem(hwnd, IDC_LIST1);
			hProgress1 = GetDlgItem(hwnd, IDC_PROGRESS1);

			SendMessage(hSpin1, UDM_SETRANGE32, 0, 100); // Установим диапазон счетчика 

			SendMessage(hProgress1, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); // Установка интервала для индикатора 
			SendMessage(hProgress1, PBM_SETPOS, 0, 0); // Установка текущей позиции индикатора

			// Установливаем приятеля
			SendMessage(hSpin1, UDM_SETBUDDY, WPARAM(hEdit1), 0);

			return TRUE;
		}
		case WM_COMMAND: {
			if (LOWORD(wParam) == IDC_NEWGAME && HIWORD(wParam) == BN_CLICKED) {
				_TCHAR buff[250] = _TEXT("");

				// Получаем значение таймера
				SendMessage(hEdit1, WM_GETTEXT, sizeof(buff) / sizeof(TCHAR), LPARAM(buff));

				// Преобразование buff в int
				countTimer = timer = _ttoi(buff);

				if (countTimer == 0) {
					MessageBox(hwnd, _TEXT("Enter timer value!"), _TEXT("Message"), MB_ICONINFORMATION);

					return TRUE;
				}

				int randNumber = 0;
				int startOfRange = 0;
				int endOfRange = 100;

				countMove = 0;

				srand(time(NULL));

				// Очистка вектора
				numbers.clear();

				for (int i = 0; i < numberButtons; i++) {
					// Включаем кнопки
					EnableWindow(dlgButtons[i], TRUE);

					// Очищаем кнопки
					SendMessage(dlgButtons[i], WM_SETTEXT, 0, LPARAM(_TEXT("")));

					numbers.push_back(rand() % (endOfRange - startOfRange) + startOfRange);
					wsprintf(buff, _TEXT("%d"), numbers[i]);

					// SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));
					SendMessage(dlgButtons[i], WM_SETTEXT, 0, LPARAM(buff));
				}

				// Сортировка вектора по возрастанию
				std::sort(numbers.begin(), numbers.end());

				// Очищаем список
				SendMessage(hList1, LB_RESETCONTENT, 0, 0);

				// Вывод массива в List
				/*for (int i = 0; i < numberButtons; i++) {
					wsprintf(buff, _TEXT("%d"), numbers[i]);
					SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));
				}*/
				
				// Запускаем таймер
				SetTimer(hwnd, 1, 1000, NULL);

				// Установка индикатора на 0
				SendMessage(hProgress1, PBM_SETPOS, 0, 0);

				// Установка значения таймера в заголовок окна
				wsprintf(buff, _TEXT("%d"), countTimer);
				SetWindowText(hwnd, buff);

				oneStep = true;
			}
			else if (LOWORD(wParam) >= IDC_BUTTON1 && LOWORD(wParam) <= IDC_BUTTON16 && HIWORD(wParam) == BN_CLICKED) {
				// ID нажатой кнопки
				int buttonID = LOWORD(wParam);

				// Дескрпитор кнопки
				HWND hButton = GetDlgItem(hwnd, buttonID);

				// Помещаем значение кнопки в buff
				_TCHAR buff[250] = _TEXT("");
				GetWindowText(hButton, buff, sizeof(buff) / sizeof(_TCHAR));

				// Преобразование buff в int
				int value = _ttoi(buff);

				//wsprintf(buff, _TEXT("%d. %d"), value, numbers[countMove]);
				//SetWindowText(hwnd, buff);

				// Сравниваем значение нажатой кнопки и элемента массива
				if (value == numbers[countMove]) {
					countMove++;

					EnableWindow(hButton, FALSE);

					SendMessage(hList1, LB_ADDSTRING, 0, LPARAM(buff));
				}

				// Проверка на завершение игры: все ли кнопки были правильно нажаты
				if (countMove == numberButtons) {
					// Останавливаем таймер
					KillTimer(hwnd, 1);

					MessageBox(hwnd, _TEXT("You WIN!"), _TEXT("Game"), MB_ICONINFORMATION);
				}
			}
			return TRUE;
		}
		case WM_TIMER: {
			countTimer--;

			if (countTimer + 1 == 0) {
				// Останавливаем таймер
				KillTimer(hwnd, 1);

				MessageBox(hwnd, _TEXT("You lose!("), _TEXT("Game"), MB_ICONINFORMATION);

				// Отключаем кнопки
				for (int i = 0; i < numberButtons; i++) {
					EnableWindow(dlgButtons[i], FALSE);
				}

				return TRUE;
			}

			_TCHAR buff[250] = _TEXT("");

			wsprintf(buff, _TEXT("%d"), countTimer);
			SetWindowText(hwnd, buff);

			if (oneStep) {
				oneStep = false;

				// Вычисляем остаток
				int remainder = 100 % timer;

				// Добавляем остаток к индикатору выполнения
				SendMessage(hProgress1, PBM_SETSTEP, remainder, 0);
				SendMessage(hProgress1, PBM_STEPIT, 0, 0);

				// Устанавливаем шаг инкремента равным timer
				SendMessage(hProgress1, PBM_SETSTEP, 100 / timer, 0);
			}

			// Изменяем текущую позицию индикатора путём прибавления шага
			SendMessage(hProgress1, PBM_STEPIT, 0, 0);

			return TRUE;
		}
	}

	return FALSE;
}