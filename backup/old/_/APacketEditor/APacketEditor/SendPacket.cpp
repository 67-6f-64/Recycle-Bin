#include"SendPacket.h"
#include"Address.h"
#include"AirASM.h"

char StrArray[128] = "0123456789ABCDEF";

int vChartoByte(char Array[], BYTE b[]){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x2A){Array[i] = StrArray[rand()%0x10];}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}

BOOL vSendPacket(char Packet_STR[]){
	int Packet_Size;
	BYTE Packet[4096];

	Packet_Size = vChartoByte(Packet_STR, Packet);

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
		add esp,0x04
		add esp,0x10
	}

	return TRUE;
}

bool Pa::SendPacketString(LPSTR lpString){
	BYTE vlpArray[4096];
	char vTemporary[4096*3];
	DWORD dwSize = 0;
	ZeroMemory(vlpArray, 4096);
	ZeroMemory(vTemporary, 4096*3);
	dwSize = StringCleaner(vTemporary, lpString);

	if(dwSize%2){
		return false;
	}

	dwSize = ChartoByte(vlpArray, vTemporary);


	_asm{
		lea eax,[vlpArray]
		mov ebx,[dwSize]
		push 0x00
		push ebx
		push eax
		push 0x00
		push esp
		call SendFunction
		add esp,0x04
		add esp,0x10
	}

	return true;
}
