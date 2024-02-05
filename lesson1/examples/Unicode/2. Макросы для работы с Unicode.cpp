// определение символической константы _UNICODE
#define _CRT_SECURE_NO_WARNINGS
//#define _UNICODE
#include <iostream>
#include <tchar.h>// для макросов windows
using namespace std;

void main()
{
	_TCHAR szBuf3[15] = _TEXT("Hello,");
	_tcscat(szBuf3, _TEXT(" world!"));
	cout << (char)*(szBuf3+5) << '\n';
	wcout << szBuf3 << '\n';
	cout << "The size of array: " << sizeof(szBuf3) << " bytes\n"; // 30 байт

	unsigned short s = 34;
	cout << s << endl; // Ansi
	wcout << szBuf3 << endl; // Ansi Unicode


	system("pause");
}