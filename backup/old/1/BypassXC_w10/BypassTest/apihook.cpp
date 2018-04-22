#include<Windows.h>
#include"apihook.h"

apihook::apihook(char *dll){
	hModule = GetModuleHandleA(dll);
	if(!hModule){
		Error("dll name");
	}
}

void apihook::Error(char *message){
	MessageBoxA(NULL, message, "error:apihook", NULL);
	TerminateProcess(GetCurrentProcess(), 0x1337);
}

#define mov_edi_edi 0xFF8B
#define push_byte 0x6A
#define mov_eax_dword 0xB8
#define push_dword 0x68
#define push_ebp 0x55
#define mov_ebp_esp 0xEC8B
#define mov_eax_fs 0xA164

void apihook::Hook(char *api, void *api_hook, void *_api){
	DWORD Address = (DWORD)GetProcAddress(hModule, api);
	DWORD Memory = NULL;
	DWORD Size = 0;
	bool bhp = false;

	if(!Address){
		Error("api name");
	}

	FullAccess(Address - 0x05, 0x0C);

	switch(*(BYTE *)Address){
	case push_dword://5 bytes opcode
	case mov_eax_dword://5 bytes opcode (ntdll)
		Size = 5;
		break;
	case push_byte://2 bytes opcode
		Size = 2;
		bhp = HotPatchCheck(Address);
		break;
	default:
		{
			switch(*(WORD *)Address){
			case mov_eax_fs://7 bytes opcode
				Size = 7;
				break;
			case mov_edi_edi://2 bytes ocpde
				Size = 2;
				bhp = HotPatchCheck(Address);
				if(bhp){
					break;
				}
				if(*(BYTE *)(Address + 2) == push_ebp && *(WORD *)(Address + 3) == mov_ebp_esp){
					Size += 3;
				}
				break;
			default:
				break;
			}
		}
		break;
	}
	
	if(Size >= 5){
		Memory = createapi(Address, Size);
		jmp(Address, (DWORD)api_hook);
	}
	else if(Size == 2 && bhp == true){
		Memory = createapi(Address, Size);
		jmp(Address - 5, (DWORD)api_hook);
		backjmp(Address);
	}
	else{
		Error("hook");
	}

	*(DWORD *)_api = Memory;
}

DWORD apihook::createapi(DWORD Address, DWORD Size){
	DWORD Memory = (DWORD)malloc(Size + 5);
	FullAccess(Memory, Size + 5);
	RtlCopyMemory((LPVOID)Memory, (LPVOID)Address, Size);
	jmp(Memory + Size, Address + Size);
	return Memory;
}

void apihook::FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}


bool apihook::HotPatchCheck(DWORD Address){
	int i;
	for(i=1; i<=5; i++){
		if(*(BYTE *)(Address - i) != 0x90 && *(BYTE *)(Address - i) != 0xCC){
			return false;
		}
	}
	return true;
}

#define BackJMP 0xF9EB

void apihook::backjmp(DWORD Address){
	*(WORD *)Address = BackJMP;
}

void apihook::jmp(DWORD Prev, DWORD Next){
	*(BYTE *)Prev = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}