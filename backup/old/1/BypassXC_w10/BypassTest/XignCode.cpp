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


void _declspec(naked) vm_ret(){
	_asm{
		nop
		nop
		nop
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
		cmp eax,0x26
		je vm_NtOpenProcess
		cmp eax,0x71013
		je vm_GetWindowThreadProcessId
		cmp eax,0x107D
		je vm_GetClassNameW
		jmp vm_ret
vm_NtOpenProcess:
		lea edx,[esp+4]
		mov edx,[edx]
		mov edx,[edx]//TargetProcessId
		cmp dword ptr [dwPID],edx
		je vm_ret
		mov eax,0xC0000022
vm_return:
		ret
vm_GetWindowThreadProcessId:
		lea edx,[esp+4]
		cmp dword ptr [edx+0x04],0//getprocessid
		jne vm_ret
		call vm_ret
		cmp dword ptr [dwPID],eax
		je vm_return
		xor eax,eax
		jmp vm_return
vm_GetClassNameW:
		lea edx,[esp+4]
		cmp dword ptr [edx+0x04],0//getclassnamew
		jne vm_ret
		mov eax,0x71013
		call vm_ret
		cmp dword ptr [dwPID],eax
		mov eax,0x00
		jne vm_return
		mov eax,0x107D
		jmp vm_ret
	}
}

#include<stdio.h>
void syscall(){
	DWORD BaseAddress = __readfsdword(0xC0) - 0x68C;//Windows 10 wow64cpu.dll + 0x17D4 = enter syscall
	
	FullAccess(BaseAddress, 0x05);//jmp
	FullAccess(BaseAddress + 0x7A, 0x05);//short jmp
	FullAccess((DWORD)vm_ret, 0x0A);
	
	//return to original code
	RtlCopyMemory((LPVOID)vm_ret, (LPVOID)(BaseAddress + 0x7A), 0x05);
	jmp((DWORD)vm_ret + 0x05, (BaseAddress + 0x7A) + 0x05);
	jmp(BaseAddress, (DWORD)vm);//go to hook code
	*(WORD *)(BaseAddress + 0x7A) = 0x84EB;//back jmp


	/*
	FullAccess(Address - 0x05, 0x2B);
	FullAccess((DWORD)vm_ret, 0x07);
	RtlCopyMemory((LPVOID)vm_ret, (LPVOID)(Address + 0x18), 0x07);
	jmp(Address - 5, (DWORD)vm);
	shortjmp(Address + 0x18);

	FullAccess(Address2 - 0x05, 0x2B);
	jmp(Address2 - 5, (DWORD)vm);
	shortjmp(Address2 + 0x18);
	*/
}


void XignCode(){
	syscall();
	apihook sechost("sechost.dll");
	sechost.Hook("StartServiceW", StartServiceW_Hook, &_StartServiceW);

	apihook kernelbase("kernelbase.dll");

	apihook ntdll("ntdll.dll");

	apihook user32("user32.dll");

	apihook kernel32("kernel32.dll");

	LoadLibraryA("x3.xem");
}