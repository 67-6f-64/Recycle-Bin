#include"Additional.h"
#include"Function.h"

//55 8B EC 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9 2nd
DWORD dwVEHDebug = 0x00577800;

//6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9
DWORD dwProcessListPacket = 0x01082970;

////83 EC 08 56 8B F1 8D 4E 20 E8
DWORD dwInternalDetection = 0x010B91B0;

//81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 55 56 6A 00 8B F1 E9
DWORD dwHSless1 = 0x010B8FB0;//HackShield Initialization

//6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8D 44 24 ?? 6A 00 50 E8
DWORD dwHSless2 = 0x010B9440;//HackShield HeartBeat

//55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 85 CC FE FF FF 00 00 00 00 C6 85 C0 FC FF FF 00 B9 82 00 00 00 33 C0 8D BD C1 FC FF FF F3 AB
DWORD dwHSless3 = 0x010D7EBB;//Load EHSvc.dll

//55 8B EC 81 EC ?? ?? ?? ?? 57 C7 45 BC 00 00 00 00 C7 45 EC 00 00 00 00 C6 85 AC FD FF FF 00 C7 45 E8 00 00 00 00 C7 45 F4 00 00 00 00 C7 45 F8 00 00 00 00 C7 45 C0 00 00 00 00 B9 09 00 00 00 33 C0 8D 7D C4 F3 AB
DWORD dwHSless4 = 0x010D6AF2;//Load EHSvc.dll

//55 8B EC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 C4 ?? ?? ?? ?? 53 56 57 89 65 E8 C7 45 BC 00 00 00 00 C7 45 E4 00 00 00 00 C7 85 A8 FE FF FF 00 00 00 00 C7 45 D4 00 00 00 00 33 C0 89 45 D8 89 45 DC 89 45 E0 C7 85 18 FE FF FF 00 00 00 00 B9 10 00 00 00 33 C0 8D BD 1C FE FF FF F3 AB
DWORD dwHSUpdate = 0x010DB7D9;

DWORD dwStartUpDlg = 0x009DFF30;
DWORD dwCreateMutexA_Prev = 0x010175C8;
DWORD dwCreateMutexA_Next = 0x01017629;
DWORD dwCreateWindowExA = 0x0101E7E9;
DWORD dwCreateWindowExA_Return = dwCreateWindowExA + 0x06;
DWORD dwShowWindow = 0x0102316B;


void _stdcall _VisibleWindow(HWND hwnd){
	ShowWindow(hwnd, SW_SHOW);
}


void _declspec(naked) VisibleWindow(){
	_asm{
		pushad
		push eax
		call _VisibleWindow
		popad
		mov edx,[ebp-0x68]
		mov [edx+0x08],eax
		jmp dword ptr [dwCreateWindowExA_Return]
	}
}


void HSless(){
	WriteCODE(dwHSless1, "31 C0 C2 04 00");
	WriteCODE(dwHSless2, "31 C0 C2 04 00");
	WriteCODE(dwHSless3, "31 C0 C2 10 03");
	WriteCODE(dwHSless4, "31 C0 C2 18 00");
	WriteCODE(dwHSUpdate, "31 C0 C2 10 00");
}


void DisableAntiHack(){
	WriteCODE(dwVEHDebug, "31 C0 C3");
	WriteCODE(dwProcessListPacket, "31 C0 C2 04 00");
	WriteJMP(dwCreateMutexA_Prev, dwCreateMutexA_Next);
	WriteJMP(dwCreateWindowExA, (DWORD)VisibleWindow);
	WriteCODE(dwShowWindow, NULL, 18);
	WriteCODE(dwStartUpDlg, "31 C0 40 C3");
}

