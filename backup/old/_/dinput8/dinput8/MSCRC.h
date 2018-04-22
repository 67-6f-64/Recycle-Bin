#ifndef __MSCRC_H__
#define __MSCRC_H__

#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

extern BYTE *MS_Memory;
extern DWORD dwMS_Memory_Start;
extern DWORD dwMS_Memory_End;

void MSCRCBypass();

#endif
