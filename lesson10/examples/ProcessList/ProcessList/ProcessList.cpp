// ProcessList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void ProcessList()
{
	
	HANDLE hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32;
	memset(&pe32, 0, sizeof(PROCESSENTRY32));
	pe32.dwSize=sizeof(PROCESSENTRY32);
	if(Process32First(hSnapShot, &pe32))
	{
		_tprintf(_T("ID:  %4u  Threads: %5u   priority:  %2u    name:   %s\n"), pe32.th32ProcessID, pe32.cntThreads, pe32.pcPriClassBase, pe32.szExeFile);

		while(Process32Next(hSnapShot, &pe32))
		{
			_tprintf(_T("ID:  %4u  Threads: %5u   priority:  %2u    name:   %s\n"), pe32.th32ProcessID, pe32.cntThreads, pe32.pcPriClassBase, pe32.szExeFile);
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	ProcessList();
	_getch();
	return 0;
}

