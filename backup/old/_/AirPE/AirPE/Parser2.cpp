#include"Parser.h"

char RBuffer[8192*4];
DWORD RCurrentPointer;
DWORD RPacketSize;
DWORD orgRSize;
DWORD orgRReturn;

char HEX2[] = "0123456789ABCDEF0123456789abcdef";

void _stdcall AddBYTE2(BYTE b){
	if(!bFormatView){
		return;
	}
	RBuffer[RCurrentPointer] = HEX2[b/0x10];
	RBuffer[RCurrentPointer+1] = HEX2[b%0x10];
	RBuffer[RCurrentPointer+2] = ' ';
	RCurrentPointer += 3;
	RPacketSize += 1;
}

void _stdcall AddWORD2(WORD w){
	if(!bFormatView){
		return;
	}
	AddBYTE2(w >> 8);
	RCurrentPointer--;
	AddBYTE2(w);
}

void _stdcall AddDWORD2(DWORD d){
	if(!bFormatView){
		return;
	}
	AddWORD2(d >> 16);
	RCurrentPointer--;
	AddWORD2(d);
}

void _stdcall AddQWORD2(DWORD d1, DWORD d2){
	if(!bFormatView){
		return;
	}
	AddDWORD2(d2);
	RCurrentPointer--;
	AddDWORD2(d1);
}

void _stdcall AddString2(char s[]){
	int i;

	if(!bFormatView){
		return;
	}

	RBuffer[RCurrentPointer] = '\"';
	for(i=0; s[i]; i++){
		RBuffer[RCurrentPointer+1+i] = s[i];
	}
	RBuffer[RCurrentPointer+1+i] = '\"';
	RBuffer[RCurrentPointer+1+i+1] = ' ';
	RCurrentPointer += i + 3;

	RPacketSize += 2 + i;//WORD(size) + String
	
}

//bool BufferReading = false;

void _stdcall AddBuffer2(BYTE Buffer[], DWORD BufferSize){
	int i;
	if(!bFormatView){
		return;
	}
	/*
	if(BufferReading){
		if(!BufferSize){
			BufferReading = false;
		}
		return;
	}
	
	BufferReading = true;
	*/

	for(i=0; i<BufferSize; i++){
		AddBYTE2(Buffer[i]);
		RCurrentPointer--;
	}
	RCurrentPointer++;
}

void _stdcall RInit(){
	RPacketSize = 0;
	RCurrentPointer = 0;
}

void _stdcall RFinish(){
	if(!bFormatView){
		return;
	}
	RBuffer[--RCurrentPointer] = '\0';
}

DWORD C_RemoveAll = 0x005F0D1D;
DWORD RemoveAll = 0x00490BF0;
DWORD Decode1 = 0x00414860;
DWORD Decode2 = 0x00414900;
DWORD Decode4 = 0x004149A0;
DWORD Decode8 = 0x005269C0;
DWORD DecodeString = 0x004BABD0;
DWORD DecodeBuffer = 0x00414A40;
DWORD Decode1_Ret = Decode1 + 5;
DWORD Decode2_Ret = Decode2 + 5;
DWORD Decode4_Ret = Decode4 + 5;
DWORD Decode8_Ret = Decode8 + 5;
DWORD DecodeString_Ret = DecodeString + 5;
DWORD DecodeBuffer_Ret = DecodeBuffer + 5;

void _declspec(naked) RemoveAll_Hook(){
	_asm{
		pushad
		call DecodeFinish
		popad
		jmp dword ptr [RemoveAll]
	}
}


void _declspec(naked) Decode1_Hook(){
	_asm{
		call Decode1_org
		pushad
		push eax
		call AddBYTE2
		popad
		ret
Decode1_org:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [Decode1_Ret]
	}
}

void _declspec(naked) Decode2_Hook(){
	_asm{
		call Decode2_org
		pushad
		push eax
		call AddWORD2
		popad
		ret
Decode2_org:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [Decode2_Ret]
	}
}

void _declspec(naked) Decode4_Hook(){
	_asm{
		call Decode4_org
		pushad
		push eax
		call AddDWORD2
		popad
		ret
Decode4_org:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [Decode4_Ret]
	}
}

void _declspec(naked) Decode8_Hook(){
	_asm{
		call Decode8_org
		pushad
		push edx
		push eax
		call AddQWORD2
		popad
		ret
Decode8_org:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [Decode8_Ret]
	}
}


void _declspec(naked) DecodeString_Hook(){
	_asm{
		mov eax,esp
		push [eax+0x04]
		call DecodeString_org
		pushad
		push [eax]
		call AddString2
		popad
		ret 0x0004
DecodeString_org:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [DecodeString_Ret]
	}
}

void _declspec(naked) DecodeBuffer_Hook(){
	_asm{
		mov eax,esp
		push [eax+0x08]
		push [eax+0x04]
		call DecodeBuffer_org
		push eax
		mov eax,esp
		pushad
		push [eax+0x0C]
		push [eax+0x08]
		call AddBuffer2
		popad
		pop eax
		ret 0x0008
DecodeBuffer_org:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [DecodeBuffer_Ret]
	}
}

void JMP2(DWORD dwPrev, DWORD dwNext){
	DWORD old;
	VirtualProtect((DWORD *)dwPrev, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext -  dwPrev -5;
}

void CALL2(DWORD dwPrev, DWORD dwNext){
	DWORD old;
	VirtualProtect((DWORD *)dwPrev, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE *)(dwPrev) = 0xE8;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext -  dwPrev -5;
}


void DecodeHook(){
	JMP2(Decode1, (DWORD)Decode1_Hook);
	JMP2(Decode2, (DWORD)Decode2_Hook);
	JMP2(Decode4, (DWORD)Decode4_Hook);
	JMP2(Decode8, (DWORD)Decode8_Hook);
	CALL2(C_RemoveAll, (DWORD)RemoveAll_Hook);
	JMP2(DecodeString, (DWORD)DecodeString_Hook);
	JMP2(DecodeBuffer, (DWORD)DecodeBuffer_Hook);
	//JMP(SendPacket, (DWORD)SendPacket_Hook);
}