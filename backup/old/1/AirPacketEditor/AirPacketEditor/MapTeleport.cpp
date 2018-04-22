#include<Windows.h>
#include"MapTeleport.h"
#include"Packet.h"
#include"AirMemory.h"
#include"Hook.h"

WORD Header_EnterStarPlanet;
WORD Header_HeartBeat;
bool bTeleporting = false;

void MapTeleport(DWORD MapID){
	OutPacket p(Header_EnterStarPlanet);
	p.Encode1(0x02);
	p.Encode4(MapID);
	p.Encode2(0);
	p.Encode4(1);
	p.Encode4(0);
	p.SendPacket();
}

void DelayDisconnect(){
	Sleep(50);
	OutPacket p(Header_HeartBeat);//v339(Heart Beat)
	p.SendPacket();
}

void (_fastcall *_InitHeartBeat)(LPVOID ecx);
void _fastcall InitHeartBeatAndDisconnect(LPVOID ecx){
	_InitHeartBeat(ecx);
	if(bTeleporting){
		bTeleporting = false;
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DelayDisconnect, NULL ,NULL, NULL);
		CloseHandle(hThread);
	}
}
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)
#include<stdio.h>

void AobErrorMH(){
	char asdf[256];
	sprintf(asdf, "Aob(MH) is broken(%p)", _ReturnAddress());
	MessageBoxA(NULL, asdf, "Error", NULL);
}

void Hook_MT(){
	AirMemory AM;
	DWORD Address;

	AM.Init();
	Address = AM.AobScan("68 ?? ?? ?? ?? 8D 4C 24 ?? E8 ?? ?? ?? ?? 8B 44 24 ?? 50 8D 4C 24 ?? C7 44 24 ?? 00 00 00 00 E8 ?? ?? ?? ?? 8B 4C 24 ?? 51 8D 4C 24 ?? E8 ?? ?? ?? ?? E8");
	if(!Address) AobErrorMH();

	Header_EnterStarPlanet = *(DWORD *)(Address + 1);

	Address = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 6A ?? 8D 4C 24 ?? C6 86 ?? ?? ?? ?? 01 E8");
	if(!Address) AobErrorMH();
	set_hook(Address, 7, InitHeartBeatAndDisconnect, &_InitHeartBeat);
	Header_HeartBeat = *(BYTE *)(Address + 0x27);
}