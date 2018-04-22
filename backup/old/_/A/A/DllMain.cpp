#include<Windows.h>
#include"lib/Asm.h"
#include"lib/Airgui.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	//ERROR
	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	MessageBoxA(NULL, "HackShield is dead!\nThank you!", "FuckShield beta", NULL);
	
	//TestHook_ret = Asm::Write_Hook("ja", 0x00E890EF, (DWORD)TestHook, 2);
	//Asm::Write_code(0x00E890EF, "bbaa cc Ad fa ba AA 12 44 5F 5f", 512);
	Airgui::Test(hinstDLL);
	return TRUE;
}