#include"Kyubey.h"

/*
	Class OutPacket
*/

OutPacket::OutPacket(WORD Header){
	Packet = (BYTE *)malloc(MAX_LENGTH);
	ZeroMemory(Packet, MAX_LENGTH);
	Length = 0;
	Encode2(Header);
};

OutPacket::~OutPacket(){
	free(Packet);
}

void OutPacket::Encode1(BYTE b){
	Packet[Length] = b;
	Length++;
}

void OutPacket::Encode2(WORD w){
	*(WORD *)&Packet[Length] = w;
	Length += 2;
}

void OutPacket::Encode4(DWORD dw){
	*(DWORD *)&Packet[Length] = dw;
	Length += 4;
}

void OutPacket::Encode8(ULONGLONG ull){
	*(ULONGLONG *)&Packet[Length] = ull;
	Length += 8;
}

void OutPacket::EncodeString(char *s){
	Encode2(strlen(s));
	memcpy(&Packet[Length], s, strlen(s) + 1);
	Length += strlen(s) + 1;
}

void OutPacket::EncodeBuffer(BYTE *b, DWORD dwLength){
	Encode2(dwLength);
	memcpy(&Packet[Length], b, dwLength);
	Length += dwLength;
}

LPBYTE OutPacket::getPacket(){
	return Packet;
}

DWORD OutPacket::getLength(){
	return Length;
}