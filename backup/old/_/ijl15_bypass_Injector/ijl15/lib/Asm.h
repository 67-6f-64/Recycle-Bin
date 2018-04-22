/*
=======================================================================

	AIRRIDE's C++ (Win32API) Asm Code Library v2.13 (2014/01/06)

=======================================================================
*/


#ifndef __Asm_H__
#define __Asm_H__

#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
#pragma comment(lib, "lib/asm.lib")

namespace Asm{
	#define Asm_Byte_Code_Max 0x100
	
	/*
	typedef struct{
		int s;
		BYTE b[Asm_Byte_Code_Max];
		BOOL m[Asm_Byte_Code_Max];
	}Aob;
	*/
}

namespace Asm{
	void Debug(BOOL Flag);
	DWORD Write_Hook(char code[], DWORD Prev, DWORD Next, int nop_count = 0);
	DWORD Write_Pointer_Hook(DWORD Pointer, DWORD HookAddr);
	BOOL Write_code(DWORD Addr, char Array[], int nop_count = 0);
	BOOL BytetoChar(unsigned char Packet[], int Size, char StrPacket[]);
	int ChartoByte(char Array[], BYTE b[]);
	DWORD AobScan(char Array[], char code[] = NULL, DWORD Memory_Start = 0x00401000, DWORD Memory_End = 0x02000000);
	DWORD ReadPointer(DWORD BaseAddr, DWORD Offset);
	DWORD ReadPointerAddress(DWORD BaseAddr, DWORD Offset);
}

#endif