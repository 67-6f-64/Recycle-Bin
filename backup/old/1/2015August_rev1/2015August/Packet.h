#ifndef __PACKET_H__
#define __PACKET_H__

typedef	struct SPacket{
	DWORD lpv1;
	BYTE *Packet;
	DWORD Length;
	DWORD lpv2;
	BOOL bSentCheck;
};

class OutPacket{
//private:
public:
	SPacket p;
	int iDelay;
//public:
	OutPacket();
	OutPacket(WORD Header);
	~OutPacket();

	void SetPacket(SPacket *sp);
	void Encode1(BYTE b);
	void Encode2(WORD w);
	void Encode4(DWORD dw);
	void Encode8(unsigned long long int ull);
	void EncodeStr(char *s);
	void EncodeBuffer(BYTE *b, int len);
	void SendPacket();
};

typedef struct{
	LPVOID lpv1;
	LPVOID lpv2;
	BYTE *Packet;
	DWORD BufferSize;//Buffer Size
	LPVOID lpv4;
	DWORD Size;//Length
	DWORD DecodedSize;//Read Size
}RPacket;

#define BADPACKET 0xFFFF


class InPacket{
private:
	RPacket rp;
public:
	InPacket(RPacket *np);
	~InPacket();
	WORD DecodeHeader();
	BYTE Decode1();
	WORD Decode2();
	DWORD Decode4();
	void DecodeString(char *s);
	void Reject();
	void Next(int Size);
	void ReadData(char *s, int Size);
	void Set(int Position);
	//BYTE* DecodeBuffer;
};

extern void (__fastcall *__SendPacket)(DWORD ecx, SPacket *p);
extern DWORD CClientSocket;

#endif