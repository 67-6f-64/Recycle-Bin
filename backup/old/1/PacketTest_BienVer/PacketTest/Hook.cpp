#include<Windows.h>
#include"Hook.h"
#include"memory.h"
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)
#include"Packet.h"
#include"gui_pe.h"
#include<stdio.h>
#include"apihook.h"

//Send
DWORD *_CClientSocket;
void (_fastcall *__SendPacket)(LPVOID ecx, OutPacket *p);

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

//SendPacket
OutPacket op;
BYTE Packet[1024];

void SendPacketQueue(char *s){
	op.Packet = NULL;
	op.Length = 0;
	WORD Header;
	int cs = 0;
	for(int i=0; s[i]; i++){
		if(s[i] == 0x40){
			cs = i + 1;
			break;
		}
	}
	if(!cs){
		return;
	}
	if(sscanf(&s[cs], "%04X", &Header) == 1){
		*(WORD *)&Packet[0] = Header;
		op.Length = 2;
	}
	else{
		op.Packet = NULL;
		return;
	}

	for(int i=cs; s[i]; i++){
		if(s[i] == 0x20){
			cs = i + 1;
			if(*(WORD *)&s[cs] == 0x2A2A){
				Packet[op.Length++] = (BYTE)rand()%0x100;
			}
			else if(sscanf(&s[cs], "%02X", &Packet[op.Length]) == 1){
				op.Length++;
			}
		}
	}

	op.Packet = Packet;
}

void CALLBACK ScriptInjector(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){

	if(op.Packet){
		__SendPacket((LPVOID)*_CClientSocket, &op);
		op.Packet = 0;
	}
}
/*
HWND (WINAPI *_CreateWindowExA)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam){
	HWND hWnd = _CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	if(lpClassName && strstr(lpClassName, "MapleStoryClass")){
		op.Packet = NULL;
		op.Length = 0;
		SetTimer(hWnd, 1337, 50, ScriptInjector);
	}
	return hWnd;
}


void SetScriptInjector(){
	apihook user32("user32.dll");
	user32.Hook("CreateWindowExA", CreateWindowExA_Hook, &_CreateWindowExA);
}
*/
//End

DWORD dwSendPacketReturn;
void _fastcall SendPacket(DWORD CClientSocket, LPVOID edx, OutPacket *p){
	DWORD Return = (DWORD)_ReturnAddress();

	if(Return != dwSendPacketReturn){
		if(p->Length < 4096) AddLog(0, Return, p->Length, p->Packet, FormatS);
		else AddLog(0, Return, 2, p->Packet, "エラー");
	}

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
	FormatR[0] = 0;//初期化
	_ProcessPacket(ecx, edx, p);
	if(p->DataLength < 4096) AddLog(1, p->Return, p->DataLength, p->Packet, FormatR);
	else AddLog(1, p->Return, 2, p->Packet, "エラー");
}

void _fastcall Init(OutPacket *p, LPVOID edx, WORD w){
	FormatS[0] = 0;//初期化
	return _Init(p, edx, w);
}

void _fastcall Encode1(OutPacket *p, LPVOID edx, BYTE b){
	if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], " %02X", b);
	return _Encode1(p, edx, b);
}

void _fastcall Encode2(OutPacket *p, LPVOID edx, WORD w){
	if(FormatS[0] == NULL){
		if(p->Length < 4096) sprintf(&FormatS[0], "@%04X", w);
	}
	else{
		if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], " %04X", w);
	}
	return _Encode2(p, edx, w);
}

void _fastcall Encode4(OutPacket *p, LPVOID edx, DWORD dw){
	if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], " %08X", dw);
	return _Encode4(p, edx, dw);
}

void _fastcall Encode8(OutPacket *p, LPVOID edx, ULONGLONG ull){
	if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], " %08X%08X", (DWORD)(ull >> 32), (DWORD)ull);
	return _Encode8(p, edx, ull);
}

void _fastcall EncodeStr(OutPacket *p, LPVOID edx, char *s){
	if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], " \"%s\"", s);
	return _EncodeStr(p, edx, s);
}

void _fastcall EncodeBuffer(OutPacket *p, LPVOID edx, BYTE *b, int Length){
	if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], " ");
	for(int i=0; i<Length; i++){
		if(p->Length < 4096) sprintf(&FormatS[strlen(FormatS)], "%02X", b[i]);
	}
	return _EncodeBuffer(p, edx, b, Length);
}

BYTE _fastcall Decode1(InPacket *p, LPVOID edx){

	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	BYTE b = _Decode1(p, edx);
	if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], " %02X", b);
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
		if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], "@%04X", w);
	}
	else{
		if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], " %04X", w);
	}
	return w;
}

DWORD _fastcall Decode4(InPacket *p, LPVOID edx){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	DWORD dw =  _Decode4(p, edx);
	if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], " %08X", dw);
	return dw;
}

ULONGLONG _fastcall Decode8(InPacket *p, LPVOID edx){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	ULONGLONG ull = _Decode8(p, edx);
	if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], " %08X%08X", (DWORD)(ull >> 32), (DWORD)ull);
	return ull;
}

char** _fastcall DecodeStr(InPacket *p, LPVOID edx, LPVOID lpv1){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	char **s = _DecodeStr(p, edx, lpv1);
	if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], " \"%s\"", *s);
	return s;
}

void _fastcall DecodeBuffer(InPacket *p, LPVOID edx, BYTE *b, int Length){
	if(p->DecodedLength == 6){
		p->Return = (DWORD)_ReturnAddress();
	}
	_DecodeBuffer(p, edx, b, Length);
	if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], " ");
	for(int i=0; i<Length; i++){
		if(p->DataLength < 4096) sprintf(&FormatR[strlen(FormatR)], "%02X", b[i]);
	}
}

typedef struct Frame{
	LPVOID lpv1;
	LPVOID lpv2;
	HWND hWnd;
	LPVOID lpv4;
	LPVOID lpv5;
	LPVOID lpv6;
	LPVOID lpv7;
	DWORD time;
};

//Aob v354.0
void Hook(){
	memory Maple;

	//Maple.scan("55 8B EC 6A FF 68  ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 85 ?? ?? ?? ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? E9");//v353.1
	Maple.scan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 4D ?? E9 -all");//v354.0

	Maple.Hook(Maple.get(2), SendPacket, &_SendPacket, 5);

	Maple.write(Maple.get(2) + 0x30, "52 E8 CA FF FF FF 90 C3");
	dwSendPacketReturn = Maple.get(2) + 0x30 + 0x06;

	__SendPacket = (void (_fastcall *)(LPVOID, OutPacket*))(Maple.get(2) + 0x30);

	Maple.scan("8B 0D ?? ?? ?? ?? 3B CB 74");
	_CClientSocket = (DWORD *)*(DWORD *)(Maple.get() + 0x02);

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
	
	Maple.scan("A1 ?? ?? ?? ?? 80 B8 ?? ?? ?? ?? 00 56 8B F1 0F 84");
	Frame *Client = (Frame *)(*(DWORD *)(*(DWORD *)(Maple.get() + 0x01)));

	while(!Client->hWnd){
		Sleep(50);
	}
	
	SetTimer(Client->hWnd, 1337, 50, ScriptInjector);
}