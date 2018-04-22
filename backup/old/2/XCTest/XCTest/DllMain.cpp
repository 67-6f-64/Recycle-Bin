#include<Windows.h>
#include"ExportHook.h"
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include<stdio.h>

BOOL (WINAPI *_CreateProcessInternalW)(DWORD Always01, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, DWORD Always02);
BOOL WINAPI CreateProcessInternalW_Hook(DWORD Always01, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, DWORD Always02){
	BOOL bResult;
	HANDLE hThread;
	if(lpCommandLine != NULL && wcsstr(lpCommandLine, L"xxd-0.xem")){
		
		bResult = _CreateProcessInternalW(Always01, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, Always02);
		hThread = OpenThread(THREAD_ALL_ACCESS, NULL, lpProcessInformation->dwThreadId);
		//InjectThisDLL(lpProcessInformation->dwProcessId);
		//ResumeThread(hThread);
		CloseHandle(hThread);
		return bResult;
	}
	return _CreateProcessInternalW(Always01, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, Always02);
}

DWORD x3_start, x3_end;

FARPROC (WINAPI *_GetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
FARPROC WINAPI GetProcAddress_Hook(HMODULE hModule, LPCSTR lpProcName){
	if(x3_start <= (DWORD)_ReturnAddress() && x3_end >= (DWORD)_ReturnAddress()){
		FILE *fp;
		char txtname[256], filename[256];
		GetModuleFileNameA(hModule, filename, 256);
		sprintf(txtname, "ranaruta_%04X.txt", GetCurrentProcessId());
		fp = fopen(txtname, "a");
		fprintf(fp, "x3.xem+%X:%s(%s)\n", (DWORD)_ReturnAddress() - x3_start, lpProcName, filename);
		fclose(fp);
	}
	return _GetProcAddress(hModule, lpProcName);
}

void x3load(){
	HMODULE hModule = LoadLibraryA("XignCode/x3.xem");
	MODULEINFO ModInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &ModInfo, sizeof(MODULEINFO));
	x3_start = (DWORD)ModInfo.lpBaseOfDll;
	x3_end = (DWORD)ModInfo.lpBaseOfDll + (DWORD)ModInfo.SizeOfImage;
}

void Hook(){
	x3load();
	ExportHook kernel32, kernelbase;
	kernel32.Init("kernel32.dll");
	kernelbase.Init("kernelbase.dll");
	kernel32.Hook("CreateProcessInternalW", CreateProcessInternalW_Hook, &_CreateProcessInternalW);
	kernelbase.Hook("GetProcAddress", GetProcAddress_Hook, &_GetProcAddress);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}
