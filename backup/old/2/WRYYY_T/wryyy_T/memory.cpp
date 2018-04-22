#include<Windows.h>
#include"memory.h"
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include<stdio.h>

memory::memory(){
	HMODULE hModule = GetModuleHandleA(NULL);
	IMAGE_NT_HEADERS *nth;

	nth = (IMAGE_NT_HEADERS *)((DWORD)hModule + PIMAGE_DOS_HEADER(hModule)->e_lfanew);
	BaseAddress = (DWORD)nth->OptionalHeader.ImageBase;
	ImageSize = (DWORD)nth->OptionalHeader.SizeOfImage;
}

memory::memory(char *dll){
	HMODULE hModule = LoadLibraryA(dll);
	MODULEINFO ModInfo;

	if(!hModule){
		Error("dll");
	}

	GetModuleInformation(GetCurrentProcess(), hModule, &ModInfo, sizeof(MODULEINFO));
	BaseAddress = (DWORD)ModInfo.lpBaseOfDll;
	ImageSize = (DWORD)ModInfo.lpBaseOfDll + (DWORD)ModInfo.SizeOfImage;
}

void memory::Error(char *message){
	MessageBoxA(NULL, message, "error:memory", NULL);
	TerminateProcess(GetCurrentProcess(), 0x1337);
}

void memory::FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
}

void memory::write(DWORD Address, char *code){
	int i, j = 0;
	BYTE b[256], bt;
	ZeroMemory(b, 256);
	DWORD Size;

	for(i=0; code[i]; i++){
		bt = (BYTE)code[i];
		if('0' <= bt && bt <= '9'){
			b[j++/2] += (j%2) ? (bt - '0') : (bt - '0') << 4;
		}
		else if('A' <= bt && bt <= 'F'){
			b[j++/2] += (j%2) ? (bt - 'A' + 0x0A) : (bt - 'A' + 0x0A) << 4;
		}
		else if('a' <= bt && bt <= 'f'){
			b[j++/2] += (j%2) ? (bt - 'a' + 0x0A) : (bt - 'a' + 0x0A) << 4;
		}
	}

	if(j%2){
		Error("write");
	}

	Size = j/2;

	FullAccess(Address, Size);
	RtlCopyMemory((LPVOID)Address, b, Size);
}

void memory::scan(char *Aob, char *code){
	int i, j = 0;
	BYTE b[256], mask[256], bt;
	DWORD Size, Address, Result = 0, Target = 0;
	char *cmd = NULL;
	bool bAll = false;

	ZeroMemory(b, 256);
	ZeroMemory(mask, 256);
	ZeroMemory(AobResult, MAX_RESULT);
	CountResult = 0;

	for(i=0; Aob[i]; i++){
		bt = (BYTE)Aob[i];
		if('0' <= bt && bt <= '9'){
			b[j++/2] += (j%2) ? (bt - '0') : (bt - '0') << 4;
		}
		else if('A' <= bt && bt <= 'F'){
			b[j++/2] += (j%2) ? (bt - 'A' + 0x0A) : (bt - 'A' + 0x0A) << 4;
		}
		else if('a' <= bt && bt <= 'f'){
			b[j++/2] += (j%2) ? (bt - 'a' + 0x0A) : (bt - 'a' + 0x0A) << 4;
		}
		else if(bt == '?'){
			mask[j++/2] = true;
		}
		else if(bt == '-'){
			cmd = &Aob[i+1];
			if(strstr(cmd, "all")){
				bAll = true;
			}
			else{
				sscanf(cmd, "%d", &Target);
			}
			break;
		}
	}

	if(j%2){
		Error("Aob");
	}


	Size = j/2;

	for(Address = BaseAddress; Address < (BaseAddress + ImageSize); Address++){
// bien
//		for(i=0; i<Size; i++){
		for(i=0; i<(int)Size; i++){
//			if(*(BYTE *)(Address + i) != b[i] && mask[i] != true){
			if(*(BYTE *)(Address + i) != b[i] && mask[i] == 0x00){
				break;
			}
		}
		if(i==Size){
			AobResult[Result++] = Address;
			if(Target == 0 || Target == Result || bAll){
				if(code){
					write(Address, code);
				}
				if(!bAll){
					return;
				}
			}
		}
	}

	CountResult = Result;
	if(!bAll){
		Error("Aob");
	}
}

DWORD memory::get(DWORD Result){
	return AobResult[Result];
}

DWORD memory::count(){
	return CountResult;
}

#define push_byte 0x6A
#define push_dword 0x68

#define push_ebp 0x55
#define mov_ebp_esp 0xEC8B

#define sub_esp_dword 0xEC81
#define mov_edi_edi 0xFF8B
#define mov_eax_fs 0xA164

void memory::Hook(DWORD Address, void *function_hook, void *_function, DWORD Size){
	DWORD Memory = NULL;


	if(Size >= 5){
		FullAccess(Address, Size);
		Memory = createfunction(Address, Size);
		jmp(Address, (DWORD)function_hook);
		*(DWORD *)_function = Memory;
		return;
	}

	bool bhp = HotPatchCheck(Address);

	FullAccess(Address - 0x05, 0x0C);

	if(*(BYTE *)Address == push_byte){
		Size = 2;
		if(*(BYTE *)(Address + 0x02) == push_dword){
			Size += 5;
		}
	}
	else if(*(WORD *)Address == sub_esp_dword){
		Size = 6;
	}
	else if(*(BYTE *)Address == push_ebp && *(WORD *)(Address + 0x01) == mov_ebp_esp && *(BYTE *)(Address + 0x03) == push_byte){
		Size = 5;
	}

	
	if(Size >= 5){
		Memory = createfunction(Address, Size);
		jmp(Address, (DWORD)function_hook);
	}
	else if(Size == 2 && bhp == true){
		Memory = createfunction(Address, Size);
		jmp(Address - 5, (DWORD)function_hook);
		backjmp(Address);
	}
	else{
		Error("hook");
	}

	*(DWORD *)_function = Memory;
}


DWORD memory::createfunction(DWORD Address, DWORD Size){
	DWORD Memory = (DWORD)malloc(Size + 5);
	FullAccess(Memory, Size + 5);
	RtlCopyMemory((LPVOID)Memory, (LPVOID)Address, Size);
	jmp(Memory + Size, Address + Size);
	return Memory;
}

bool memory::HotPatchCheck(DWORD Address){
	int i;
	for(i=1; i<=5; i++){
		if(*(BYTE *)(Address - i) != 0x90 && *(BYTE *)(Address - i) != 0xCC){
			return false;
		}
	}
	return true;
}

#define BackJMP 0xF9EB

void memory::backjmp(DWORD Address){
	*(WORD *)Address = BackJMP;
}

void memory::jmp(DWORD Prev, DWORD Next){
	*(BYTE *)Prev = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}