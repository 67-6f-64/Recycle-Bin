#include<Windows.h>
#include"lib/Asm.h"
#include"guigui.h"


void _declspec(naked) ThreadCheck(){
	_asm{
		mov eax,0x7EFDD000
		mov eax,[eax+0x24]
		mov ecx,eax//using ecx
		ret
	}
}
HINSTANCE hDLL;
void hello(){
	Asm::Write_Hook("call", 0x01AA5F4F, (DWORD)ThreadCheck, 0);
	Asm::Write_Hook("call", 0x01ABD739, (DWORD)ThreadCheck, 0);
	Asm::Write_code(0x018D0F71, "B8 00 D0 FD 7E", 1);//6B8
	Asm::Write_code(0x01A1BAEE, "B8 00 D0 FD 7E", 1);//24
	Asm::Write_code(0x01B17033, "B8 00 D0 FD 7E", 1);//others
	PacketDialog(hDLL);
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;
	hDLL = hinstDLL;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, NULL, NULL, NULL);

	return TRUE;
}