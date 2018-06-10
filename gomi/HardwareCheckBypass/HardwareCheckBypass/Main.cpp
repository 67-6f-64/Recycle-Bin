#include<stdio.h>
#include<Windows.h>
#include<TlHelp32.h>
#pragma comment(lib, "kernel32.lib")

//éGÇ∑Ç¨...?
bool test(DWORD dwPID, ULONGLONG ulBase, ULONGLONG ulSize){
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwPID);
	if(!hProcess){
		return false;
	}
	printf("go to patch\n");

	BYTE Buffer[5];
	SIZE_T ulReturn;

	ReadProcessMemory(hProcess, (void *)(ulBase + 0x2F310), Buffer, 6, &ulReturn);
	if(*(WORD *)&Buffer[0] == 0xF3FF){//push ebx
		if(*(DWORD *)&Buffer[2] == 0x40EC8348){//sub rsp,40
			DWORD old;
			SIZE_T st;
			VirtualProtectEx(hProcess, (void *)(ulBase + 0x2F310), 6, PAGE_EXECUTE_READWRITE, &old);
			BYTE ReturnTrue[6] = {0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3};
			WriteProcessMemory(hProcess, (void *)(ulBase + 0x2F310), ReturnTrue, 6, &st);
			VirtualProtectEx(hProcess, (void *)(ulBase + 0x2F310), 6, old, &old);
			CloseHandle(hProcess);
			return true;
		}
	}

	CloseHandle(hProcess);
	return false;
}

bool ModuleCheck(DWORD dwPID){
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if(hSnapshot == INVALID_HANDLE_VALUE){
		return false;
	}

	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);

	if(Module32First(hSnapshot, &me) == FALSE){
		CloseHandle(hSnapshot);
		return false;
	}

	do{
		if(!_strnicmp(me.szModule, "wuaueng.dll", sizeof("wuaueng.dll"))){
			CloseHandle(hSnapshot);
			printf("go to test\n");
			return test(dwPID, (ULONGLONG)me.modBaseAddr, me.modBaseSize);
		}
	}while(Module32Next(hSnapshot, &me));

	CloseHandle(hSnapshot);
	return false;
}

bool ProcessCheck(){
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if(hSnapshot == INVALID_HANDLE_VALUE){
		return false;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	if(Process32First(hSnapshot, &pe) == FALSE){
		CloseHandle(hSnapshot);
		return false;
	}

	do{
		if(!_strnicmp(pe.szExeFile, "svchost.exe", sizeof("svchost.exe"))){
			if(ModuleCheck(pe.th32ProcessID)){	
				printf("go to memory check\n");
				CloseHandle(hSnapshot);
				return true;
			}
		}
	}while(Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
	return false;
}

bool Debug(){
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValueA(NULL, SE_DEBUG_NAME, &luid)){
		return false;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	HANDLE hToken;
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)){
		return false;
	}

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)){
		return false;
	}

	CloseHandle(hToken);
	return true;
}

int main(){

	if(!Debug()){
		MessageBoxA(NULL, "Debug", NULL, NULL);
		return 1;
	}

	if(ProcessCheck()){
		MessageBoxA(NULL, "ê¨å˜", NULL, NULL);
	}
	else{
		MessageBoxA(NULL, "é∏îs", NULL, NULL);
	}

	return 0;
}