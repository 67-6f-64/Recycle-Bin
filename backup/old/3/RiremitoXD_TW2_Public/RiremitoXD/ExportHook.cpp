#include"ExportHook.h"

void ExportHook::Init(char *DllName, bool bLoad){
	if(!bLoad){
		hModuleEH = GetModuleHandleA(DllName);
	}
	else{
		hModuleEH = LoadLibraryA(DllName);
	}
}

#define mov_edi_edi 0xFF8B
#define push_byte 0x6A
#define mov_eax_dword 0xB8
#define push_dword 0x68
#define push_ebp 0x55
#define mov_ebp_esp 0xEC8B
#define mov_eax_fs 0xA164

void ExportHook::Hook(char *FunctionName, void *HookFunction, void *NewFunction){
	DWORD Memory;
	DWORD Address = (DWORD)GetProcAddress(hModuleEH, FunctionName);
	DWORD HotPatch = Address - 5;
	if(*(WORD *)Address == mov_edi_edi || *(BYTE *)Address == push_byte){
		
			if(HotPatchCheck(Address)){
				Memory = (DWORD)malloc(7);
				FullAccess((DWORD)Memory, 7);
				RtlCopyMemory((void *)Memory, (void *)Address, 2);
				WriteJMP(Memory + 2, Address + 2);
				WriteJMP(HotPatch, (DWORD)HookFunction);
				//WriteJMP(HotPatch, (DWORD)Memory);
				WriteBackJMP(Address);
			}
			else{
				if(*(BYTE *)(Address + 2) == push_ebp && *(WORD *)(Address + 3) == mov_ebp_esp){
					Memory = (DWORD)malloc(10);
					FullAccess((DWORD)Memory, 10);
					RtlCopyMemory((void *)Memory, (void *)Address, 5);
					WriteJMP(Memory + 5, Address + 5);
					WriteJMP(Address, (DWORD)HookFunction);
				}
			}
	}
	else if(*(BYTE *)Address == mov_eax_dword || *(BYTE *)Address == push_dword){
		Memory = (DWORD)malloc(10);
		FullAccess((DWORD)Memory, 10);
		RtlCopyMemory((void *)Memory, (void *)Address, 5);
		WriteJMP(Memory + 5, Address + 5);
		WriteJMP(Address, (DWORD)HookFunction);
	}
	else if(*(WORD *)Address == mov_eax_fs){
		Memory = (DWORD)malloc(11);
		FullAccess((DWORD)Memory, 11);
		RtlCopyMemory((void *)Memory, (void *)Address, 6);
		WriteJMP(Memory + 6, Address + 6);
		WriteJMP(Address, (DWORD)HookFunction);
	}
	*(DWORD *)(NewFunction) = Memory;
}

void ExportHook::FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((void *)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}

void ExportHook::WriteJMP(DWORD Prev, DWORD Next){
	FullAccess(Prev, 5);
	*(BYTE *)Prev = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}

bool ExportHook::HotPatchCheck(DWORD Address){
	DWORD i = Address - 5;
	FullAccess(i, 5);
	for(; i<Address; i++){
		if(*(BYTE *)i != 0x90 && *(BYTE *)i != 0xCC){
			return false;
		}
	}
	return true;
}

#define BackJMP 0xF9EB

void ExportHook::WriteBackJMP(DWORD Address){
	FullAccess(Address, 2);
	*(WORD *)(Address) = BackJMP;
}