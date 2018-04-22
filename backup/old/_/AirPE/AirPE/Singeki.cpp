#include"Singeki.h"

DWORD dwSendFunction = 0x0048F600;

void SendPacket2(BYTE vPacket[], DWORD dwSize){
	_asm{
		pushad
		push 0x00
		push [dwSize]
		push vPacket
		push 0x00
		push esp
		call dword ptr [dwSendFunction]
		add esp,0x04
		add esp,0x10
		popad
	}
}

BYTE Packet_Singeki_Start[7] = {0x56, 0x02, 0x00, 0x00, 0x00, 0x00};
BYTE Packet_Singeki_Clear[16] = {0xDA, 0x00, 0x00, 0x06, 0x00, 0x6E, 0x65, 0x78, 0x74, 0x30, 0x30, 0x27, 0x0D, 0xA2, 0x00};

void Singeki(){
	SendPacket2(Packet_Singeki_Start, 6);
	Packet_Singeki_Clear[2] += 2;
	//if(Packet_Singeki_Clear[2] == 256){
	//	Packet_Singeki_Clear[2] = 0;
	//}
	SendPacket2(Packet_Singeki_Clear, 15);
}

BYTE Packet_CC[7] = {0x47, 0x00, 0x0A, 0xD8, 0x85, 0x29, 0x00};

void CCPLZ(){
	Packet_CC[2] = rand()%0x0D;
	Packet_Singeki_Clear[2] = 0;
	SendPacket2(Packet_CC, 7);
}