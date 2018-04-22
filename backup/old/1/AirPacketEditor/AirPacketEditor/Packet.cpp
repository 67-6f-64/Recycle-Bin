#include<Windows.h>
#include"Packet.h"

OutPacket::OutPacket(WORD Header){
	p.Packet = new BYTE(256);
	p.Length = 0;
	p.lpv1 = 0;
	p.lpv2 = 0;
	p.bSentCheck = FALSE;
	Encode2(Header);
}

char HexArray[] ="0123456789ABCDEF0123456789abcdef";
int StringtoByteArray(BYTE *bCode, char *Code){
	int iSize = 0, i, j;

	for(i=0; Code[i]; i++){
		if(Code[i] == '*'){
			Code[i] = HexArray[rand()%0x10];
		}
		for(j=0; HexArray[j]; j++){
			if(Code[i] == HexArray[j]){
				if(iSize%2 == 0){
					bCode[iSize/2] = (j%0x10)*0x10;
				}
				else{
					bCode[iSize/2] += j%0x10;
				}
				iSize++;
				break;
			}
		}
	}

	if(iSize%2){
		return 0;
	}

	return (iSize/2);
}

OutPacket::OutPacket(char *s){
	p.Packet = new BYTE(256);
	p.Length = 0;
	p.bSentCheck = FALSE;

	if(!s) return;

	p.Length = StringtoByteArray(p.Packet, s);
}

OutPacket::~OutPacket(){
	delete[] p.Packet;
}

void OutPacket::SendPacket(){
	if(p.Length < 2) return;
	_fSendPacket(*(DWORD *)CClientSocket, &p);
}


void OutPacket::Encode1(BYTE b){
	p.Packet[p.Length] = b;
	p.Length++;
}

void OutPacket::Encode2(WORD w){
	*(WORD *)&p.Packet[p.Length] = w;
	p.Length += 2;
}
void OutPacket::Encode4(DWORD dw){
	*(DWORD *)&p.Packet[p.Length] = dw;
	p.Length += 4;
}

void OutPacket::Encode8(unsigned long long int ull){
	*(unsigned long long *)&p.Packet[p.Length] = ull;
	p.Length += 8;
}

void OutPacket::EncodeStr(char *s){
	int s_length = strlen(s);
	Encode2(s_length);
	for(int i=0; i<s_length; i++){
		*(char *)p.Packet[p.Length + i] = s[i];
	}
	p.Length += s_length;
}

void OutPacket::EncodeBuffer(BYTE *b, int len){
	for(int i=0; i<len; i++){
		p.Packet[p.Length + i] = b[i];
	}
	p.Length += len;
}