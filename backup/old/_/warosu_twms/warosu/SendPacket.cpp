#include"SendPacket.h"

namespace PS{
	DWORD SendFunction = 0x00478020;//v101.1
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
