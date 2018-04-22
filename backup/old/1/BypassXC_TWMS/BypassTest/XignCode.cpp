#include<Windows.h>
#include"XignCode.h"
#include"apihook.h"
#include"memory.h"
#include <intrin.h>
#pragma  intrinsic(_ReturnAddress)

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

void XignCode(){
	apihook sechost("sechost.dll");
	sechost.Hook("StartServiceW", StartServiceW_Hook, &_StartServiceW);

	syscall();

	//LoadLibraryA("XignCode/x3.xem");
}