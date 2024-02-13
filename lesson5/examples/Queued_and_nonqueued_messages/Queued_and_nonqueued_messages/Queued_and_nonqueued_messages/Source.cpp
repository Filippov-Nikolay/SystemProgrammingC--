#include <windows.h>
#include <tchar.h>


TCHAR szClassWindow[] = TEXT(" аркасное приложение");

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{

    LPCTSTR               lpApplicationName;
    LPTSTR                lpCommandLine;
    LPSECURITY_ATTRIBUTES lpProcessAttributes;
    LPSECURITY_ATTRIBUTES lpThreadAttributes;
    BOOL                  bInheritHandles;
    DWORD                 dwCreationFlags;
    LPVOID                lpEnvironment;
    LPCTSTR               lpCurrentDirectory;
    LPSTARTUPINFO         lpStartupInfo;
    LPPROCESS_INFORMATION lpProcessInformation;

    HWND hWindow; // окно " алькул€тор ѕлюс"
    HWND hWindow1; // окно "ћо€ программа"


    LPPROCESS_INFORMATION pi = { 0 };
    SHELLEXECUTEINFO sei = { 0 };

    SECURITY_ATTRIBUTES sa = { 0 };
    sa.nLength = sizeof(sa);

    STARTUPINFO si = { 0 };
    si.cb = sizeof(si);   // –азмер структуры в байтах.
    si.dwX = 100; // смещением по X верхнего левого угла созданного окна, в пиксел€х
    si.dwY = 100; //  смещением по Y верхнего левого угла созданного окна, в пиксел€х
    si.dwXSize = 400; // шириной окна, в пиксел€х
    si.dwYSize = 200; // высотой окна, в пиксел€х
    si.dwXCountChars = NULL; //
    si.dwYCountChars = NULL; //
    si.dwFillAttribute = FOREGROUND_BLUE; //  определ€ет начальный цвет текста и фона
    si.dwFlags = STARTF_USEPOSITION;
    si.wShowWindow = NULL;
    si.cbReserved2 = NULL;

    lpApplicationName = TEXT("c:\calc.exe");  // указатель на им€ исполн€емого файла
    lpCommandLine = NULL;                 // указатель на командную строку
    lpProcessAttributes = NULL;   // указатель на атрибуты безопасности процесса
    lpThreadAttributes = NULL;     // указатель на атрибуты безопасности потока
    bInheritHandles = TRUE;                // указатель на флаг наследовани€
    dwCreationFlags = CREATE_DEFAULT_ERROR_MODE;           // флаги создани€
    lpEnvironment = NULL;                 // указатель на новый блок среды
    lpCurrentDirectory = NULL;            // указатель на им€ текущего каталога
    lpStartupInfo = &si;             // указатель на структуру STARTUPINFO
    lpProcessInformation = pi;     // указатель на структуру PROCESS_INFORMATION

        CreateProcess(

            lpApplicationName,
            lpCommandLine,
            lpProcessAttributes,
            lpThreadAttributes,
            bInheritHandles,
            dwCreationFlags,
            lpEnvironment,
            lpCurrentDirectory,
            lpStartupInfo,
            lpProcessInformation

        );

    Sleep(30);

    HWND calc = FindWindow(_T("SciCalc"), NULL);
    SetWindowText(calc, _T("ƒолбаный калькул€торплюс"));


    /*CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);*/
}