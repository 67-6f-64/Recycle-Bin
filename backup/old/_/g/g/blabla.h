#ifndef __AIRASM_H__
#define __AIRASM_H__

#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

BOOL FullAccess(DWORD dwMemory, DWORD dwSize);
DWORD Alloc(DWORD dwSize);
void Write1(LPDWORD dwMemory, BYTE bCode, bool bNext = true);
void Write2(LPDWORD dwMemory, WORD wCode, bool bNext = true);
void Write4(LPDWORD dwMemory, DWORD dwCode, bool bNext = true);
int WriteCode(LPDWORD dwMemory, LPCSTR lpString, bool bNext = true);

#endif