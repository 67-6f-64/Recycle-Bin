#ifndef __GUI_H__
#define __GUI_H__

#include"AirWindow.h"
#include"Packet.h"

extern AirWindow AW;
#define EDIT_LOG 101

void gui(HINSTANCE hinstDLL);

#define PACKET_IGNORE 1
#define PACKET_BLOCK 2

DWORD gui_AddSendPacket(SPacket *p, DWORD Return);
DWORD gui_AddProcessPacket(RPacket *p);
DWORD gui_AddSendPacket2(SPacket *p, DWORD Return);

#endif