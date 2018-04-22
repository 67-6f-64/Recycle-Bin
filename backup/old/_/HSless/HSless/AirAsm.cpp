#include"AirAsm.h"
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

BYTE* MemoryDump(char *ModuleName, DWORD *Memory_Start, DWORD *Memory_End){
	HMODULE hTarget;
	IMAGE_NT_HEADERS *nth;
	MODULEINFO hInfo;
	BYTE *Memory;

	if(ModuleName == NULL){
		hTarget = GetModuleHandleA(NULL);
		nth = (IMAGE_NT_HEADERS *)((DWORD)hTarget + PIMAGE_DOS_HEADER(hTarget)->e_lfanew);
		Memory_Start[0] = (DWORD)nth->OptionalHeader.ImageBase;
		Memory_End[0] = Memory_Start[0] + (DWORD)nth->OptionalHeader.SizeOfImage;
		Memory = (BYTE *)malloc((DWORD)nth->OptionalHeader.SizeOfImage);
		RtlCopyMemory(Memory, (DWORD *)nth->OptionalHeader.ImageBase, (DWORD)nth->OptionalHeader.SizeOfImage);
	}
	else{
		hTarget = GetModuleHandleA(ModuleName);
		GetModuleInformation(GetCurrentProcess(), hTarget, &hInfo, sizeof(MODULEINFO));
		Memory_Start[0] = (DWORD)hInfo.lpBaseOfDll;
		Memory_End[0] = Memory_Start[0] + (DWORD)hInfo.SizeOfImage;
		Memory = (BYTE *)malloc((DWORD)hInfo.SizeOfImage);
		RtlCopyMemory(Memory, (DWORD *)hInfo.lpBaseOfDll, (DWORD)hInfo.SizeOfImage);
	}

	return Memory;
}


int ChartoAob(char Array[], BYTE b[], BYTE m[]){
   int i,Hit=0;
   for(i=0; Array[i]!=0x00; i++){
     if(Array[i] == 0x3F){b[Hit]=0x00; m[Hit/2] = 1; Hit++; continue;}
     if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
     else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
     else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
   }

   Hit=Hit/2;

   for(i=0; i<Hit; i++){
     b[i]=b[i*2]*0x10+b[i*2+1];
   }

   return Hit;
}

DWORD AobScan(char *String, DWORD result, char *ModuleName){
	HMODULE hTarget;
	IMAGE_NT_HEADERS *nth;
	MODULEINFO hInfo;
	DWORD Memory_Start;
	DWORD Memory_End;
	BYTE Aob[0x100] ={0};
	BYTE Mask[0x100] = {0};
	DWORD dwSize = ChartoAob(String, Aob, Mask);
	DWORD i, j, k = 0;

	if(ModuleName == NULL){
		hTarget = GetModuleHandleA(NULL);
		nth = (IMAGE_NT_HEADERS *)((DWORD)hTarget + PIMAGE_DOS_HEADER(hTarget)->e_lfanew);
		Memory_Start = (DWORD)nth->OptionalHeader.ImageBase;
		Memory_End = Memory_Start + (DWORD)nth->OptionalHeader.SizeOfImage;
	}
	else{
	
		hTarget = GetModuleHandleA(ModuleName);
		GetModuleInformation(GetCurrentProcess(), hTarget, &hInfo, sizeof(MODULEINFO));
		Memory_Start = (DWORD)hInfo.lpBaseOfDll;
		Memory_End = Memory_Start + hInfo.SizeOfImage;
	}

	__try{
		for(i=Memory_Start; i<Memory_End; i++){
			for(j=0; j< dwSize; j++){
				if(Mask[j]){
					continue;
				}
				if(Aob[j]^(*(BYTE*)(i+j))){
					break;
				}
			}
			if(j==dwSize){
				k++;
				if(result > k){
					continue;
				}
				return i;
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return 0;
	}

	return 0;
}


char StrArray[128] = "0123456789ABCDEF";

BOOL Enable_Write_Memory(DWORD Addr, int Size){
	DWORD old;
	if(VirtualProtect((DWORD *)Addr, Size, PAGE_EXECUTE_READWRITE, &old) == FALSE){
		return FALSE;
	}

	return TRUE;
}

int ChartoByte(char Array[], BYTE b[]){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x2A){Array[i] = StrArray[rand()%0x10];}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}


DWORD WriteHook(DWORD Prev, char code[], DWORD Next, int nop_count){
	int i;
	BOOL Flag = FALSE;

	if(Enable_Write_Memory(Prev, 7 + nop_count) == FALSE){
		return FALSE;
	}

	switch(*(DWORD *)code){
		case 0x00706D6A://jmp
			*(BYTE *)Prev = 0xE9;
			break;

		case 0x6C6C6163://call
			*(BYTE *)Prev = 0xE8;
			break;

		case 0x0000656A://je
			*(WORD *)Prev = 0x840F;
			Flag = TRUE;
			break;

		case 0x00656E6A://jne
			*(WORD *)Prev = 0x850F;
			Flag = TRUE;
			break;
			
		case 0x0000626A://jb
			*(WORD *)Prev = 0x820F;
			Flag = TRUE;
			break;

		case 0x0000616A://ja
			*(WORD *)Prev = 0x870F;
			Flag = TRUE;
			break;

		default:
			return FALSE;
			break;
	}

	*(DWORD *)(Prev + 1 + Flag) = Next - Prev - 5 - Flag;

	if(nop_count == 0){
		return Prev + 5 + Flag;
	}

	for(i=0; i < nop_count; i++){
		*(BYTE *)(Prev + 5 + Flag + i) = 0x90;//nop
	}

	return Prev + 5 + nop_count + Flag;
}


DWORD WritePointerHook(DWORD Pointer, DWORD HookAddr){
	DWORD Function;

	if(Enable_Write_Memory(Pointer, 4) == FALSE){
		return FALSE;
	}

	Function = *(DWORD *)Pointer;

	 if(Function == 0){
		 return FALSE;
	 }

	 *(DWORD *)Pointer = HookAddr;
	 return Function;
}


BOOL WriteCode(DWORD Addr, char Array[], int nop_count){
	int i, Hit;
	BYTE ByteCode[0x100];//

	if(Array != NULL && Array[0] != 0x00){


	Hit = ChartoByte(Array, ByteCode);

	if(Enable_Write_Memory(Addr, Hit) == FALSE){
		return FALSE;
	}

	for(i=0; i<Hit; i++){
		*(BYTE *)(Addr + i) = ByteCode[i];
	}

	if(nop_count == 0){
		return TRUE;
	}

	if(Enable_Write_Memory(Addr + Hit, nop_count) == FALSE){
		return FALSE;
	}

	}else{
		Hit = 0;
	}

	for(i=0; i<nop_count; i++){
		*(BYTE *)(Addr + Hit + i) = 0x90;//nop
	}


	return TRUE;
}