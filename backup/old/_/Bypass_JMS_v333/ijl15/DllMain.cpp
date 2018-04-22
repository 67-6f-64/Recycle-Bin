#include<Windows.h>
#include"ijl15.h"
#include"AirMemory.h"
#include"MultiMS.h"
//code start
WORD Header_Login = 0x0037;
WORD Header_Connect = 0x001D;
BYTE HWID[10];

void CreateNewHWID(BYTE *Packet){
	int i;
	WORD wMapleIDSize = *(WORD *)(&Packet[24]);
	char *MapleID = (char *)(&Packet[26]);

	ZeroMemory(HWID, 10);
	RtlCopyMemory(HWID, &Packet[2], 10);
	
	for(i=0; i<10; i++){
		HWID[i] ^=  wMapleIDSize;
	}

	for(i=0; i<10; i++){
		HWID[i] ^= MapleID[i%wMapleIDSize];
	}
}


void _stdcall HWIDRandomizer(BYTE *Packet){
	WORD Header = *(WORD *)&Packet[0];

	if(Header == Header_Login){
		CreateNewHWID(Packet);
		RtlCopyMemory(&Packet[2], HWID, 10);
	}
	else if(Header == Header_Connect){
		RtlCopyMemory(&Packet[10], HWID, 10);
	}
}

//55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 ?? ?? ?? 64 A3 00 00 00 00 ?? ?? ?? 6A 00 E9

DWORD HookAddr = 0x01C2D069;
DWORD HookRet;

void _declspec(naked) PacketHook(){
	_asm{
		pushad
		mov eax,[ebp+0x08]
		push [eax+0x04]
		call HWIDRandomizer
		popad
		jmp dword ptr [HookRet]
	}
}


void HWIDBypass(){
	Air::WriteCall(HookAddr, (DWORD)PacketHook, &HookRet);
}


DWORD Memory;
DWORD Memory_Start, Memory_End;
DWORD MSCRC1_Ret;

void _declspec(naked) MSCRC1_Hook(){
	_asm{
		xor eax,eax
		add eax,edx
		mov edx,[ebp+0x18]
		sub eax,0x08
		mov eax,[edx]
		shr eax,0x08
		xor ecx,ecx
		mov ecx,eax
		shl ecx,0x08
		mov ecx,[ebp+0x08]
		add ecx,[ebp-0x38]
		xor edx,edx
		mov ebx,[ebp+0x08]
		cmp ecx,[Memory_Start]
		jb Ending
		cmp ecx,[Memory_End]
		ja Ending
		sub ecx,[Memory_Start]
		add ecx,[Memory]
Ending:
		jmp dword ptr [MSCRC1_Ret]
	}
}

void MSCRCBypass(){
	DWORD MSCRC1 = Air::AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9") + 6;
	MSCRC1_Ret = Air::AobScan("8A 11 80 C2 01 8B 4D 18");
	Air::CreateMemoryDump(&Memory, &Memory_Start, &Memory_End);
	Air::FullAccess(MSCRC1, 7);
	*(DWORD *)(MSCRC1 + 2) = (DWORD)MSCRC1_Hook - MSCRC1 - 6;
}

void DisableDetections(){
	DWORD DR_Check = Air::AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");
	DWORD ProcessListPacket = Air::AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	
	Air::WriteCode(DR_Check, "31 C0 C3");
	Air::WriteCode(ProcessListPacket, "31 C0 C2 04 00");
}

void DisableSecurity(){
	DWORD ASPLunchr = Air::AobScan("83 EC 0C 56 57 6A 00 8B F1 E9");
	DWORD mkd25tray = Air::AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", 2);
	DWORD Autoup = Air::AobScan("56 8D 71 2C 8B CE E8");

	Air::WriteCode(ASPLunchr, "33 C0 C3");
	Air::WriteCode(mkd25tray, "33 C0 C3");
	Air::WriteCode(Autoup , "33 C0 C3");
}

void HSless(){
	DWORD HSless1 = Air::AobScan("81 EC 20 01 00 00 A1 ?? ?? ?? ?? 33 C4 89 84 24 1C 01 00 00 55 56 6A 00 8B F1 E9");
	DWORD HSless2 = Air::AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 92 01 00 00");

	
	Air::WriteCode(HSless1 , "31 C0 C2 04 00");
	Air::WriteCode(HSless2 , "31 C0 C2 04 00");
}

DWORD DeviceIoControl_Ret;

void _declspec(naked) _DeviceIoControl(){
	_asm{
		cmp dword ptr [esp+0x08],0x80000040
		je DriverReject
		mov edi,edi
		push ebp
		mov ebp,esp
		jmp dword ptr [DeviceIoControl_Ret]
DriverReject:
		mov eax,1
		ret 0x0020
	}
}

void HSBypass(){
	DWORD dwDeviceIoControl = Air::GetAPIAddress("KERNEL32.DLL", "DeviceIoControl");
	Air::WriteCodeAtAPI("KERNEL32.DLL", "K32GetMappedFileNameA", "31 C0 C2 10 00");
	Air::WriteCodeAtAPI("KERNEL32.DLL", "K32EnumProcesses", "31 C0 C2 0C 00");
	Air::WriteCodeAtAPI("KERNEL32.DLL", "K32EnumProcessModules", "31 C0 C2 10 00");
	DeviceIoControl_Ret = dwDeviceIoControl + 5;
	Air::FullAccess(dwDeviceIoControl, 5);
	Air::WriteJump(dwDeviceIoControl, (DWORD)_DeviceIoControl);
}

DWORD HS_Memory, HS_Memory_Start, HS_Memory_End;
DWORD HSCRC1_Ret, HSCRC2_Ret, HSCRC3_Ret, HSCRC4_Ret;

void _declspec(naked) HSCRC1_Hook(){
	_asm{
		cmp ecx,[HS_Memory_Start]
		jb Ending_HSCRC1
		cmp ecx,[HS_Memory_End]
		ja Ending_HSCRC1
		sub ecx,[HS_Memory_Start]
		add ecx,[HS_Memory]
Ending_HSCRC1:
		mov dl,[ecx]
		xor eax,edx
		mov ecx,[ebp+0x10]
		jmp dword ptr [HSCRC1_Ret]
	}
}

void _declspec(naked) HSCRC2_Hook(){
	_asm{
		cmp ebx,[HS_Memory_Start]
		jb Ending_HSCRC2
		cmp ebx,[HS_Memory_End]
		ja Ending_HSCRC2
		sub ebx,[HS_Memory_Start]
		add ebx,[HS_Memory]
Ending_HSCRC2:
		add al,[ebx]
		pop ebx
		push edx
		mov dh,-0x78
		jmp dword ptr [HSCRC2_Ret]
	}
}

void _declspec(naked) HSCRC3_Hook(){
	_asm{
		cmp edx,[HS_Memory_Start]
		jb Ending_HSCRC3
		cmp edx,[HS_Memory_End]
		ja Ending_HSCRC3
		push edx
		sub edx,[HS_Memory_Start]
		cmp edx,0x902A2 - 0x100
		jb Ending_HSCRC3_2
		cmp edx,0x35DBB9 + 0x100
		ja Ending_HSCRC3_2
		cmp edx,0x902A2 + 0x100
		jb Ending_HSCRC3_1
		cmp edx,0x35DBB9 - 0x100
		ja Ending_HSCRC3_1
		jmp Ending_HSCRC3_2
Ending_HSCRC3_1:
		add edx,[HS_Memory]
		mov edx,[edx]
		mov dword ptr [esp+0x04],edx
Ending_HSCRC3_2:
		pop edx
Ending_HSCRC3:
		jmp dword ptr [HSCRC3_Ret]
	}
}

void _declspec(naked) HSCRC4_Hook(){
	_asm{
		push esi
		cmp esi,[Memory_Start]
		jb Ending_
		cmp esi,[Memory_End]
		ja Ending_
		sub esi,[Memory_Start]
		add esi,[Memory]
Ending_:
		lea edi,[ebp-0x1228]
		repe movsd
		pop esi
		jmp dword ptr [HSCRC4_Ret]
	}
}

void HSCRC5_TableHack(DWORD dwHSCRC5_Table){
	int i;
	for(i=0; i<3; i++){
		*(DWORD *)(dwHSCRC5_Table + i*8) = ((*(DWORD *)(dwHSCRC5_Table + i*8)^*(DWORD *)(dwHSCRC5_Table + 0x18)) - HS_Memory_Start + HS_Memory)^(*(DWORD *)(dwHSCRC5_Table + 0x18));
	}
}

DWORD MultiClient_Ret;
DWORD PointerX;

void _declspec(naked) MultiClient_Bypass(){
	_asm{
		mov eax,[PointerX]
		mov [eax],1
		mov eax,1
		jmp dword ptr [MultiClient_Ret]
	}
}
void HackShieldBypass(){

	while(!GetModuleHandleA("EHSvc.dll")){
		Sleep(100);
	}

	DWORD EHSvc = (DWORD)GetModuleHandleA("EHSvc.dll");

	Air::CreateMemoryDump(&HS_Memory, &HS_Memory_Start, &HS_Memory_End, "EHSvc.dll");
	
	Air::WriteJumpAtModule("EHSvc.dll", 0x902A2, (DWORD)HSCRC1_Hook, &HSCRC1_Ret, 2);//HSCRC1
	Air::WriteJumpAtModule("EHSvc.dll", 0x35DBB9, (DWORD)HSCRC2_Hook, &HSCRC2_Ret, 1);//HSCRC2
	Air::WriteJumpAtModule("EHSvc.dll", 0x2578AE, (DWORD)HSCRC3_Hook);//HSCRC3
	HSCRC3_Ret = EHSvc + 0x24FAD3;
	Air::WriteJumpAtModule("EHSvc.dll", 0x38A37, (DWORD)HSCRC4_Hook, &HSCRC4_Ret, 3);//HSCRC4
	HSCRC5_TableHack(EHSvc + 0x153040);
	Air::WriteCodeAtModule("EHSvc.dll", 0x4DB20, "31 C0 C2 04 00");//Process Scanner
	Air::WriteCodeAtModule("EHSvc.dll", 0x548F0, "31 C0 C2 04 00");//Module Scanner
	Air::WriteCodeAtModule("EHSvc.dll", 0x10AE0, "31 C0 C3");//HardwareBreakPoint Detection(Main)
	Air::WriteCodeAtModule("EHSvc.dll", 0xF240, "31 C0 C3");//HardwareBreakPoint Detection2
	Air::WriteCodeAtModule("EHSvc.dll", 0xF430, "31 C0 C3");//HardwareBreakPoint Detection3
	Air::WriteCodeAtModule("EHSvc.dll", 0xFBC0, "31 C0 C2 18 00");//HardwareBreakPoint Detection4
	Air::WriteCodeAtModule("EHSvc.dll", 0x6DCB0, "31 C0 C3");//SoftwareBreakPoint Detection
	Air::WriteCodeAtModule("EHSvc.dll", 0xCA642, "B8 00 00 00 00");//Memory Protection
	//Air::WriteCodeAtModule("EHSvc.dll", 0x48820, "31 C0 C2 10 00");//HSCRC6 for DLLs

}

//code end

BYTE _CreateWindowExA[17] = {0x60, 0xE8, 0xFF, 0xFF, 0xFF, 0xFF, 0x61, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0xFF, 0xFF, 0xFF, 0xFF};
DWORD dwCreateWindowExA;

void RestoreHooks(){
	RtlCopyMemory((void *)dwCreateWindowExA, &_CreateWindowExA[7], 5);
}

void _stdcall ChainFunction(){
	RestoreHooks();
	//MultiMS();
	MSCRCBypass();
	DisableDetections();
	//HSless();
	DisableSecurity();
	//HSBypass();
	//HWIDBypass();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackShieldBypass, NULL, NULL, NULL);
}


void WriteHooks(){
	dwCreateWindowExA = Air::GetAPIAddress("USER32.DLL", "CreateWindowExA");

	Air::FullAccess(dwCreateWindowExA, 5);
	Air::FullAccess((DWORD)_CreateWindowExA, 17);

	//CreateHookFunction
	RtlCopyMemory(&_CreateWindowExA[7], (void *)dwCreateWindowExA, 5);

	*(DWORD *)(&_CreateWindowExA[1+1]) = (DWORD)ChainFunction - (DWORD)&_CreateWindowExA[1] - 5;
	*(DWORD *)(&_CreateWindowExA[12+1]) = (DWORD)(dwCreateWindowExA + 5) - (DWORD)&_CreateWindowExA[12] - 5;

	//HookCode
	Air::WriteJump(dwCreateWindowExA, (DWORD)&_CreateWindowExA[0]);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateExports();
	WriteHooks();

	return TRUE;
}