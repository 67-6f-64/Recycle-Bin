#ifndef __AIRCRC_H__
#define __AIRCRC_H__

#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

namespace Air{
	bool CreateMemoryDump(DWORD *Memory, DWORD *Memory_Start, DWORD *Memory_End, LPSTR lpModuleName = NULL);
	void BypassCRC(DWORD dwAddress, DWORD Memory, DWORD Memory_Start, DWORD Memory_End);
}

#endif