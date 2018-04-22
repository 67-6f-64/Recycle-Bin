#include<Windows.h>
#include<stdio.h>
#include"memory.h"
#include"Packet.h"

//SendPacket
COutPacket op;
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

DWORD *_CClientSocket;
void (_fastcall *__SendPacket)(LPVOID ecx, COutPacket *p);

void CALLBACK ScriptInjector(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
	if(op.Packet){
		__SendPacket((LPVOID)*_CClientSocket, &op);
		op.Packet = 0;
	}
}

void EnablePacketSender(){
	memory Maple;
	Maple.scan("Send", "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 ?? 64 A3 00 00 00 00 89 4D ?? E9 -all");//v354.0
	Maple.write(Maple.get(2) + 0x30, "52 E8 CA FF FF FF 90 C3");
	__SendPacket = (void (_fastcall *)(LPVOID, COutPacket*))(Maple.get(2) + 0x30);
	Maple.scan("Socket", "8B 0D ?? ?? ?? ?? 3B CB 74");
	_CClientSocket = (DWORD *)*(DWORD *)(Maple.get() + 0x02);
}