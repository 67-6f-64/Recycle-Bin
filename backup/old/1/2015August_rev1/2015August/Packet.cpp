#include<Windows.h>
#include"Packet.h"

OutPacket::OutPacket(){
	p.Packet = new BYTE(256);
	p.Length = 0;
	p.bSentCheck = FALSE;
}

OutPacket::OutPacket(WORD Header){
	p.Packet = new BYTE(256);
	p.Length = 0;
	p.bSentCheck = FALSE;
	Encode2(Header);
}

void OutPacket::SetPacket(SPacket *sp){
	p.Length = sp->Length;
	RtlCopyMemory(p.Packet, sp->Packet, sp->Length);
}

OutPacket::~OutPacket(){
	delete[] p.Packet;
}

void OutPacket::SendPacket(){
	if(p.Length < 2) return;

	__SendPacket(*(DWORD *)CClientSocket, &p);
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

//in

InPacket::InPacket(RPacket *np){
	RtlCopyMemory(&rp, np, sizeof(RPacket));
}

InPacket::~InPacket(){
	//do nothing
}

WORD InPacket::DecodeHeader(){
	if(rp.BufferSize < 6){
		return BADPACKET;
	}
	Set(0);
	return Decode2();
}

BYTE InPacket::Decode1(){
	BYTE b = rp.Packet[rp.DecodedSize];
	rp.DecodedSize++;
	return b;
}

WORD InPacket::Decode2(){
	WORD w = *(WORD *)&rp.Packet[rp.DecodedSize];
	rp.DecodedSize += 2;
	return w;
}

DWORD InPacket::Decode4(){
	DWORD dw = *(DWORD *)&rp.Packet[rp.DecodedSize];
	rp.DecodedSize += 4;
	return dw;
}

void InPacket::DecodeString(char *s){
	WORD Size = Decode2();
	if(s){
		RtlCopyMemory(s, &rp.Packet[rp.DecodedSize], Size);
		s[Size] = 0;
	}
	rp.DecodedSize += Size;
}

void InPacket::Reject(){
	*(WORD *)&rp.Packet[4] = BADPACKET;
}

void InPacket::Next(int Size){
	rp.DecodedSize += Size;
}

void InPacket::ReadData(char *s, int Size){
	ZeroMemory(s, Size);
	RtlCopyMemory(s, &rp.Packet[rp.DecodedSize], Size);
}

void InPacket::Set(int Position){
	rp.DecodedSize = 4 + Position;
}
