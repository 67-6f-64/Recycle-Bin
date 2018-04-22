#include"AirBypass2.h"
#include"MultiMS.h"


DWORD Memory, Memory_Start, Memory_End;
DWORD MSCRC1, MSCRC1_Ret;

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

DWORD HS_Init_Ret;

void _declspec(naked) HS_ManualInit(){
	_asm{
		mov dword ptr [esp+0x14],0x024A3DBE
		push ebp
		mov ebp,esp
		sub esp,0x2B0
		jmp dword ptr [HS_Init_Ret]
	}
}


DWORD DR_Check, ProcessListPacket, ASPLunchr, mkd25tray, autoup;
DWORD WindowsHook, HSUpdate;
DWORD HS_Init;
DWORD MSCRC23;

void Bypass(){
	AM.Init();
	AM.CreateMemoryDump();
	AM.GetDumpInfo(&Memory_Start, &Memory_End, &Memory);

	MSCRC1 = AM.AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9") + 6;
	MSCRC1_Ret = AM.AobScan("8A 11 80 C2 01 8B 4D 18");

	DR_Check = AM.AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");
	ProcessListPacket = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	
	ASPLunchr = AM.AobScan("83 EC 0C 56 57 6A 00 8B F1 E9");
	mkd25tray = AM.AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", 2);
	autoup = AM.AobScan("56 8D 71 2C 8B CE E8");

	BW.AddString(EDIT_LOG, "********** AobScan Results **********\r\n");
	BW.AddFormatString(EDIT_LOG, "MSCRC1 = %d, MSCRC1_Ret = %d\r\nDR_Check = %d\r\nProcessListPacket = %d\r\nASPLunchr = %d\r\nmkd25tray = %d\r\nautoup = %d\r\n", MSCRC1, MSCRC1_Ret, DR_Check, ProcessListPacket, ASPLunchr, mkd25tray, autoup);
	BW.AddString(EDIT_LOG, "*************************************\r\n");

	AM.WriteHook(MSCRC1, JNE, MSCRC1_Hook);

	AM.MemoryWriter(DR_Check, "31 C0 C3");
	AM.MemoryWriter(ProcessListPacket, "31 C0 C2 04 00");

	AM.MemoryWriter(ASPLunchr, "31 C0 C3");
	AM.MemoryWriter(mkd25tray, "31 C0 C3");
	AM.MemoryWriter(autoup , "31 C0 C3");

	WindowsHook = AM.AobScan("56 8B 74 24 08 57 8B F9 8B 47 14 85 F6 74 06 85 C0 74 78 EB 04");
	HSUpdate = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 45 BC 00 00 00 00 C7 45 E4 00 00 00 00 C7 85 A8 FE FF FF 00 00 00 00 C7 45 D4 00 00 00 00 33 C0 89 45 D8 89 45 DC 89 45 E0 C7 85 18 FE FF FF 00 00 00 00 B9 10 00 00 00 33 C0 8D BD 1C FE FF FF F3 AB");
	
	BW.AddFormatString(EDIT_LOG, "SetWindowsHookExA = %d\r\nHSUpdate:%d\r\n", WindowsHook, HSUpdate);

	
	AM.MemoryWriter(WindowsHook, "31 C0 C2 04 00");
	AM.MemoryWriter(HSUpdate, "31 C0 C2 10 00");

	
	HS_Init = AM.AobScan("55 8B EC 81 EC B0 02 00 00 57 C7 ?? ?? 00 00 00 00 C7 ?? ?? 00 00 00 00 C6 ?? ?? ?? ?? ?? 00");
	
	if(HS_Init){
		AM.WriteHook(HS_Init, JMP, HS_ManualInit, &HS_Init_Ret, 4);
		BW.AddFormatString(EDIT_LOG, "Patched Multi Client Detection\r\n");
	}
	

	//
	MSCRC23 = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D ?? ?? 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? C7 85 ?? ?? ?? ?? 00 00 00 00 6A 01 8B 8D ?? ?? ?? ?? E8");
	if(MSCRC23){
		AM.MemoryWriter(MSCRC23, "31 C0 C2 04 00");
		BW.AddFormatString(EDIT_LOG, "Removed MSCRC2/3\r\n");
	}
	//
	MultiMS();
}