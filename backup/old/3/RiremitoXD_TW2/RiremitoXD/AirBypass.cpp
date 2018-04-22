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

NTSTATUS (WINAPI *_NtQueryInformationProcess)(HANDLE ProcessHandle, PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);


HWND (WINAPI *_CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND WINAPI CreateWindowExAHook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam){
	//MessageBoxA(NULL, lpClassName, NULL, NULL);
	if(lpClassName && strstr(lpClassName, "StartUpDlgClass")){
		Bypass();
		MultiMS();
	}
	return _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((void *)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}
/*
void WriteJMP(DWORD Prev, DWORD Next){
	FullAccess(Prev, 5);
	*(BYTE *)Prev = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}
*/

void _stdcall TestLog(DWORD Return){
	FILE *fp;
	fp = fopen("Rasuun.txt", "a");
	fprintf(fp, "%p:\n",Return);
	fclose(fp);
}

DWORD _stdcall CheckDLL(DWORD Address){
	if(Address > (DWORD)GetModuleHandleA("ntdll.dll")){
		return 1;
	}
	return 0;
}

DWORD SystemCall;
void _declspec(naked) SystemCallHook(){
	_asm{
		cmp eax,0xCA
		je HWBP
		cmp eax,0x150
		je HWBP
		//cmp eax,0x16
		//je ModuleScan
Return:
		jmp dword ptr [SystemCall]
		/*
ModuleScan:
		cmp dword ptr [edx+0x04],0
		jne Return
		jmp Return
		*/
HWBP:
		push eax
		mov eax,[esp]
		pushad
		push eax
		call CheckDLL
		test eax,eax
		popad
		pop eax
		jne Return
		add dword ptr [esp],0x03
		xor eax,eax
		ret
	}
}

void NtHook(){
	DWORD Address = __readfsdword(0xC0);
	if(*(BYTE *)Address == 0xEA){
		FullAccess(Address, 12);
		WriteJMP(Address + 7, (DWORD)SystemCallHook);
		SystemCall = (DWORD)malloc(7);
		FullAccess(SystemCall, 7);
		*(BYTE *)SystemCall = 0xEA;
		*(DWORD *)(SystemCall + 1) = *(DWORD *)(Address + 1);
		*(WORD *)(SystemCall + 5) = 0x0033;
		*(WORD *)Address = 0x05EB;
	}
}

void AirBypass(){
	ExportHook KernelBase, Kernel32, User32;
	//HSCRC();
	//KernelBase.Init("KernelBase.dll");
	//KernelBase.Hook("DeviceIoControl", DeviceIoControlHook, &_DeviceIoControl);
	Kernel32.Init("Kernel32.dll");
	//Kernel32.Hook("K32GetMappedFileNameA", K32GetMappedFileNameAHook, &_K32GetMappedFileNameA);
	//Kernel32.Hook("K32EnumProcesses", K32EnumProcessesHook, &_K32EnumProcesses);
	//Kernel32.Hook("K32EnumProcessModules", K32EnumProcessModulesHook, &_K32EnumProcessModules);
	Kernel32.Hook("CreateProcessInternalW", CreateProcessInternalWHook, &_CreateProcessInternalW);
	//Kernel32.Hook("CreateToolhelp32Snapshot", CreateToolhelp32SnapshotHook, &_CreateToolhelp32Snapshot);
	//NtHook();
	
	User32.Init("User32.dll");
	User32.Hook("CreateWindowExA", CreateWindowExAHook, &_CreateWindowExA);
}