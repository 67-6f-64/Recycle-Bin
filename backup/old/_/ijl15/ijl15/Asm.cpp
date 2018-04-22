#include"Asm.h"

namespace Asm{
	void ErrorMessage(char message[]);
	BOOL Enable_Write_Memory(DWORD Addr, int Size);
	//BOOL ChartoAob(char Array[], Aob sc);
	int ChartoAob(char Array[], BYTE b[], BOOL m[]);
}

namespace Asm{
	char StrArray[128] = "0123456789ABCDEF";

}


BOOL Asm::BytetoChar(BYTE Packet[], int Size, char StrPacket[]){
	int i,p;

	p = 0;

	if(Size < 2){
		return FALSE;
	}

	for(i=0; i<Size; i++){
		StrPacket[p] = StrArray[Packet[i]/0x10];p++;
		StrPacket[p] = StrArray[Packet[i]%0x10];p++;
		StrPacket[p] = 0x20;p++;
	}
		StrPacket[p-1] = 0x00;

	return TRUE;
}


void Asm::ErrorMessage(char message[]){
	MessageBoxA(NULL, message, "Asm::ErrorMessage", NULL);
}



BOOL Asm::Enable_Write_Memory(DWORD Addr, int Size){
	DWORD old;
	if(VirtualProtect((DWORD *)Addr, Size, PAGE_EXECUTE_READWRITE, &old) == FALSE){
		ErrorMessage("Enable_Write_Memory");
		return FALSE;
	}

	return TRUE;
}


DWORD Asm::Write_Hook(char code[], DWORD Prev, DWORD Next, int nop_count){
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
			ErrorMessage("@Write_Hook");
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


DWORD Asm::Write_Pointer_Hook(DWORD Pointer, DWORD HookAddr){
	DWORD Function;

	if(Enable_Write_Memory(Pointer, 4) == FALSE){
		return FALSE;
	}

	Function = *(DWORD *)Pointer;

	 if(Function == 0){
		 ErrorMessage("@Write_Pointer_Hook");
		 return FALSE;
	 }

	 *(DWORD *)Pointer = HookAddr;
	 return Function;
}


BOOL Asm::Write_code(DWORD Addr, char Array[], int nop_count){
	int i, Hit;
	BYTE ByteCode[Asm_Byte_Code_Max];//

	if(Array != NULL && Array[0] != 0x00){


	Hit = Asm::ChartoByte(Array, ByteCode);

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


int Asm::ChartoByte(char Array[], BYTE b[]){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x2A){Array[i] = StrArray[rand()%0x10];}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		ErrorMessage("@ChartoByte");
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}

/*
BOOL Asm::ChartoAob(char Array[], Aob sc){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x3F){sc.b[Hit]=0x00; sc.m[Hit/2] = TRUE; Hit++;continue;}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {sc.b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {sc.b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {sc.b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		ErrorMessage("@ChartoAob");
		return FALSE;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		sc.b[i]=sc.b[i*2]*0x10+sc.b[i*2+1];
	}

	sc.s = Hit;

	return TRUE;
}
*/

int Asm::ChartoAob(char Array[], BYTE b[], BOOL m[]){
   int i,Hit=0;
   for(i=0; Array[i]!=0x00; i++){
     if(Array[i] == 0x3F){b[Hit]=0x00; m[Hit/2] = TRUE; Hit++;continue;}
     if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
     else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
     else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
   }

   if(Hit%2!=0){
     ErrorMessage("@ChartoByte");
     return -1;
   }

   Hit=Hit/2;

   for(i=0; i<Hit; i++){
     b[i]=b[i*2]*0x10+b[i*2+1];
   }

   return Hit;
}


/*
DWORD Asm::AobScan(char Array[], char code[], DWORD Memory_Start, DWORD Memory_End){
	Aob sc;
	ZeroMemory(sc.m, Asm_Byte_Code_Max);
	ZeroMemory(sc.b, Asm_Byte_Code_Max);
	int i;
	BOOL Flag;

	DWORD MS_Memory;
	Flag = ChartoAob(Array, sc);

	if(Flag == FALSE){
		return 0;
	}

	__try{

	for(MS_Memory = Memory_Start; MS_Memory < Memory_End; MS_Memory++){

		for(i=0; i<sc.s; i++){
			if(sc.m[i] == TRUE){
				continue;
			}
				if(*(BYTE *)(MS_Memory + i)^sc.b[i]){
					break;
				}
		}
		if(i == sc.s){
			if(code == NULL){
				return MS_Memory;
			 }
			switch(*(DWORD *)code){
				case 0x00706D6A://jmp
				case 0x6C6C6163://call
					return *(DWORD *)(MS_Memory + 1) + MS_Memory + 5;

				case 0x0000656A://je
				case 0x00656E6A://jne
				case 0x0000626A://jb
				case 0x0000616A://ja
					return *(DWORD *)(MS_Memory + 2) + MS_Memory + 6;

				case 0x00786165://eax
					return *(DWORD *)(MS_Memory + 1);
					
				case 0x00786265://ebx
				case 0x00786365://ecx
				case 0x00786465://ecx
				case 0x00697365://esi
				case 0x00496465://edi
					return *(DWORD *)(MS_Memory + 2);

				default:
					return MS_Memory;
			}
		}
	}

	}__except(EXCEPTION_EXECUTE_HANDLER){
		return 0;
	}



	return 0;
}
*/



DWORD Asm::AobScan(char Array[], char code[], DWORD Memory_Start, DWORD Memory_End){
   BYTE ByteCode[Asm_Byte_Code_Max];
   BOOL Mask[Asm_Byte_Code_Max] = {0};
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


DWORD Asm::ReadPointer(DWORD BaseAddr, DWORD Offset){
	__try{
		return *(DWORD *)((*(DWORD *)BaseAddr) + Offset);
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return 0;
	}
}

DWORD Asm::ReadPointerAddress(DWORD BaseAddr, DWORD Offset){
	__try{
		return (*(DWORD *)BaseAddr) + Offset;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return 0;
	}
}


void Asm::Debug(BOOL Flag){
	if(Flag == TRUE){
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
	}else{
		FreeConsole();
	}
}


void Asm::CodeCave(DWORD dwHookAddr, DWORD dwCodeSize, char opcode[], BOOL originalcode){
	DWORD i;
	BYTE injectcode[0x40];
	DWORD ic_size = Asm::ChartoByte(opcode, injectcode);
	DWORD newmem = (DWORD)new BYTE[25+dwCodeSize+ic_size+5+4];
	VirtualProtect((DWORD *)newmem, 25+dwCodeSize+ic_size+5+4, PAGE_EXECUTE_READWRITE, &i);

	if(dwCodeSize < 5) return;

	if(!VirtualProtect((DWORD *)(dwHookAddr), dwCodeSize, PAGE_EXECUTE_READWRITE, &i)) return;


	for(i=0; i<ic_size; i++) *(BYTE *)(newmem+i) = injectcode[i];

	if(originalcode == TRUE){
		for(i=0; i<dwCodeSize; i++) *(BYTE *)(newmem+ic_size+i) = *(BYTE *)(dwHookAddr+i);
		*(BYTE *)(newmem+ic_size+dwCodeSize) = 0xE9;
		*(DWORD *)(newmem+ic_size+dwCodeSize+1) = (dwHookAddr+dwCodeSize)-(newmem+ic_size+dwCodeSize)-5;
	}else{
		*(BYTE *)(newmem+ic_size) = 0xE9;
		*(DWORD *)(newmem+ic_size+1) = (dwHookAddr+dwCodeSize)-(newmem+ic_size)-5;
	}

	*(BYTE *)(dwHookAddr)  = 0xE9;
	*(DWORD *)(dwHookAddr+1)  = newmem-dwHookAddr-5;
	for(i=5; i<dwCodeSize; i++) *(BYTE *)(dwHookAddr+i) = 0x90;

	return;
}

void Asm::StackHook(DWORD dwHookAddr, DWORD dwCodeSize, DWORD dwFunction, char opcode[], BOOL ebp){
	DWORD i;
	BYTE injectcode[0x40];
	DWORD ic_size = Asm::ChartoByte(opcode, injectcode);
	DWORD newmem = (DWORD)new BYTE[25+dwCodeSize+ic_size+5+4];
	VirtualProtect((DWORD *)newmem, 25+dwCodeSize+ic_size+5+4, PAGE_EXECUTE_READWRITE, &i);

	if(dwCodeSize < 5) return;

	if(!VirtualProtect((DWORD *)(dwHookAddr), dwCodeSize, PAGE_EXECUTE_READWRITE, &i)) return;

	*(WORD *)(newmem) = 0x0589;
	*(DWORD *)(newmem+2) = newmem+25+dwCodeSize+ic_size+5;
	if(!ebp) *(DWORD *)(newmem+6) = 0x5060C48B;
	else *(DWORD *)(newmem+6) = 0x5060C58B;
	*(BYTE *)(newmem+10) = 0xE8;
	*(DWORD *)(newmem+11) = dwFunction-(newmem+10)-5;
	
	*(WORD *)(newmem+15) = 0xC085;
	*(BYTE *)(newmem+17) = 0x61;
	*(BYTE *)(newmem+18) = 0x74;
	*(BYTE *)(newmem+19) = ic_size;

	for(i=0; i<ic_size; i++) *(BYTE *)(newmem+20+i) = injectcode[i];

	*(BYTE *)(newmem+20+ic_size) = 0xA1;
	*(DWORD *)(newmem+ic_size+21) = newmem+25+dwCodeSize+ic_size+5;

	for(i=0; i<dwCodeSize; i++) *(BYTE *)(newmem+25+ic_size+i) = *(BYTE *)(dwHookAddr+i);

	*(BYTE *)(newmem+25+dwCodeSize+ic_size) = 0xE9;
	*(DWORD *)(newmem+25+dwCodeSize+ic_size+1) = (dwHookAddr+dwCodeSize)-(newmem+25+dwCodeSize+ic_size)-5;

	*(BYTE *)(dwHookAddr)  = 0xE9;
	*(DWORD *)(dwHookAddr+1)  = newmem-dwHookAddr-5;
	for(i=5; i<dwCodeSize; i++) *(BYTE *)(dwHookAddr+i) = 0x90;

	return;
}


void Asm::HookAPI2(char ModuleName[], char ProcName[], DWORD func, DWORD hikisuu, DWORD retval){
	HMODULE hModule;
	DWORD StartRoutine;
	DWORD old;
	DWORD garbage = (DWORD)new BYTE[0x100];
	VirtualProtect((DWORD *)garbage, 0x100, PAGE_EXECUTE_READWRITE, &old);

	//DLLの先頭アドレス取得
	hModule = GetModuleHandleA(ModuleName);
	if(!hModule){
		printf("ERROR(1)@%s\n", ModuleName);
		return;
	}

	//関数の先頭アドレス取得
	StartRoutine = (DWORD)GetProcAddress(hModule, ProcName);
	if(!StartRoutine){
		printf("ERROR(1)@%s\n", ProcName);
		return;
	}

	//メモリ読み書き実行可能に設定
	if(!VirtualProtect((DWORD *)(StartRoutine-5), 7, PAGE_EXECUTE_READWRITE, &old)){
		printf("ERROR(2)@%s\n", ProcName);
		return;
	}

	//メモリ上に実行可能というかフックコード書き込み
	*(DWORD *)(garbage) = 0x5060C48B;
	*(BYTE *)(garbage+4) = 0xE8;
	*(DWORD *)(garbage+5) = func-(garbage+4)-5;
	*(WORD *)(garbage+9) = 0xC085;
	*(BYTE *)(garbage+11) = 0x61;
	*(WORD *)(garbage+12) = 0x0775;
	*(WORD *)(garbage+14) = *(WORD *)StartRoutine;//書き換え前のメモリ(2bytes)
	*(BYTE *)(garbage+16) = 0xE9;
	*(DWORD *)(garbage +17) = (StartRoutine+2)-(garbage+16)-5;
	*(BYTE *)(garbage+21) = 0xB8;
	*(DWORD *)(garbage+22) = retval;
	
	if(hikisuu){
		*(BYTE *)(garbage+26) =0xC2;
		*(WORD *)(garbage+27) =hikisuu*4;
	}else{
		*(BYTE *)(garbage+26) =0xC3;
	}

	//書き込みスペースに書き込み
	*(BYTE *)(StartRoutine -5) = 0xE9;
	*(DWORD *)(StartRoutine -4) = (DWORD)garbage-(StartRoutine-5)-5;

	//書き込みスペースにジャンプ
	*(WORD *)StartRoutine = 0xF9EB;

	return;
}