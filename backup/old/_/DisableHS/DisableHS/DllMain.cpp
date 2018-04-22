#include<Windows.h>
#include"ASM.h"
#include"MultiMS.h"
#include"MSCRCbypass.h"

//10 minutes d/c
void Errorbypass(){
	//Internal Detection
	Asm::Write_code(0x00E3BCA0, "C3");

	//VEH Debug Detection
	Asm::Write_code(0x0051D4F0, "31 C0 C3");

	//HackShield Loader
	Asm::Write_code(0x00E5A7BB, "31 C0 40 C3");

	//HackShield Initialize Error 0x00000002
	Asm::Write_code(0x00E3C600, "C3");

	//MultiClient Detection 0x00000206, HackShield Initialize Error 0x00000003
	Asm::Write_code(0x00E3BA90, "C2 04 00");

	//Error 0x00000301, Hacking Threat Detect 0x00000003, This function calls HeartBeat
	Asm::Write_code(0x00E3BF30, "C2 04 00");

	//Kill Dialog
	Asm::Write_call(0x00DAE83E, (DWORD)ExitProcess);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	if (fdwReason == DLL_PROCESS_ATTACH){
		CRC::CRCbypass();
		Errorbypass();
		//Moby::MultiClient();
	}
	return TRUE;
}