#include"Kyubey.h"

/*
	Class InPacket
*/

InPacket::InPacket(){
	Packet = (BYTE *)malloc(MAX_LENGTH);
	ZeroMemory(Packet, MAX_LENGTH);
	DecodedLength = 0;
}

InPacket::~InPacket(){
	free(Packet);
}

BYTE InPacket::Decode1(){
	BYTE b = Packet[DecodedLength++];
	return b;
}

WORD InPacket::Decode2(){
	WORD w = *(WORD *)&Packet[DecodedLength];
	DecodedLength += 2;
	return w;
}

DWORD InPacket::Decode4(){
	DWORD dw = *(DWORD *)&Packet[DecodedLength];
	DecodedLength += 4;
	return dw;
}

ULONGLONG InPacket::Decode8(){
	ULONGLONG ull = *(ULONGLONG *)&Packet[DecodedLength];
	DecodedLength += 8;
	return ull;
}

void InPacket::DecodeString(char *s){
	DWORD dwLength = Decode2();
	memcpy(s, &Packet[DecodedLength], dwLength);
	s[dwLength] = 0;
	DecodedLength += dwLength;
	Decode1();
}

void InPacket::DecodeBuffer(BYTE *b){
	DWORD dwLength = Decode2();
	memcpy(b, &Packet[DecodedLength], dwLength);
	DecodedLength += dwLength;
}

LPBYTE InPacket::getPacket(){
	return Packet;
}

void InPacket::setDecodedLength(DWORD dw){
	DecodedLength = dw;
}