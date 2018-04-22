#include"MemoryHacks.h"

char StrArray[128] = "0123456789ABCDEF";

BOOL Enable_Write_Memory(DWORD Addr, int Size){
	DWORD old;
	if(VirtualProtect((DWORD *)Addr, Size, PAGE_EXECUTE_READWRITE, &old) == FALSE){
		//ErrorMessage("Enable_Write_Memory");
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
		//ErrorMessage("@ChartoByte");
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}


DWORD Write_Hook(char code[], DWORD Prev, DWORD Next, int nop_count = 0){
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
			//ErrorMessage("@Write_Hook");
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

BOOL Write_code(DWORD Addr, char Array[], int nop_count = 0){
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

int ChartoAob(char Array[], BYTE b[], BOOL m[]){
   int i,Hit=0;
   for(i=0; Array[i]!=0x00; i++){
     if(Array[i] == 0x3F){b[Hit]=0x00; m[Hit/2] = TRUE; Hit++;continue;}
     if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
     else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
     else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
   }

   if(Hit%2!=0){
     //ErrorMessage("@ChartoByte");
     return -1;
   }

   Hit=Hit/2;

   for(i=0; i<Hit; i++){
     b[i]=b[i*2]*0x10+b[i*2+1];
   }

   return Hit;
}


DWORD AobScan(char Array[], char code[] = NULL, DWORD Memory_Start = 0x00400000, DWORD Memory_End = 0x02000000){
   BYTE ByteCode[0x100];
   BOOL Mask[0x100] = {0};
   int hit, i;
   DWORD MS_Memory;
   hit = ChartoAob(Array, ByteCode, Mask);

   __try{

   for(MS_Memory = Memory_Start; MS_Memory < Memory_End; MS_Memory++){

     for(i=0; i<hit; i++){
       if(Mask[i] == TRUE){
         continue;
       }
         if(*(BYTE *)(MS_Memory + i)^ByteCode[i]){
           break;
         }
     }
     if(i == hit){
		 if(code == NULL){
			return MS_Memory;
		 }
			switch(*(DWORD *)code){
				case 0x00706D6A://jmp XXXXXXXX
				case 0x6C6C6163://call XXXXXXXX
					return *(DWORD *)(MS_Memory + 1) + MS_Memory + 5;

				case 0x0000656A://je XXXXXXXX
				case 0x00656E6A://jne XXXXXXXX
				case 0x0000626A://jb XXXXXXXX
				case 0x0000616A://ja XXXXXXXX
					return *(DWORD *)(MS_Memory + 2) + MS_Memory + 6;

				//mov eax,[XXXXXXXX]...
				case 0x00786165://eax
					return *(DWORD *)(MS_Memory + 1);
					
				//mov ecx,[XXXXXXXX]...
				case 0x00786265://ebx
				case 0x00786365://ecx
				case 0x00786465://ecx
				case 0x00697365://esi
				case 0x00496465://edi
					return *(DWORD *)(MS_Memory + 2);
			}
     }
   }

   }__except(EXCEPTION_EXECUTE_HANDLER){
     return 0;
   }

   return 0;
}


//=====================================================================================

unsigned char *Memory;
DWORD dwMemoryStart;
DWORD dwMemoryEnd;
DWORD dwCRCMainStart;
DWORD dwCRCMainEnd;

void MemoryDump(){
	IMAGE_NT_HEADERS *nth = (IMAGE_NT_HEADERS *)((DWORD)GetModuleHandleA(NULL) + PIMAGE_DOS_HEADER(GetModuleHandleA(NULL))->e_lfanew);
	Memory = (unsigned char *)malloc(nth->OptionalHeader.SizeOfImage);
	RtlCopyMemory(Memory, (DWORD *)nth->OptionalHeader.ImageBase, nth->OptionalHeader.SizeOfImage);
	dwMemoryStart = (DWORD)nth->OptionalHeader.ImageBase;
	dwMemoryEnd = dwMemoryStart+(DWORD)nth->OptionalHeader.SizeOfImage;
}

void _declspec(naked) CRCMainHook(){
	_asm{
		xor eax,eax
		add eax,edx
		mov edx,[ebp+0x18]
		sub eax,0x08
		mov eax,[edx]
		shr eax,0x08
		xor ecx,ecx
		mov ecx,eax
		shl ecx,0x08
		mov ecx,[ebp+0x08]
		add ecx,[ebp-0x38]
		xor edx,edx
		mov ebx,[ebp+0x08]
		cmp ecx,[dwMemoryStart]
		jb Ending
		cmp ecx,[dwMemoryEnd]
		ja Ending
		sub ecx,[dwMemoryStart]
		add ecx,Memory
Ending:
		jmp dwCRCMainEnd
	}
}


bool MSCRCBypass(){
	//dwCRCMainStart = AobScan("0F 85 ?? ?? ?? ?? 6A 00 E9");
	dwCRCMainStart = AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9")+6;
	dwCRCMainEnd = AobScan("8A 11 80 C2 01 8B 4D 18");

	if(!dwCRCMainStart || !dwCRCMainEnd) return false;

	MemoryDump();
	Write_Hook("jne", dwCRCMainStart, (DWORD)CRCMainHook);
	return true;
}

bool RemoveThemidaCRCs(){
	DWORD dwThemidaCRCs = AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 6A 01 8B 8D ?? ?? ?? ?? E8");

	if(!dwThemidaCRCs) return false;

	Write_code(dwThemidaCRCs, "33 C0 C3");
	return true;
}

bool VEHBypass(){
	DWORD dwVEH = AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");

	if(!dwVEH) return false;

	Write_code(dwVEH, "33 C0 C3");
	return true;
}

bool StopProcessListPacket(){
	DWORD dwPL = AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	
	if(!dwPL) return false;

	Write_code(dwPL, "33 C0 C2 04 00");
	return true;
}

bool KillProcesses(){
	DWORD dwASPLunchr = AobScan("83 EC 0C 56 57 6A 00 8B F1 E9");
	DWORD mkd25tray = AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", NULL, dwASPLunchr);
	DWORD dwAutoup = AobScan("56 8D 71 2C 8B CE E8");

	if(!dwASPLunchr || !mkd25tray || !dwAutoup) return false;

	Write_code(dwASPLunchr, "33 C0 C3");
	Write_code(mkd25tray, "33 C0 C3");
	Write_code(dwAutoup , "33 C0 C3");

	return true;

}

bool HSless(){
	DWORD dwHSless1 = AobScan("81 EC 20 01 00 00 A1 ?? ?? ?? ?? 33 C4 89 84 24 1C 01 00 00 55 56 6A 00 8B F1 E9");
	DWORD dwHSless2 = AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 92 01 00 00");
	
	if(!dwHSless1 || !dwHSless2) return false;
	
	Write_code(dwHSless1 , "33 C0 C2 04 00");
	Write_code(dwHSless2 , "33 C0 C2 04 00");

	return true;
}