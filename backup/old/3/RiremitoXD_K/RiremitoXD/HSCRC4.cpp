#include"AirBypass.h"

void HSCRC4(){
	HMODULE hModule = LoadLibraryA("HShield/EHSvc.dll");
	DWORD Address = (DWORD)hModule + 0x49E707;
	BYTE leetcode[10] = {0xB8, 0x01, 0x00, 0x00, 0x00, 0x83, 0xC4, 0x04 ,0x90,0x90};
	FullAccess(Address, 10);
	RtlCopyMemory((void *)Address, leetcode, 10);
}