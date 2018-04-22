#ifndef __AIR_ASM_H__
#define __AIR_ASM_H__

#include<Windows.h>

BYTE* MemoryDump(char *ModuleName, DWORD *Memory_Start, DWORD *Memory_End);
DWORD AobScan(char *String, DWORD result = 0, char *ModuleName = NULL);
DWORD WriteHook(DWORD Prev, char code[], DWORD Next, int nop_count = 0);
BOOL WriteCode(DWORD Addr, char Array[], int nop_count = 0);

#endif