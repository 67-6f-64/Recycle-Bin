#include"AirBypass2.h"

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

DWORD DR_Check, ProcessListPacket, ASPLunchr, mkd25tray, autoup;


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
}