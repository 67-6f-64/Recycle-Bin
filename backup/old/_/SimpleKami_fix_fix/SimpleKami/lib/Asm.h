/*
=======================================================================

	AIRRIDE's C++ (Win32API) Asm Code Library v2.00 (2013/12/11)

=======================================================================
*/


#ifndef __Asm_H__
#define __Asm_H__

#include<Windows.h>
#pragma comment(lib, "lib/asm.lib")

namespace Asm{
	DWORD Write_Hook(char code[], DWORD Prev, DWORD Next, int nop_count = 0);
	DWORD Write_Pointer_Hook(DWORD Pointer, DWORD HookAddr);
	BOOL Write_code(DWORD Addr, char Array[], int nop_count = 0);
	BOOL BytetoChar(unsigned char Packet[], int Size, char StrPacket[]);
	int ChartoByte(char Array[], unsigned char b[]);
}

#endif