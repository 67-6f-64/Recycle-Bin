#ifndef __PACKET_H__
#define __PACKET_H__

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

#endif