#ifndef __PACKET_H__
#define __PACKET_H__

void SendPacketQueue(char *s);
void CALLBACK ScriptInjector(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void EnablePacketSender();

typedef	struct OutPacket{
	DWORD Return;
	BYTE *Packet;
	DWORD Length;
	LPVOID v2;
	LPVOID v3;
}OutPacket;

typedef	struct InPacket{
	DWORD Return;
	LPVOID v2;
	BYTE *Packet;
	DWORD Length;
	LPVOID v3;
	DWORD DataLength;
	DWORD DecodedLength;
}InPacket;

extern DWORD *_CClientSocket;
extern void (_fastcall *__SendPacket)(LPVOID ecx, OutPacket *p);

#endif