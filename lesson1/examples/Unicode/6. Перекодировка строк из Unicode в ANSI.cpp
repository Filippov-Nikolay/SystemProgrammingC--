#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
using namespace std;

void main()
{
	wchar_t buffer[] = L"WideCharToMultiByte converts Unicode-string to ANSI-string";
	// определим размер памяти, необходимый для хранения ANSI-строки
	int length = WideCharToMultiByte(CP_ACP /*ANSI code page*/, 0, buffer, -1, NULL, 0, 0, 0);
	char *ptr = new char[length];
	// конвертируем Unicode-строку в ANSI-строку
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, ptr, length, 0, 0); 
	cout << ptr << endl;
	cout << "Length of ANSI-string: " << strlen(ptr) << endl;
	cout << "Size of allocated memory: " << _msize(ptr) << endl;
	delete[] ptr;

	system("pause");
}