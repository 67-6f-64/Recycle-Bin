#include<Windows.h>
#include<stdio.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include"resource.h"
#include"lib/Asm.h"
#include"SendPacket.h"

namespace PE{
	void PacketDialog(HINSTANCE hinstDLL);
	void _stdcall AddPacketList(char Type[], DWORD retAddr, int Size, BYTE Packet[]);
}

namespace PE{
	extern BOOL SHook;
	extern BOOL RHook;
}


#define MAX_PACKET 8192
#define MAX_StrPACKET MAX_PACKET*3

