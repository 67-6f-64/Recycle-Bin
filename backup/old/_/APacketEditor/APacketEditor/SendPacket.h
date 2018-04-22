#ifndef __SENDPACKET_H__
#define __SENDPACKET_H__

#include<Windows.h>

BOOL vSendPacket(char Packet_STR[]);

namespace Pa{
	bool SendPacketString(LPSTR lpString);
}

#endif