#include"AirPL.h"

DWORD dwSendHook_Ret;

void _declspec(naked) SendHook(){
	_asm{
		pushad
		mov eax,[ebp+0x08]//SPacket
		push [ebp+0x04]//Return
		push [eax+0x08]//Size
		push [eax+0x04]//Packet
		call SendLog
		popad
		jmp dword ptr [dwSendHook_Ret]
	}
}

DWORD dwRecvPointer_Org;
DWORD dwRecvReturn;

void _declspec(naked) RecvHook(){
	_asm{
		mov eax,[dwRecvReturn]
		cmp dword ptr [esp],eax
		jne RH_Ending
		mov eax,esp
		pushad
		mov ebx,[eax+0x34]//RPacket
		push [eax+0x30]//Return
		push [ebx+0x0C]//Size
		push [ebx+0x08]//Packet
		call RecvLog
		popad
RH_Ending:
		jmp dword ptr [dwRecvPointer_Org]
	}
}

AirMemory AM;

void WriteHooks(){
	DWORD SendFunction, dwHookAddr, RecvPointer;
	AM.Init();
	SendFunction = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 ?? ?? ?? 64 A3 00 00 00 00 ?? ?? ?? 6A 00 E9");
	dwHookAddr = AM.AutoVMHook(SendFunction, SendHook, &dwSendHook_Ret);
	dwRecvReturn = AM.AobScan("8B 7C 24 2C 8B CF C7 44 24 24 00 00 00 00 E8");
	if(dwRecvReturn){
		RecvPointer = *(DWORD *)(dwRecvReturn - 4);
		AM.PointerHook(RecvPointer, RecvHook, &dwRecvPointer_Org);
	}

	if(!SendFunction){
		MessageBoxA(NULL, "ERROR:SendHook", "AirPL", NULL);
	}

	if(!dwRecvReturn){
		MessageBoxA(NULL, "ERROR:RecvHook", "AirPL", NULL);
	}

}