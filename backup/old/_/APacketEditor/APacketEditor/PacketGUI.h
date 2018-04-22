#ifndef __PACKETGUI_H__
#define __PACKETGUI_H__

#include"Air.h"
#pragma comment(lib, "Airlib2.lib")

void APacketEditor(HINSTANCE hInstance);
bool _stdcall AddPacket(bool bType, DWORD dwReturn, DWORD dwSize, BYTE Packet[]);

#endif