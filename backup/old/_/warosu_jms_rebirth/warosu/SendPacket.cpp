#include"SendPacket.h"

namespace PS{
	DWORD SendFunction = 0x00486DB0;//v324.1
}

BOOL PS::SendPacket(char Packet_STR[]){
	int Packet_Size;
	BYTE Packet[4096];

	Packet_Size = Asm::ChartoByte(Packet_STR, Packet);

	if(Packet_Size == -1 || Packet_Size < 2){
		return FALSE;
	}

	//created by airride^^
	_asm{
		/*
		mov eax,fs:[0x18]
		mov ebx,0x7EFDD000
		mov ebx,[ebx+0x24]
		mov [eax+0x24],ebx
		mov [eax+0x6B8],ebx
		*/
		lea eax,[Packet]
		mov ebx,[Packet_Size]
		push 0x00
		push ebx
		push eax
		push 0x00
		push esp
		call SendFunction
	}

	return TRUE;
}
