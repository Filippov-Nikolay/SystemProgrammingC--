#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
    STARTUPINFO s = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION p;

    TCHAR buffer[] = TEXT("chrome.exe");

    BOOL fs = CreateProcess(NULL, buffer, NULL, NULL, FALSE, 0, NULL, NULL, &s, &p);

    if (fs) {
        CloseHandle(p.hThread);
        CloseHandle(p.hProcess);
    }

    int v = 0;

    cin >> v;

    if (v == 0) {
        ExitProcess(0);
    } else if (v == 1) {
        TerminateProcess(p.hProcess, 0);
    }

    return 0;
}
