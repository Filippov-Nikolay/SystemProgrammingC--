#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

void main()
{
	wchar_t buffer[] = L"wcstombs converts Unicode-string to ANSI-string";
	// Определим размер памяти, необходимый для хранения преобразованной ANSI-строки
	int length = wcstombs(NULL, buffer, 0);
	char *ptr = new char[length + 1]; 
	// конвертируем Unicode-строку в ANSI-строку 
	wcstombs(ptr, buffer, length + 1);
	cout << ptr;
	cout << "\nLength of ANSI-string: " << strlen(ptr) << endl;
	cout << "Size of allocated memory: " << _msize(ptr) << " bytes" << endl;
	delete[] ptr;
}
