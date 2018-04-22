#include"hook.h"
#include"gui.h"
#include"packet.h"
#include"AutoLogin.h"

DWORD Action = 0;

void FullAccess(DWORD Address, DWORD Size){
	DWORD old;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &old);
}

void write_jmp(DWORD Prev, DWORD Next){
	*(BYTE *)(Prev) = 0xE9;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}

void write_call(DWORD Prev, DWORD Next){
	*(BYTE *)(Prev) = 0xE8;
	*(DWORD *)(Prev + 1) = Next - Prev - 5;
}

void set_hook(DWORD Function, DWORD Size, LPVOID HookFunction, LPVOID NewFunction){
	DWORD OverWritten = (DWORD)new BYTE[Size + 5];
	FullAccess(OverWritten, Size + 5);//ŽÀs‰Â”\‚É•ÏX
	RtlCopyMemory((LPVOID)OverWritten, (LPVOID)Function, Size);//Á‚¦‚é•”•ª‚ð•œŒ³

	if(*(BYTE *)Function == 0xE9){
		DWORD Address = *(DWORD *)(Function + 1) + Function + 5;
		*(DWORD *)(OverWritten + 1) = Address - OverWritten - 5;
	}

	write_jmp(OverWritten + Size, Function + Size);
	*(DWORD *)NewFunction = (DWORD)OverWritten;
	FullAccess(Function, 5);
	write_jmp(Function, (DWORD)HookFunction);
	if(Size > 5){
		for(int i=0; i<Size-5; i++){
			*(BYTE *)(Function + 5 + i) = 0x90;
		}
	}
}

WORD Header_HeartBeat;
void DelayDisconnect(){
	Sleep(50);
	OutPacket sp(Header_HeartBeat);//v339(Heart Beat)
	sp.SendPacket();
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

WORD Header_EnterStarPlanet;
void MapTeleport(DWORD MapID){
	OutPacket sp(Header_EnterStarPlanet);
	sp.Encode1(0x02);
	sp.Encode4(MapID);
	sp.Encode2(0);
	sp.Encode4(1);
	sp.Encode4(0);
	sp.SendPacket();
}

//
DWORD Ptr_ThreadID = 0x01D8FF4C;
DWORD Addr_SendPacket;
DWORD Addr_FakeRet;
//
void (__fastcall *__SendPacket)(DWORD ecx, SPacket *p);
DWORD (__fastcall *_GetData4)(DWORD ecx);

DWORD __fastcall GetData4_Hook(DWORD ecx){
	if(ecx == Ptr_ThreadID){
		return 0;
	}
	return _GetData4(ecx);
}

void (__fastcall *_SendPacket)(DWORD ecx, DWORD edx, SPacket *p);
void _declspec(naked) __fastcall FakeSendPacket(DWORD ecx, DWORD edx, SPacket *p){
	_asm{
		push PUSHBACK
		push p
		push [Addr_FakeRet]//fake return
		jmp dword ptr [_SendPacket]
PUSHBACK:
		ret 0x0004
	}
}

void __fastcall SendPacket_Hook(DWORD ecx, DWORD edx, SPacket *p){
	UpdatePacketAL(p);
	return FakeSendPacket(ecx, edx, p);
}

void (__fastcall *_ProcessPacket)(DWORD ecx, DWORD edx, RPacket *p);
void __fastcall ProcessPacket_Hook(DWORD ecx, DWORD edx, RPacket *p){
	_ProcessPacket(ecx, edx, p);
	//ReplyPacketAL(p);//must write this here
}


void AobError(){
	MessageBoxA(NULL, "Aob is broken1", "Error", NULL);
}

DWORD ProxyScan(DWORD Address){
	DWORD i;
	for(i=0x00401000; i<0x02000000; i++){
		if(*(BYTE *)i == 0xE9){
			if(*(DWORD *)(i + 1) + i + 5 == Address){
				return i;
			}
		}
	}
	return 0;
}

void write_byte(DWORD Address, BYTE b){
	FullAccess(Address, 1);
	*(BYTE *)Address = b;
}

DWORD CClientSocket;

void BypassSD(){
	AirMemory AM;
	AM.Init();

	DWORD Address, ReturnVAL;
	Address = AM.AobScan("E9 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 85 ?? ?? ?? ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 6A 00 E9");
	if(!Address){
		Address = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 85 ?? ?? ?? ?? 53 56 57 50 8D 45 ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 6A 00 E9");
	}
	if(!Address) AobError();
	set_hook(Address, 5, SendPacket_Hook, &_SendPacket);
	Addr_SendPacket = Address;

	Address = AM.AobScan("83 EC 08 53 56 57 8B F9 8B 57 ?? 8B 02 89 44 24 ?? 8A 42 ?? 84 C0 75", 2);
	if(!Address) AobError();
	_GetData4 = (DWORD (__fastcall *)(DWORD))Address;
	Address = ProxyScan(Address);
	if(!Address) AobError();
	write_jmp(Address, (DWORD)GetData4_Hook);

	Address = AM.AobScan("52 E8 ?? ?? ?? ?? 90 C3", NULL, 0x01800000);
	if(!Address){
		Address = AM.AobScan("CC CC CC CC CC CC CC CC", NULL, 0x01800000);
		if(!Address) AobError();
		write_byte(Address, 0x52);//push edx
		write_call(Address + 1, Addr_SendPacket);
		write_byte(Address + 1 + 5, 0x90);//nop
		write_byte(Address + 1 + 5 + 1, 0xC3);//ret
	}
	__SendPacket = (void (__fastcall *)(DWORD, SPacket*))Address;
	Addr_FakeRet = Address + 6;

	CClientSocket = 0x01D8B478;
}

void hook(){
	BypassSD();

	AirMemory AM;
	AM.Init();
	DWORD Address;

	Address = AM.AobScan("E9 ?? ?? ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 8B 2D ?? ?? ?? ?? 89 6C 24 ?? 85 ED 74");
	if(!Address){
		Address = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 8B 2D ?? ?? ?? ?? 89 6C 24 ?? 85 ED 74");
	}
	if(!Address) AobError();
	set_hook(Address, 7, ProcessPacket_Hook, &_ProcessPacket);


	Address = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B F1 6A ?? 8D 4C 24 ?? C6 86 ?? ?? ?? ?? 01 E8");
	if(!Address) AobError();
	set_hook(Address, 7, InitHeartBeatAndDisconnect, &_InitHeartBeat);
	Header_HeartBeat = *(BYTE *)(Address + 0x27);

	Address = AM.AobScan("68 ?? ?? ?? ?? 8D 4C 24 ?? E8 ?? ?? ?? ?? 8B 44 24 ?? 50 8D 4C 24 ?? C7 44 24 ?? 00 00 00 00 E8 ?? ?? ?? ?? 8B 4C 24 ?? 51 8D 4C 24 ?? E8 ?? ?? ?? ?? E8");
	if(!Address) AobError();

	Header_EnterStarPlanet = *(DWORD *)(Address + 1);


	Address = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC 0C 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 44 24 ?? 64 A3 00 00 00 00 8B 0D ?? ?? ?? ?? 8B 6C 24 ?? 85 C9 0F 84 ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 0F 84");
	if(!Address) AobError();
	AM.MemoryWriter(Address, "C3");//Dlg
	//AutoLogin_Hook();
}

