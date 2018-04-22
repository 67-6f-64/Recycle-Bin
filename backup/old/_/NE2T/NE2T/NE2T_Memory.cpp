#include"NE2T.h"

unsigned char Data[1024];

bool MemoryReader(DWORD Addr, DWORD Size){
	ZeroMemory(Data,sizeof(Data));
	if(Size > 1024) return false;
	if(!ReadProcessMemory(hProcess, (DWORD *)Addr, Data, Size, NULL))return false;
	return true;
}


DWORD byte4;
bool PointerReader(DWORD Pointer,DWORD Offset){
	DWORD Ptr;
	if(!ReadProcessMemory(hProcess, (DWORD *)Pointer, &Ptr, 4, NULL))return false;
	if(!ReadProcessMemory(hProcess, (DWORD *)(Ptr+Offset), &byte4, 4, NULL))return false;
	return true;
}