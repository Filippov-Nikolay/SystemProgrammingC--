#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szClassWindow[] = TEXT("Каркасное приложение");

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow) {
	// 1
	_TCHAR value[] = _TEXT("Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. Lorem Ipsum is simply dummy text of the printing and typesetting industry. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages.");
	_TCHAR buffer[100];
	int wordCount = _tcslen(value) / 3;

	MessageBox(0,
		_TEXT("Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book."),
		_TEXT("Окно сообщения"), MB_OK | MB_ICONINFORMATION);

	MessageBox(0,
		_TEXT("Lorem Ipsum is simply dummy text of the printing and typesetting industry."),
		_TEXT("Окно сообщения"), MB_OK | MB_ICONINFORMATION);

	_stprintf_s(buffer, _TEXT("Кол-во слов: %d"), wordCount);

	MessageBox(0,
		_TEXT("It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages."),
		buffer,
		MB_OK | MB_ICONINFORMATION);


	// 2
	_TCHAR buffGame[200];

	int number = 3;
	int counter = 0;
	int answer = 0;
	bool playAgainFlag = true;
	bool f = false;
	
	int start = MessageBox(0,
		_TEXT("Начать игру?"),
		_TEXT("Игра"), MB_YESNO | MB_ICONQUESTION);

	if (start == IDYES) {
		do {
			wsprintf(buffGame, _TEXT("Это число: %d ?"), counter);

			answer = MessageBox(0,
				buffGame,
				_TEXT("Угадай"),
				MB_YESNO | MB_ICONQUESTION);

			if (counter == number && answer == IDYES) {
				MessageBox(0,
					_TEXT("Вы победили!"),
					_TEXT("Игра"), MB_OK | MB_ICONASTERISK);
				f = true;
			}
			else if (counter != number && answer == IDYES) {
				MessageBox(0,
					_TEXT("Вы Проиграли!"),
					_TEXT("Игра"), MB_OK | MB_ICONEXCLAMATION);
				f = true;
			}

			if (f) {
				int playAgain = MessageBox(0,
					_TEXT("Желаете сыграть ещё?"),
					_TEXT("Игра"),
					MB_YESNO | MB_ICONQUESTION);

				if (playAgain == IDYES) {
					counter = -1;
					f = false;
				}
				else {
					playAgainFlag = false;
				}
			}

			counter++;
		} while (playAgainFlag);
	}
	
	if (!playAgainFlag || start == IDNO) {
		MessageBox(0,
			_TEXT("До свидания!"),
			_TEXT("Игра"), MB_OK | MB_ICONSTOP);
	}

	return 0;
}