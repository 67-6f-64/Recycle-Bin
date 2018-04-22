#include<Windows.h>
#include"AirCRC.h"
#include<detours.h>

DWORD Memory, Memory_Start, Memory_End;


void WRYYY(){
	Air::CreateMemoryDump(&Memory, &Memory_Start, &Memory_End);
	Air::BypassCRC(0x0068A852, Memory, Memory_Start, Memory_End);
	Air::BypassCRC(0x005CEE48, Memory, Memory_Start, Memory_End);
	Air::BypassCRC(0x005D4B39, Memory, Memory_Start, Memory_End);
	Air::BypassCRC(0x008EB32D, Memory, Memory_Start, Memory_End);
	Air::BypassCRC(0x0150FB09, Memory, Memory_Start, Memory_End);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	WRYYY();

	return TRUE;
}