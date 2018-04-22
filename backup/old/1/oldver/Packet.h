#ifndef __PACKET_H__
#define __PACKET_H__

#include<Windows.h>

class OutPacket{
private:
	BYTE *Packet;
	DWORD Length;
public:
	OutPacket(WORD Header);
	~OutPacket();
	void Encode1(BYTE b);
	void Encode2(WORD w);
	void Encode4(DWORD dw);
	void Encode8(ULONGLONG ull);
	void EncodeString(char *s);
	void EncodeBuffer(BYTE *b, DWORD dwLength);
	LPBYTE getPacket();
	DWORD getLength();
};

class InPacket{
private:
	BYTE *Packet;
	DWORD DecodedLength;
public:
	InPacket();
	~InPacket();
	BYTE Decode1();
	WORD Decode2();
	DWORD Decode4();
	ULONGLONG Decode8();
	void DecodeString(char *s);
	void DecodeBuffer(BYTE *b);
	LPBYTE getPacket();
	void setDecodedLength(DWORD dw);
};

#endif