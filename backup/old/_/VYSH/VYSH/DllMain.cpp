#include<Windows.h>
#include"MemoryHacks.h"

DWORD HeartBeat;

void _declspec(naked) ServiceDispatch(){
	_asm{
		cmp dword ptr [esp+0x04],0x0D
		jne Ending
		mov eax,[esp+0x08]
		pushad
		push [eax+0x08]
		push [eax+0x04]
		push [eax]
		call HeartBeat
		popad
		//add esp,0x0C
Ending:
		mov eax,1
		ret 0x000C
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	DWORD EHSvc = (DWORD)LoadLibraryA("HShield/_EHSvc.dll");
	*(DWORD *)(EHSvc+0x14E7EC) = 0x101;
	*(DWORD *)(EHSvc+0x154EB8) = 0x04;
	//HeartBeat = EHSvc+0x9045F;
	HeartBeat = (DWORD)GetProcAddress((HMODULE)EHSvc, "16");

	MSCRCBypass();
	RemoveThemidaCRCs();
	VEHBypass();
	StopProcessListPacket();
	InternalDetection();

	return TRUE;
}