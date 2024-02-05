//#define _UNICODE

#include <iostream>
#include <tchar.h>
#include <windows.h>

using namespace std;

int NumberOfVowels(_TCHAR* text);
_TCHAR* DeleteByIndex(_TCHAR* text, int index);
_TCHAR* DeleteBySymbol(_TCHAR* text, char& symbol);
_TCHAR* AddSymbol(_TCHAR* text, char& symbol, int index = 0);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    _TCHAR characterString[] = _TEXT("Lorem 1253 Ipsum, is simply dummy, text of the printing and 535 typesetting, industry");
    _TCHAR* tempString = new _TCHAR[255];

    // 1
    for (int i = 0; characterString[i] != '\0'; i++)
        characterString[i] == ' ' ? tempString[i] = '\t' : tempString[i] = characterString[i];

    tempString[_tcslen(characterString)] = '\0';

    wcout << tempString << endl;


    // 2
    int numberOfLetters = 0;
    int numberOfDigits = 0;
    int otherCharacters = 0;

    for (int i = 0; characterString[i] != '\0'; i++)
        isalpha(characterString[i]) ? numberOfLetters++ : isdigit(characterString[i]) ? numberOfDigits++ : otherCharacters++;

    cout << "Number Of Letters: " << numberOfLetters << ";\n" 
        << "Number Of Digits: " << numberOfDigits << ";\n" 
        << "Other Characters: " << otherCharacters << ";\n";
    

    // 3
    int wordCount = 0;
    numberOfDigits = 0;

    for (int i = 0; characterString[i] != '\0'; i++) {
        if (isalpha(characterString[i]))
            numberOfDigits++;

        if ((characterString[i] == ' ' || characterString[i + 1] == '\0') && numberOfDigits >= 2) {
            wordCount++;
            numberOfDigits = 0;
        }
    }
    
    cout << "\nWord count: " << wordCount << ";\n";


    // 4
    const int sizeBuffer = 250;
    _TCHAR Buffer[sizeBuffer] = _TEXT("");

    cout << "Enter: ";
    _getts_s(Buffer, sizeBuffer);

    cout << "Number Of Vowels: " << NumberOfVowels(Buffer) << ";\n";


    // 5
    _TCHAR word[] = _TEXT("tenet");
    tempString = new _TCHAR[255];
    int size = 0;

    for (int i = 0; i < word[i] != '\0'; i++) { size = i; }

    for (int i = 0; i <= size; i++) {
        tempString[i] = word[size - i];
        if (i == size)
            tempString[i + 1] = '\0';
    }

    if (_tcscmp(tempString, word) == 0)
        cout << "The word palindrome\n";
    else
        cout << "This word is not a palindrome\n";


    // 6
    int index = 0;
    
    cout << "Enter index: ";
    cin >> index;

    wcout << DeleteByIndex(characterString, index) << endl;


    // 7
    char symbol = '0';

    cout << "Enter symbol: ";
    cin >> symbol;

    wcout << DeleteBySymbol(characterString, symbol) << endl;


    // 8
    cout << "Enter symbol: ";
    cin >> symbol;

    cout << "Enter index: ";
    cin >> index;

    wcout << AddSymbol(characterString, symbol, index) << endl;


    delete[] tempString;

    return 0;
}

int NumberOfVowels(_TCHAR* text) {
    if (text == nullptr)
        return -1;

    int NumberOfVowels = 0;   

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == 'а' || text[i] == 'у' || text[i] == 'о' || text[i] == 'и' || text[i] == 'э' || 
            text[i] == 'ы' || text[i] == 'я' || text[i] == 'ю' || text[i] == 'е' || text[i] == 'ё') {
            NumberOfVowels++;
        }
    }

    return NumberOfVowels;
}

_TCHAR* DeleteBySymbol(_TCHAR* text, char& symbol) {
    if (text == nullptr)
        return (_TCHAR*)_TEXT("Ptr is null!\n");

    _TCHAR* tempString = new _TCHAR[_tcslen(text) + 2];

    int j = 0;

    for (int i = 0; text[i] != '\0'; i++)
        tolower(text[i]) != symbol ? tempString[j++] = text[i] : NULL;

    tempString[j] = '\0';

    return tempString;
}

_TCHAR* AddSymbol(_TCHAR* text, char& symbol, int index) {
    if (text == nullptr || index < 0 || index >= _tcslen(text))
        return (_TCHAR*)_TEXT("Invalid index or null ptr!\n");

    _TCHAR* tempString = new _TCHAR[_tcslen(text) + 1];
 
    for (int i = 0; text[i] != '\0'; i++)
        index == i ? tempString[i] = symbol : tempString[i] = text[i];

    tempString[_tcslen(text) - 1] = '\0';

    return tempString;
}

_TCHAR* DeleteByIndex(_TCHAR* text, int index) {
    if (text == nullptr || index < 0 || index >= _tcslen(text))
        return (_TCHAR*)_TEXT("Invalid index or null ptr!\n");

    _TCHAR* tempString = new _TCHAR[_tcslen(text) + 1];
    
    for (int i = 0, j = 0; text[i] != '\0'; i++)
        i != index ? tempString[j++] = text[i] : NULL;

    tempString[_tcslen(text) - 1] = '\0';

    return tempString;
}