#ifndef __PACKET_H__
#define __PACKET_H__


typedef struct RFormat{
	char *String;
	DWORD lpv2;
	DWORD FunctionAddress;
	DWORD DecodeCount;
};

typedef struct SPInfo{
	char *s;
	DWORD s_Length;
	DWORD EncodedLength;
};

typedef	struct SPacket{
	DWORD lpv1;
	BYTE *Packet;
	DWORD Length;
	DWORD lpv2;
	BOOL bSentCheck;
};

class OutPacket{
private:
	SPacket p;
public:
	OutPacket(WORD Header);
	OutPacket(char *s);
	~OutPacket();
	void Encode1(BYTE b);
	void Encode2(WORD w);
	void Encode4(DWORD dw);
	void Encode8(unsigned long long int ull);
	void EncodeStr(char *s);
	void EncodeBuffer(BYTE *b, int len);
	void SendPacket();
};

typedef	struct RPacket{
	char *lpv1;
	DWORD Always2;
	BYTE *Packet;
	DWORD Length;
	DWORD lpv2;
	DWORD DataLength;
	DWORD DecodedLength;
};

class InPacket{
private:
	RPacket p;
public:
};

extern RFormat *RF;
extern SPInfo *spi;

extern void (__fastcall *_fSendPacket)(DWORD ecx, SPacket *p);
extern DWORD CClientSocket;

#endif