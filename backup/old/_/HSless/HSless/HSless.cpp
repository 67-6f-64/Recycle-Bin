#include"HSless.h"
#include"AirAsm.h"
#include"lib/Air.h"
#pragma comment(lib, "lib/AirLib2.lib")

void HSless();

DWORD MSCRC_HookAddr;
DWORD MSCRC_ReturnAddr;
DWORD VEHDebug_Detection;
DWORD ProcessList_Packet;
DWORD HackShield_Initiazation;
DWORD HackShield_HeartBeat;
DWORD EHSvc_Loader1;
DWORD EHSvc_Loader2;
DWORD HSUpdate_exe;
DWORD ASPLunchr_exe;
DWORD mkd25tray_exe;
DWORD autoup_exe;
DWORD ThemidaCRCs;
BYTE *Memory;
DWORD Memory_Start;
DWORD Memory_End;

void ScanAddresses(HWND hWndParent, WORD nIDDlgItem){
	MSCRC_HookAddr = AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9") + 6;
	MSCRC_ReturnAddr = AobScan("8A 11 80 C2 01 8B 4D 18");
	VEHDebug_Detection = AobScan("55 8B EC 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9", 2);
	ProcessList_Packet = AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	HackShield_Initiazation = AobScan("81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 55 56 6A 00 8B F1 E9");
	HackShield_HeartBeat = AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 44 24 ?? 6A 00 50 E8");
	EHSvc_Loader1 = AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 85 CC FE FF FF 00 00 00 00 C6 85 C0 FC FF FF 00 B9 82 00 00 00 33 C0 8D BD C1 FC FF FF F3 AB");
	EHSvc_Loader2 = AobScan("55 8B EC 81 EC ?? ?? ?? ?? 57 C7 45 BC 00 00 00 00 C7 45 EC 00 00 00 00 C6 85 AC FD FF FF 00 C7 45 E8 00 00 00 00 C7 45 F4 00 00 00 00 C7 45 F8 00 00 00 00 C7 45 C0 00 00 00 00 B9 09 00 00 00 33 C0 8D 7D C4 F3 AB");
	HSUpdate_exe = AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 45 BC 00 00 00 00 C7 45 E4 00 00 00 00 C7 85 A8 FE FF FF 00 00 00 00 C7 45 D4 00 00 00 00 33 C0 89 45 D8 89 45 DC 89 45 E0 C7 85 18 FE FF FF 00 00 00 00 B9 10 00 00 00 33 C0 8D BD 1C FE FF FF F3 AB");
	ASPLunchr_exe = AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", 1);
	mkd25tray_exe = AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", 2);
	autoup_exe = AobScan("56 8D 71 2C 8B CE E8");
	ThemidaCRCs = AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 6A 01 8B 8D ?? ?? ?? ?? E8");
	if(!ThemidaCRCs){
		ThemidaCRCs = AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? C7 85 ?? ?? ?? ?? 00 00 00 00 6A 01 8B 8D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 8D ?? ?? ?? ?? E8 ?? ?? ?? ?? 6A 28 8B 8D ?? ?? ?? ?? E8");
	}
	Air::AddFormatString(hWndParent, nIDDlgItem, "---------- AobScan ----------");
	Air::AddFormatString(hWndParent, nIDDlgItem, "MSCRC HookAddr = %d", MSCRC_HookAddr);
	Air::AddFormatString(hWndParent, nIDDlgItem, "MSCRC ReturnAddr = %d", MSCRC_ReturnAddr);
	Air::AddFormatString(hWndParent, nIDDlgItem, "DR_CHECK(HWBP) = %d", VEHDebug_Detection);
	Air::AddFormatString(hWndParent, nIDDlgItem, "ProcessListPacket = %d", ProcessList_Packet);
	Air::AddFormatString(hWndParent, nIDDlgItem, "HSless1(Init) = %d", HackShield_Initiazation);
	Air::AddFormatString(hWndParent, nIDDlgItem, "HSless2(HB) = %d", HackShield_HeartBeat);
	Air::AddFormatString(hWndParent, nIDDlgItem, "HSless3(Load) = %d", EHSvc_Loader1);
	Air::AddFormatString(hWndParent, nIDDlgItem, "HSless4(Load) = %d", EHSvc_Loader2);
	Air::AddFormatString(hWndParent, nIDDlgItem, "HSUpdate = %d", HSUpdate_exe);
	Air::AddFormatString(hWndParent, nIDDlgItem, "ASPLunchr = %d", ASPLunchr_exe);
	Air::AddFormatString(hWndParent, nIDDlgItem, "mkd25tray = %d", mkd25tray_exe);
	Air::AddFormatString(hWndParent, nIDDlgItem, "autoup = %d", autoup_exe);
	Air::AddFormatString(hWndParent, nIDDlgItem, "ThemidaCRCs = %d", ThemidaCRCs);
	Air::AddFormatString(hWndParent, nIDDlgItem, "-----------------------------");
	Memory = MemoryDump(NULL, &Memory_Start, &Memory_End);
	Air::AddFormatString(hWndParent, nIDDlgItem, "MemoryDump(%d) %d - %d", Memory, Memory_Start, Memory_End);

	HSless();
	Air::AddFormatString(hWndParent, nIDDlgItem, "-----------------------------");
	Air::AddFormatString(hWndParent, nIDDlgItem, "BYPASS IS INSTALLED");
}

void _declspec(naked) MSCRCHook(){
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
		add ecx,Memory
Ending:
		jmp dword ptr [MSCRC_ReturnAddr]
	}
}

void HSless(){
	if(MSCRC_HookAddr && MSCRC_ReturnAddr){
		WriteHook(MSCRC_HookAddr, "jne", (DWORD)MSCRCHook);
	}
	
	if(VEHDebug_Detection){
		WriteCode(VEHDebug_Detection, "31 C0 C3");
	}

	if(ProcessList_Packet){
		WriteCode(ProcessList_Packet, "31 C0 C2 04 00");
	}

	if(HackShield_Initiazation && HackShield_HeartBeat && EHSvc_Loader1 && EHSvc_Loader2){
		WriteCode(HackShield_Initiazation, "31 C0 C2 04 00");
		WriteCode(HackShield_HeartBeat, "31 C0 C2 04 00");
		WriteCode(EHSvc_Loader1, "31 C0 C2 10 03");
		WriteCode(EHSvc_Loader2, "31 C0 C2 18 00");
	}

	if(HSUpdate_exe && ASPLunchr_exe && mkd25tray_exe && autoup_exe){
		WriteCode(HSUpdate_exe, "31 C0 C2 10 00");
		WriteCode(ASPLunchr_exe, "31 C0 C3");
		WriteCode(mkd25tray_exe, "31 C0 C3");
		WriteCode(autoup_exe, "31 C0 C3");
	}

	if(ThemidaCRCs){
		WriteCode(ThemidaCRCs, "31 C0 C3");
	}
}