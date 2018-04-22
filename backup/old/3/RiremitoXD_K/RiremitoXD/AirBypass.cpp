#include"AirBypass.h"
#include"ExportHook.h"
#include<stdio.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include<tlhelp32.h>
#include<Winternl.h>
#include"MultiClient.h"

#define HS_Memory_Protection 0x80000040
BOOL (WINAPI *_DeviceIoControl)(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
BOOL WINAPI DeviceIoControlHook(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped){
	if(dwIoControlCode == HS_Memory_Protection){
		return TRUE;
	}
	return _DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
}

DWORD (WINAPI *_K32GetMappedFileNameA)(HANDLE hProcess, LPVOID lpv, LPTSTR lpFilename, DWORD nSize);
DWORD WINAPI K32GetMappedFileNameAHook(HANDLE hProcess, LPVOID lpv, LPTSTR lpFilename, DWORD nSize){
	DWORD dwResult;
	if(hProcess == (HANDLE)-1){
		dwResult = _K32GetMappedFileNameA(hProcess, lpv, lpFilename, nSize);
		if(dwResult){
			if(strstr(lpFilename, "MapleStory.exe") || strstr(lpFilename, "BlackCipher.aes") || strstr(lpFilename, "EHSvc.dll") || strstr(lpFilename, ".tmp")){
				return dwResult;
			}
		}
	}
	return 0;
}

BOOL (WINAPI *_K32EnumProcesses)(DWORD *pProcessIds, DWORD cb, DWORD *pBytesReturned);
BOOL WINAPI K32EnumProcessesHook(DWORD *pProcessIds, DWORD cb, DWORD *pBytesReturned){
	return 0;
}

BOOL (WINAPI *_K32EnumProcessModules)(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded);
BOOL WINAPI K32EnumProcessModulesHook(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded){
	return 0;
}

/*
	Black Cipher (Nexon Game Security) Bypass
*/
void InjectThisDLL(DWORD ProcessId){
	wchar_t FilePath[1024];
	GetModuleBaseNameW(GetCurrentProcess(), GetModuleHandleA("ijl15.dll"), FilePath, sizeof(FilePath));
	Inject(ProcessId, FilePath);

}

BOOL (WINAPI *_CreateProcessInternalW)(DWORD Always01, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, DWORD Always02);
BOOL WINAPI CreateProcessInternalWHook(DWORD Always01, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, DWORD Always02){
	BOOL bResult;
	HANDLE hThread;
	if(lpCommandLine != NULL && wcsstr(lpCommandLine, L"BlackCipher.aes")){
		bResult = _CreateProcessInternalW(Always01, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, Always02);
		hThread = OpenThread(THREAD_ALL_ACCESS, NULL, lpProcessInformation->dwThreadId);
		InjectThisDLL(lpProcessInformation->dwProcessId);
		ResumeThread(hThread);
		CloseHandle(hThread);
		return bResult;
	}
	return _CreateProcessInternalW(Always01, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, Always02);
}

HANDLE (WINAPI *_CreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);
HANDLE WINAPI CreateToolhelp32SnapshotHook(DWORD dwFlags, DWORD th32ProcessID){
	return (HANDLE)-1;
}

HWND (WINAPI *_CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND WINAPI CreateWindowExAHook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam){
	if(lpClassName && strstr(lpClassName, "StartUpDlgClass")){
		//Bypass();
		//MultiMS();
	}
	return _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((void *)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}

void AirBypass(){
	ExportHook KernelBase, Kernel32, User32;
	Kernel32.Init("Kernel32.dll");
	Kernel32.Hook("CreateProcessInternalW", CreateProcessInternalWHook, &_CreateProcessInternalW);
	User32.Init("User32.dll");
	User32.Hook("CreateWindowExA", CreateWindowExAHook, &_CreateWindowExA);
}