#include<Windows.h>
#include"Hook.h"
#include"memory.h"
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)
#include"Packet.h"
#include"gui_pe.h"
#include<stdio.h>

//Buffer
char FormatS[65536];
char FormatR[65536];

//CClientSocket
void (_fastcall *_SendPacket)(LPVOID ecx, LPVOID edx, OutPacket *p);
void (_fastcall *_ProcessPacket)(LPVOID ecx, LPVOID edx, InPacket *p);

//COutPacket
void (_fastcall *_Init)(OutPacket *p, LPVOID edx, WORD w);
void (_fastcall *_Encode1)(OutPacket *p, LPVOID edx, BYTE b);
void (_fastcall *_Encode2)(OutPacket *p, LPVOID edx, WORD w);
void (_fastcall *_Encode4)(OutPacket *p, LPVOID edx, DWORD dw);
void (_fastcall *_Encode8)(OutPacket *p, LPVOID edx, ULONGLONG ull);
void (_fastcall *_EncodeStr)(OutPacket *p, LPVOID edx, char *s);
void (_fastcall *_EncodeBuffer)(OutPacket *p, LPVOID edx, BYTE *b, int Length);

//CInPacket
BYTE (_fastcall *_Decode1)(InPacket *p, LPVOID edx);
WORD (_fastcall *_Decode2)(InPacket *p, LPVOID edx);
DWORD (_fastcall *_Decode4)(InPacket *p, LPVOID edx);
ULONGLONG (_fastcall *_Decode8)(InPacket *p, LPVOID edx);
char** (_fastcall *_DecodeStr)(InPacket *p, LPVOID edx, LPVOID lpv1);
void (_fastcall *_DecodeBuffer)(InPacket *p, LPVOID edx, BYTE *b, int Length);

DWORD dwSendPacketReturn;
void _fastcall SendPacket(DWORD CClientSocket, LPVOID edx, OutPacket *p){
	DWORD Return = (DWORD)_ReturnAddress();

	AddLog(0, Return, p->Length, p->Packet, FormatS);

	_asm{
		push SP_Return
		push [p]
		push [dwSendPacketReturn]
		mov ecx,[CClientSocket]
		jmp dword ptr [_SendPacket]
SP_Return:
	}

}

void _fastcall ProcessPacket(LPVOID ecx, LPVOID edx, InPacket *p){
	FormatR[0] = 0;//‰Šú‰»
	_ProcessPacket(ecx, edx, p);
	AddLog(1, p->Return, p->DataLength, p->Packet, FormatR);
}

void _fastcall Init(OutPacket *p, LPVOID edx, WORD w){
	FormatS[0] = 0;//‰Šú‰»
	return _Init(p, edx, w);
}

void _fastcall Encode1(OutPacket *p, LPVOID edx, BYTE b){
	sprintf(&FormatS[strlen(FormatS)], " %02X", b);
	return _Encode1(p, edx, b);
}

void _fastcall Encode2(OutPacket *p, LPVOID edx, WORD w){
	if(FormatS[0] == NULL){
		sprintf(&FormatS[0], "@%04X", w);
	}
	else{
		sprintf(&FormatS[strlen(FormatS)], " %04X", w);
	}
	return _Encode2(p, edx, w);
}

void _fastcall Encode4(OutPacket *p, LPVOID edx, DWORD dw){
	sprintf(&FormatS[strlen(FormatS)], " %08X", dw);
	return _Encode4(p, edx, dw);
}

void _fastcall Encode8(OutPacket *p, LPVOID edx, ULONGLONG ull){
	sprintf(&FormatS[strlen(FormatS)], " %08X%08X", (DWORD)(ull >> 32), (DWORD)ull);
	return _Encode8(p, edx, ull);
}

void _fastcall EncodeStr(OutPacket *p, LPVOID edx, char *s){
	sprintf(&FormatS[strlen(FormatS)], " \"%s\"", s);
	return _EncodeStr(p, edx, s);
}

void _fastcall EncodeBuffer(OutPacket *p, LPVOID edx, BYTE *b, int Length){
	sprintf(&FormatS[strlen(FormatS)], " ");
	for(int i=0; i<Length; i++){
		sprintf(&FormatS[strlen(FormatS)], "%02X", b[i]);
	}
	return _EncodeBuffer(p, edx, b, Length);
}

BYTE _fastcall Decode1(InPacket *p, LPVOID edx){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	BYTE b = _Decode1(p, edx);
	sprintf(&FormatR[strlen(FormatR)], " %02X", b);
	return b;
}

WORD _fastcall Decode2(InPacket *p, LPVOID edx){
	if(p->DecodedLength == 4){
		p->Return = 0;
	}
	else if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	WORD w =  _Decode2(p, edx);
	if(FormatR[0] == NULL){
		sprintf(&FormatR[strlen(FormatR)], "@%04X", w);
	}
	else{
		sprintf(&FormatR[strlen(FormatR)], " %04X", w);
	}
	return w;
}

DWORD _fastcall Decode4(InPacket *p, LPVOID edx){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	DWORD dw =  _Decode4(p, edx);
	sprintf(&FormatR[strlen(FormatR)], " %08X", dw);
	return dw;
}

ULONGLONG _fastcall Decode8(InPacket *p, LPVOID edx){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	ULONGLONG ull = _Decode8(p, edx);
	sprintf(&FormatR[strlen(FormatR)], " %08X%08X", (DWORD)(ull >> 32), (DWORD)ull);
	return ull;
}

char** _fastcall DecodeStr(InPacket *p, LPVOID edx, LPVOID lpv1){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	char **s = _DecodeStr(p, edx, lpv1);
	sprintf(&FormatR[strlen(FormatR)], " \"%s\"", *s);
	return s;
}

void _fastcall DecodeBuffer(InPacket *p, LPVOID edx, BYTE *b, int Length){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	_DecodeBuffer(p, edx, b, Length);
	sprintf(&FormatR[strlen(FormatR)], " ");
	for(int i=0; i<Length; i++){
		sprintf(&FormatR[strlen(FormatR)], "%02X", b[i]);
	}
}

//Aob v354.0
void Hook(){
	memory Maple;

	//Maple.scan("55 8B EC 6A FF 68  ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 85 ?? ?? ?? ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? E9");//v353.1
	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 4D ?? E9 -all");//v354.0

	Maple.Hook(Maple.get(2), SendPacket, &_SendPacket, 5);

	Maple.write(Maple.get(2) + 0x30, "52 E8 CA FF FF FF 90 C3");
	dwSendPacketReturn = Maple.get(2) + 0x30 + 0x06;

	//Maple.scan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3  00 00 00 00 8B D9 83 3D ?? ?? ?? ?? 00 0F 84 ?? ?? ?? ?? E9");//v353.1
	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 4D ?? E8 ?? ?? ?? ?? 0F B6 C0 85 C0 75 05 E9 ?? ?? ?? ?? E9 -all");//v354.0
	Maple.Hook(Maple.get(1), ProcessPacket, &_ProcessPacket, 5);
	
	
	Maple.scan("8B 44 24 04 6A 00 6A 00 50 E8 ?? ?? ?? ?? C2 04 00");
	Maple.Hook(Maple.get(), Init, &_Init, 6);

	Maple.scan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 41 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 56 08 8B 07 8A 4C 24 0C 88 0C 02 FF 46 08 5F 5E C2 04 00");
	Maple.Hook(Maple.get(), Encode1, &_Encode1, 6);

	Maple.scan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 83 C1 02 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 56 08 8B 07 66 8B 4C 24 0C 66 89 0C 02 83 46 08 02 5F 5E C2 04 00");
	Maple.Hook(Maple.get(), Encode2, &_Encode2, 6);

	Maple.scan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 83 C1 04 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 56 08 8B 07 8B 4C 24 0C 89 0C 02 83 46 08 04 5F 5E C2 04 00");
	Maple.Hook(Maple.get(), Encode4, &_Encode4, 6);

	Maple.scan("56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 83 C1 08 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 4C 24 0C 51 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 46 08 8B 0F 8B 54 24 0C 89 14 08 8B 54 24 10 89 54 08 04 83 46 08 08 5F 5E C2 08 00");
	Maple.Hook(Maple.get(), Encode8, &_Encode8, 6);

	Maple.scan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 51 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 8B 44 24 ?? C7 44 24 ?? 00 00 00 00 85 C0 74");
	Maple.Hook(Maple.get(), EncodeStr, &_EncodeStr, 7);

	Maple.scan("53 56 8B F1 8B 46 04 57 8D 7E 04 85 C0 74 03 8B 40 FC 8B 4E 08 8B 5C 24 14 03 CB 3B C8 76 1E 8B 07 85 C0 74 03 8B 40 FC 03 C0 3B C8 77 FA 8D 54 24 14 52 6A 00 50 8B CF E8 ?? ?? ?? ?? 8B 4E 08 8B 44 24 10 03 0F 53 50 51 E8 ?? ?? ?? ?? 01 5E 08 83 C4 0C 5F 5E 5B C2 08 00");
	Maple.Hook(Maple.get(), EncodeBuffer, &_EncodeBuffer, 7);

	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 51 ?? 8B 41 ?? 8B 71 ?? 2B C2 C7 45 ?? 00 00 00 00 83 F8 01");
	Maple.Hook(Maple.get(), Decode1, &_Decode1, 5);

	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 51 ?? 8B 41 ?? 8B 71 ?? 2B C2 C7 45 ?? 00 00 00 00 83 F8 02");
	Maple.Hook(Maple.get(), Decode2, &_Decode2, 5);

	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 51 ?? 8B 41 ?? 8B 71 ?? 2B C2 C7 45 ?? 00 00 00 00 83 F8 04");
	Maple.Hook(Maple.get(), Decode4, &_Decode4, 5);

	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 89 4D ?? 8B 71 ?? 8B 41 ?? 8B 51 ?? 2B C6 C7 45 ?? 00 00 00 00 83 F8 08");
	Maple.Hook(Maple.get(), Decode8, &_Decode8, 5);

	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 8B F1 89 75 ?? C7 45 ?? 00 00 00 00 8B 7D ?? B8 01 00 00 00");
	Maple.Hook(Maple.get(), DecodeStr, &_DecodeStr, 5);

	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 65 ?? 8B F1 89 75 ?? 8B 4E ?? 8B 46 ?? 8B 56 ?? 8B 7D ?? 2B C1 03 CA C7 45 ?? 00 00 00 00 3B C7");
	Maple.Hook(Maple.get(), DecodeBuffer, &_DecodeBuffer, 5);
	
}