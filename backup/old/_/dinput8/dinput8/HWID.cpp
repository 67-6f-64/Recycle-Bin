#include"HWID.h"
#include"Function.h"

DWORD dwSendHook = 0x0180D596;
DWORD dwSendHook_Return = dwSendHook + 0x05;
DWORD dwHWIDPacket1_Return = 0x01A3255A;
DWORD dwHWIDPacket2_Return = 0x0057DA71;

BYTE HWID[16] = {0};

void CreateHWID(BYTE Packet[]){
	WORD MapleID_Length = *(WORD *)(&Packet[25]);
	char *MapleID = (char *)&Packet[27];
	DWORD i, j = 0, UniqueCode = 0;

	//org HWID
	RtlCopyMemory(&HWID[0], &Packet[2], 16);

	for(i=0; i<MapleID_Length; i++){
		UniqueCode += MapleID[i];
	}

	for(i=0; i<10; i++){
		HWID[6+i] ^= UniqueCode;
		HWID[6+i] ^= MapleID[i%MapleID_Length];
	}

}

void _stdcall PacketHook(DWORD dwReturn, BYTE Packet[]){
	if(dwReturn == dwHWIDPacket1_Return){
		CreateHWID(Packet);
		RtlCopyMemory(&Packet[2], &HWID[0], 16);
	}
	else if(dwReturn == dwHWIDPacket2_Return){
		RtlCopyMemory(&Packet[10], &HWID[0], 16);
	}
	return;
}


void _declspec(naked) SendHook(){
	_asm{
		pushad
		mov eax,[ebp+0x08]
		push [eax+0x04]
		push [ebp+0x04]//Return
		call PacketHook
		popad
		add eax,0x00001000//org code
		jmp dword ptr [dwSendHook_Return]
	}
}

void HWIDBypass(){
	WriteJMP(dwSendHook, (DWORD)SendHook);
}