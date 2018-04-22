#ifndef __AIRMEMORY_H__
#define __AIRMEMORY_H__

#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

#define ByteArrayMaxSize 0x100

namespace Air{
	bool FullAccess(DWORD dwAddress, DWORD dwSize);
	bool GetAddresses(LPSTR lpModuleName, DWORD *Memory_Start, DWORD *Memory_End);
	void MemoryDump(DWORD *Memory, DWORD Memory_Start, DWORD Memory_End);
	bool CreateMemoryDump(DWORD *Memory, DWORD *Memory_Start, DWORD *Memory_End, LPSTR lpModuleName = NULL);
	int StringtoByteArray(BYTE *bCode, char *Code);
	bool WriteCode(DWORD dwAddress, char *Code);
	bool WriteCodeAtAPI(LPSTR lpModuleName, LPSTR lpProcName, char *Code);
	bool WriteCodeAtModule(LPSTR lpModuleName, DWORD dwPosition, char *Code);
	DWORD GetAPIAddress(LPSTR lpModuleName, LPSTR lpProcName);
	int CreateAob(BYTE *bCode, bool *bMask, char *Aob);
	DWORD AobScan(char *Aob, int iResult = 0, LPSTR lpModuleName = NULL);
	void WriteJump(DWORD dwPrev, DWORD dwNext, DWORD *dwRet = NULL, int iNop = 0);
	void WriteCall(DWORD dwPrev, DWORD dwNext, DWORD *dwRet = NULL);
	void WriteJumpAtModule(LPSTR lpModuleName, DWORD dwPosition, DWORD dwNext, DWORD *dwRet = NULL, int iNop = 0);
}

#endif