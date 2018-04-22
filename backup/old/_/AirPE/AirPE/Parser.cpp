#include"Parser.h"

char SBuffer[8192*4];
DWORD SCurrentPointer;
DWORD SPacketSize;

char HEX[] = "0123456789ABCDEF0123456789abcdef";

void _stdcall AddBYTE(BYTE b){
	if(!bFormatView){
		return;
	}
	SBuffer[SCurrentPointer] = HEX[b/0x10];
	SBuffer[SCurrentPointer+1] = HEX[b%0x10];
	SBuffer[SCurrentPointer+2] = ' ';
	SCurrentPointer += 3;
	SPacketSize += 1;
}

void _stdcall AddWORD(WORD w){
	if(!bFormatView){
		return;
	}
	AddBYTE(w >> 8);
	SCurrentPointer--;
	AddBYTE(w);
}

void _stdcall AddDWORD(DWORD d){
	if(!bFormatView){
		return;
	}
	AddWORD(d >> 16);
	SCurrentPointer--;
	AddWORD(d);
}

void _stdcall AddQWORD(DWORD d1, DWORD d2){
	if(!bFormatView){
		return;
	}
	AddDWORD(d2);
	SCurrentPointer--;
	AddDWORD(d1);
}

void _stdcall AddString(char s[]){
	int i;
	if(!bFormatView){
		return;
	}
	SBuffer[SCurrentPointer] = '\"';
	for(i=0; s[i]; i++){
		SBuffer[SCurrentPointer+1+i] = s[i];
	}
	SBuffer[SCurrentPointer+1+i] = '\"';
	SBuffer[SCurrentPointer+1+i+1] = ' ';
	SCurrentPointer += i + 3;

	SPacketSize += 2 + i;//WORD(size) + String
	
}

void _stdcall AddBuffer(BYTE Buffer[], DWORD BufferSize){
	int i;
	if(!bFormatView){
		return;
	}
	for(i=0; i<BufferSize; i++){
		AddBYTE(Buffer[i]);
		SCurrentPointer--;
	}
	SCurrentPointer++;
}

void _stdcall SFinish(){
	if(!bFormatView){
		return;
	}
	SBuffer[--SCurrentPointer] = '\0';
}

DWORD InitPacket = 0x00AE9B80;
DWORD Encode1 = 0x00499A00;
DWORD Encode2 = 0x00499A50;
DWORD Encode4 = 0x004917D0;
DWORD Encode8 = 0x00528C70;
DWORD EncodeString = 0x00529AF0;
DWORD EncodeBuffer = 0x00598650;

DWORD InitPacket_Ret = InitPacket + 5;
DWORD Encode1_Ret = Encode1 + 6;
DWORD Encode2_Ret = Encode2 + 6;
DWORD Encode4_Ret = Encode4 + 6;
DWORD Encode8_Ret = Encode8 + 6;
DWORD EncodeString_Ret = EncodeString + 7;
DWORD EncodeBuffer_Ret = EncodeBuffer + 7;

void _declspec(naked) InitPacket_Hook(){
	_asm{
		mov dword ptr [SCurrentPointer],0
		mov dword ptr [SPacketSize],0
		mov eax,[esp+0x08]
		push esi
		jmp dword ptr [InitPacket_Ret]
	}
}

void _declspec(naked) Encode1_Hook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x04]
		call AddBYTE
		popad
		push esi
		mov esi,ecx
		mov eax,[esi+0x04]
		jmp dword ptr [Encode1_Ret]
	}
}

void _declspec(naked) Encode2_Hook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x04]
		call AddWORD
		popad
		push esi
		mov esi,ecx
		mov eax,[esi+0x04]
		jmp dword ptr [Encode2_Ret]
	}
}

void _declspec(naked) Encode4_Hook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x04]
		call AddDWORD
		popad
		push esi
		mov esi,ecx
		mov eax,[esi+0x04]
		jmp dword ptr [Encode4_Ret]
	}
}

void _declspec(naked) Encode8_Hook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x08]
		push [eax+0x04]
		call AddQWORD
		popad
		push esi
		mov esi,ecx
		mov eax,[esi+0x04]
		jmp dword ptr [Encode8_Ret]
	}
}

void _declspec(naked) EncodeString_Hook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x04]
		call AddString
		popad
		push 0xFFFFFFFF
		push 0x015D4F58
		jmp dword ptr [EncodeString_Ret]
	}
}

void _declspec(naked) EncodeBuffer_Hook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x08]
		push [eax+0x04]
		call AddBuffer
		popad
		push ebx
		push esi
		mov esi,ecx
		mov eax,[esi+0x04]
		jmp dword ptr [EncodeBuffer_Ret]
	}
}

void JMP(DWORD dwPrev, DWORD dwNext){
	DWORD old;
	VirtualProtect((DWORD *)dwPrev, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext -  dwPrev -5;
}

void EncodeHook(){
	JMP(InitPacket, (DWORD)InitPacket_Hook);
	JMP(Encode1, (DWORD)Encode1_Hook);
	JMP(Encode2, (DWORD)Encode2_Hook);
	JMP(Encode4, (DWORD)Encode4_Hook);
	JMP(Encode8, (DWORD)Encode8_Hook);
	JMP(EncodeString, (DWORD)EncodeString_Hook);
	JMP(EncodeBuffer, (DWORD)EncodeBuffer_Hook);
	//JMP(SendPacket, (DWORD)SendPacket_Hook);
}