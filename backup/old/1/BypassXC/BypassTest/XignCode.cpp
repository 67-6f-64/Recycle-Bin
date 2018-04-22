#include<Windows.h>
#include"XignCode.h"
#include"apihook.h"
#include"memory.h"
#include <intrin.h>
#pragma  intrinsic(_ReturnAddress)


DWORD _stdcall IsXignCode(DWORD retAddr){
	HMODULE phModule;
	HMODULE hX3 = GetModuleHandleA("x3.xem");

	if(!hX3){
		return 0;
	}

	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPSTR)retAddr, &phModule);

	if(phModule == hX3){
		return 1;
	}
	if(phModule == NULL){
		return 2;
	}
	return 0;
}

//Memory Protection
BOOL (WINAPI *_StartServiceW)(SC_HANDLE hService, DWORD dwNumServiceArgs, LPCWSTR *lpServiceArgVectors);
BOOL WINAPI StartServiceW_Hook(SC_HANDLE hService, DWORD dwNumServiceArgs, LPCWSTR *lpServiceArgVectors){
	if(hService != NULL){
		return TRUE;
	}
	return _StartServiceW(hService, dwNumServiceArgs, lpServiceArgVectors);
}

/*
	push ptr_out
	push ptr_out
	push dwDesiredAccess
	push &dwProcessId
	call dword ptr [75DC11D4] { ->ntdll.NtOpenProcess }
*/
//Anti Attach1!
NTSTATUS (WINAPI *_NtOpenProcess)(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, LPVOID ObjectAttributes, LPVOID ClientId);
NTSTATUS WINAPI NtOpenProcess_Hook(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, LPVOID ObjectAttributes, LPVOID ClientId){

	//DWORD dwPID = (DWORD)*ProcessHandle;
	//if(dwPID == 0x06E4){
	//	return 0xC000000B;
	//}
	//if(IsXignCode((DWORD)_ReturnAddress())){
	//	SetLastError(0xC000000B);
	//	return 0xC000000B;
	//}

	//if((DWORD)*ProcessHandle == 0x9A0){
		//return 0xC0000022;//STATUS_ACCESS_DENIED

	//}

	//return _NtOpenProcess(ProcessHandle, DesiredAccess/*GENERIC_READ*/, ObjectAttributes, ClientId);
}

//Debugger Detect
NTSTATUS (WINAPI *_NtQueryInformationProcess)(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
NTSTATUS WINAPI NtQueryInformationProcess_Hook(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength){
	//if( == 0x9A0){
	//	MessageBoxA(NULL, "nqip", NULL, NULL);

	//}
	return _NtQueryInformationProcess(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
}


//Mimize Window
DWORD (WINAPI *_GetWindowThreadProcessId)(HWND hWnd, LPDWORD lpdwProcessId);
DWORD WINAPI GetWindowThreadProcessId_Hook(HWND hWnd, LPDWORD lpdwProcessId){
	DWORD dwPID;
	DWORD dwReturn = _GetWindowThreadProcessId(hWnd, &dwPID);
	DWORD dwCPID = GetCurrentProcessId();
	if(dwPID != dwCPID){
		return _GetWindowThreadProcessId(0, lpdwProcessId);
	}
	if(lpdwProcessId){
		*lpdwProcessId = dwPID;
	}
	return dwReturn;
}

//Minimize Window!
int (WINAPI *_GetClassNameW)(HWND hWnd, LPWSTR lpClassName, int nMaxCount);
int WINAPI GetClassNameW_Hook(HWND hWnd, LPWSTR lpClassName, int nMaxCount){
	//if(IsXignCode((DWORD)_ReturnAddress())){
		//HWND hCE = FindWindowA(NULL, "Cheat Engine 6.5.1");
		//if(hCE){
			DWORD dwPID1, dwPID2;
			dwPID1 = GetCurrentProcessId();
			GetWindowThreadProcessId(hWnd, &dwPID2);
			if(dwPID1 != dwPID2){
				return 0;
			}
		//}
	//}
	return _GetClassNameW(hWnd, lpClassName, nMaxCount);
}


//xxxxxxxxxxxx
//Minimize Window
BOOL (WINAPI *_PostMessageW)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI PostMessageW_Hook(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	if(IsXignCode((DWORD)_ReturnAddress()) && Msg == WM_SYSCOMMAND){
		return 1;
	}
	return _PostMessageW(hWnd, Msg, wParam, lParam);
}


HANDLE (WINAPI *_CreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE WINAPI CreateFileW_Hook(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile){
	if(IsXignCode((DWORD)_ReturnAddress() && lpFileName)/* && IsXignCode((DWORD)_ReturnAddress())*/){
		if(wcsstr(lpFileName, L"Prefetch") || wcsstr(lpFileName, L".exe") || wcsstr(lpFileName, L".dll")  || wcsstr(lpFileName, L".log")){
			//MessageBoxW(NULL, lpFileName, L"CreateFileW", NULL);
			return INVALID_HANDLE_VALUE;
		}
	}
	return _CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

void FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}


void shortjmp(DWORD Prev){
	*(WORD *)Prev = 0x05EB;
}

void jmp(DWORD Prev, DWORD Next){
	*(BYTE *)Prev = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}

void _declspec(naked) _stdcall msg(){
	MessageBoxA(NULL, "EnumWindows!", NULL ,NULL);
}

void _declspec(naked) vm_ret(){
	_asm{
			nop
			nop
			nop
			nop
			nop
			nop
			nop
	}
}
DWORD dwPID = GetCurrentProcessId();
void _declspec(naked) vm(){
	_asm{
		cmp eax,0x23
		je vm_NtOpenProcess
		
		cmp eax,0x1010
		je vm_GetWindowThreadProcessId
		cmp eax,0x107B
		je vm_GetClassNameW
		
		//cmp eax,0x101C
		//je vm_EnumWindows//query
		jmp vm_ret
vm_NtOpenProcess:
		push edx
		mov edx,[edx]
		mov edx,[edx]//TargetProcessId
		cmp dword ptr [dwPID],edx
		pop edx
		je vm_ret
		mov eax,0xC0000022
vm_return:
		add dword ptr [esp],0x03
		ret
vm_GetWindowThreadProcessId:
		cmp dword ptr [edx+0x04],0//getprocessid
		jne vm_ret
		push edx
		call vm_ret
		add esp,0x04//eax = processId
		pop edx
		cmp dword ptr [dwPID],eax
		je vm_return
		xor eax,eax
		jmp vm_return
vm_GetClassNameW:
		cmp dword ptr [edx+0x04],0//getclassnamew
		jne vm_ret
		push edx
		mov eax,0x1010
		mov ecx,0x07
		call vm_ret
		add esp,0x04//eax = processId
		pop edx
		cmp dword ptr [dwPID],eax
		mov eax,0x00
		jne vm_return
		mov eax,0x107B
		xor ecx,ecx
		jmp vm_ret
vm_EnumWindows:
		push eax
		push ecx
		push edx
		push [edx-0x04]
		call IsXignCode
		test eax,eax
		pop edx
		pop ecx
		pop eax
		je vm_ret
		jmp vm_ret
	}
}

void syscall(){
	DWORD Address = __readfsdword(0xC0);

	FullAccess(Address, 0x0C);
	FullAccess((DWORD)vm_ret, 0x07);
	RtlCopyMemory((LPVOID)vm_ret, (LPVOID)Address, 0x07);
	jmp(Address + 7, (DWORD)vm);
	shortjmp(Address);
}

/////////////////////////
DWORD (WINAPI *_GetModuleFileNameW)(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
DWORD WINAPI GetModuleFileNameW_Hook(HMODULE hModule, LPWSTR lpFilename, DWORD nSize){
	DWORD dwReturn = _GetModuleFileNameW(hModule, lpFilename, nSize);

	//if(IsXignCode((DWORD)_ReturnAddress())/*nSize == 0x104*/){
		if(wcsstr(lpFilename, L"vehdebug-i386.dll")){
			ZeroMemory(lpFilename, nSize);
			return ERROR_SUCCESS;
		}
	//}

	return dwReturn;
}


////////////////////////
#include<stdio.h>
NTSTATUS (WINAPI *_LdrLoadDll)(LPVOID lpv1, LPVOID lpv2, LPVOID lpv3, HMODULE *BaseAddress);
NTSTATUS WINAPI LdrLoadDll_Hook(LPVOID lpv1, LPVOID lpv2, LPVOID lpv3, HMODULE *BaseAddress){
	DWORD Return = _LdrLoadDll(lpv1, lpv2, lpv3, BaseAddress);
	if(GetProcAddress(*BaseAddress, "DbgUserBreakPoint")){//ntdll
		FreeLibrary(*BaseAddress);
		*BaseAddress = GetModuleHandleA("ntdll.dll");
		return Return;
	}
	else if(GetProcAddress(*BaseAddress, "WahCloseThread")){//ws2_32
		FreeLibrary(*BaseAddress);
		*BaseAddress = GetModuleHandleA("ws2_32.dll");
		return Return;
	}

	char FileName[260];
	GetModuleFileNameA((HMODULE)*BaseAddress, FileName, 260);
	FILE *fp = fopen("Ldr.txt", "a");
	fprintf(fp, "%08X:%s\n", *BaseAddress, FileName);
	fclose(fp);
	
	return Return;
}

LPVOID (WINAPI *_VirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize,DWORD flAllocationType, DWORD flProtect);
LPVOID WINAPI VirtualAllocEx_Hook(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize,DWORD flAllocationType, DWORD flProtect){
	LPVOID Return = _VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
	if(dwSize == 0xBFD8E){
		_asm{
			nop
		}
		return Return;
	}
	/*
	FILE *fp = fopen("Memory.txt", "a");
	fprintf(fp, "P:%p,A:%p(S:%p, F:%p)\n", GetProcessId(hProcess), Return, dwSize, flProtect);
	fclose(fp);
	*/
	return Return;
}


DWORD (WINAPI *_NtCreateThreadEx)(LPVOID v1, LPVOID v2, LPVOID v3, LPVOID v4, DWORD Address, LPVOID v6, ULONG CreateFlags, LPVOID v8, LPVOID v9, LPVOID v10, LPVOID v11);
DWORD WINAPI NtCreateThreadEx_Hook(LPVOID v1, LPVOID v2, LPVOID v3, LPVOID v4, DWORD Address, LPVOID v6, ULONG CreateFlags, LPVOID v8, LPVOID v9, LPVOID v10, LPVOID v11){
	//if(IsXignCode(Address)){
		//return _NtCreateThreadEx(v1, v2, v3, v4, (DWORD)ah, v6, CreateFlags/*CREATE_SUSPENDED*/, v8, v9, v10, v11);
	//}
	FILE *fp = fopen("Thread.txt", "a");
	fprintf(fp, "A:%p\n", Address);
	fclose(fp);


	return _NtCreateThreadEx(v1, v2, v3, v4, Address, v6, CreateFlags, v8, v9, v10, v11);
}

/////
//#include<psapi.h>
//#pragma comment(lib, "psapi.lib")
typedef BOOL (WINAPI *DllEntryPoint)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

void ManualLoad(){
	HMODULE XC = LoadLibraryExA("XignCode/x3.xem", NULL, LOAD_LIBRARY_AS_DATAFILE);
	DllEntryPoint DEP = (DllEntryPoint)((DWORD)(XC) + 0x527000);
	MessageBoxA(NULL, "XC LOAD!", NULL, NULL);
	DEP((HINSTANCE)XC, DLL_PROCESS_DETACH, 0);
}

void XignCode(){
	//ManualLoad();
	apihook sechost("sechost.dll");
	sechost.Hook("StartServiceW", StartServiceW_Hook, &_StartServiceW);

	apihook kernelbase("kernelbase.dll");
	//kernelbase.Hook("VirtualAllocEx", VirtualAllocEx_Hook, &_VirtualAllocEx);
	//kernelbase.Hook("GetModuleFileNameW", GetModuleFileNameW_Hook, &_GetModuleFileNameW);

	apihook ntdll("ntdll.dll");
	//ntdll.Hook("LdrLoadDll", LdrLoadDll_Hook, &_LdrLoadDll);

	//ntdll.Hook("NtOpenProcess", NtOpenProcess_Hook, &_NtOpenProcess);
	//ntdll.Hook("NtQueryInformationProcess", NtQueryInformationProcess_Hook, &_NtQueryInformationProcess);
	//ntdll.Hook("NtCreateThreadEx", NtCreateThreadEx_Hook, &_NtCreateThreadEx);

	apihook user32("user32.dll");
	//user32.Hook("PostMessageW", PostMessageW_Hook, &_PostMessageW);//X Minimize Window 
	//user32.Hook("GetWindowThreadProcessId", GetWindowThreadProcessId_Hook, &_GetWindowThreadProcessId);//X
	//user32.Hook("GetClassNameW", GetClassNameW_Hook, &_GetClassNameW);//X

	apihook kernel32("kernel32.dll");
	//kernel32.Hook("CreateFileW", CreateFileW_Hook, &_CreateFileW);//X




	syscall();

	LoadLibraryA("XignCode/x3.xem");
}