#ifndef __KYUBEY_H__
#define __KYUBEY_H__

#include<winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_LENGTH 0x100

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

class Kyubey{
private:
	SOCKET Server, Client;
	sockaddr_in addr;
	sockaddr_in addr_client;
public:
	Kyubey(WORD Port, void (*thread)(Kyubey*));
	Kyubey(char *Address, WORD Port);
	Kyubey(SOCKET NewClient);
	~Kyubey();
	int ProcessPacket(InPacket *p);
	void SendPacket(OutPacket *p);
	void Reconnect();
};

#endif