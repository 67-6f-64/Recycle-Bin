#include"AirWindow.h"
#pragma comment(lib, "AirWindow.lib")
#include"AirMemory.h"

AirWindow BW;
AirMemory AM, HM;

#define EDIT_LOG 2050
#define CHECK_AC 3050
#define CHECK_HL 3060

bool bAutoClose = true;
bool bHSless = false;

//////////////////////////////////////////////////
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
	DWORD MSCRC1;

	MSCRC1 = AM.AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9") + 6;
	MSCRC1_Ret = AM.AobScan("8A 11 80 C2 01 8B 4D 18");

	BW.AddFormatString(EDIT_LOG, "MSCRC1:%d\r\nMSCRC1_Ret:%d\r\n", MSCRC1, MSCRC1_Ret);

	AM.WriteHook(MSCRC1, JNE, MSCRC1_Hook);
}

void DisableDetection(){
	DWORD DR_Check, ProcessListPacket;
	
	DR_Check = AM.AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");
	ProcessListPacket = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	
	BW.AddFormatString(EDIT_LOG, "DR_Check:%d\r\nProcessListPacket:%d\r\n", DR_Check, ProcessListPacket);

	AM.MemoryWriter(DR_Check, "31 C0 C3");
	AM.MemoryWriter(ProcessListPacket, "31 C0 C2 04 00");
}

void DisableSecurity(){
	DWORD ASPLunchr, mkd25tray, autoup;

	ASPLunchr = AM.AobScan("83 EC 0C 56 57 6A 00 8B F1 E9");
	mkd25tray = AM.AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", 2);
	autoup = AM.AobScan("56 8D 71 2C 8B CE E8");

	BW.AddFormatString(EDIT_LOG, "ASPLunchr:%d\r\nmkd25tray:%d\r\nautoup:%d\r\n", ASPLunchr, mkd25tray, autoup);

	AM.MemoryWriter(ASPLunchr, "31 C0 C3");
	AM.MemoryWriter(mkd25tray, "31 C0 C3");
	AM.MemoryWriter(autoup , "31 C0 C3");
}

void HSLess(){
	DWORD HSless1, HSless2;
	HSless1 = AM.AobScan("81 EC 20 01 00 00 A1 ?? ?? ?? ?? 33 C4 89 84 24 1C 01 00 00 55 56 6A 00 8B F1 E9");
	HSless2 = AM.AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 92 01 00 00");

	BW.AddFormatString(EDIT_LOG, "HSless1:%d\r\nHSless2:%d\r\n",  HSless1, HSless2);

	AM.MemoryWriter(HSless1 , "31 C0 C2 04 00");
	AM.MemoryWriter(HSless2 , "31 C0 C2 04 00");
}

//////////////////////////////////////////////////
DWORD HS_Memory_Start, HS_Memory_End, HS_Memory;

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

void HackShieldBypass(){
	HM.WriteHook(0x902A2, JMP, HSCRC1_Hook, &HSCRC1_Ret, 2);//HSCRC1
	HM.WriteHook(0x35DBB9, JMP, HSCRC2_Hook, &HSCRC2_Ret, 1);//HSCRC2
	HM.WriteHook(0x2578AE, JMP, HSCRC3_Hook);//HSCRC3
	HSCRC3_Ret = HS_Memory_Start + 0x24FAD3;
	HM.WriteHook(0x38A37, JMP, HSCRC4_Hook, &HSCRC4_Ret, 3);//HSCRC4
	HSCRC5_TableHack(HS_Memory_Start + 0x153040);
	HM.MemoryWriter(0x4DB20, "31 C0 C2 04 00");//Process Scanner
	HM.MemoryWriter(0x548F0, "31 C0 C2 04 00");//Module Scanner
	HM.MemoryWriter(0x10AE0, "31 C0 C3");//HardwareBreakPoint Detection(Main)
	HM.MemoryWriter(0xF240, "31 C0 C3");//HardwareBreakPoint Detection2
	HM.MemoryWriter(0xF430, "31 C0 C3");//HardwareBreakPoint Detection3
	HM.MemoryWriter(0xFBC0, "31 C0 C2 18 00");//HardwareBreakPoint Detection4
	HM.MemoryWriter(0x6DCB0, "31 C0 C3");//SoftwareBreakPoint Detection
	HM.MemoryWriter(0xCA642, "B8 00 00 00 00");//Memory Protection
}
//////////////////////////////////////////////////

DWORD dwSendHook_Ret;

void _declspec(naked) SendHook(){
	_asm{
		pushad
		popad
		jmp dword ptr [dwSendHook_Ret]
	}
}

DWORD dwRecvPointer_Org;

void _declspec(naked) RecvHook(){
	_asm{
		pushad
		popad
		jmp dword ptr [dwRecvPointer_Org]
	}
}

void LibTest(){
	DWORD SendFunction, dwHookAddr, RecvPointer;
	SendFunction = AM.AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 83 EC ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 ?? ?? ?? 64 A3 00 00 00 00 ?? ?? ?? 6A 00 E9");
	BW.AddFormatString(EDIT_LOG, "SendFunction:%d\r\n", SendFunction);
	dwHookAddr = AM.AutoVMHook(SendFunction, SendHook, &dwSendHook_Ret);
	BW.AddFormatString(EDIT_LOG, "HookAddr:%d\r\n", dwHookAddr);

	RecvPointer = AM.AobScan("8B 7C 24 2C 8B CF C7 44 24 24 00 00 00 00 E8");
	if(RecvPointer){
		RecvPointer = *(DWORD *)(RecvPointer - 4);
		AM.PointerHook(RecvPointer, RecvHook, &dwRecvPointer_Org);
	}
}

////
void AirBypass(){

	BW.AddString(EDIT_LOG, "********** AobScan Results **********\r\n");

	AM.Init();
	AM.CreateMemoryDump();
	AM.GetDumpInfo(&Memory_Start, &Memory_End, &Memory);
	MSCRCBypass();
	DisableSecurity();
	DisableDetection();
	//LibTest();


	BW.AddString(EDIT_LOG, "\r\nWaiting for loading HackShield...\r\n");
	HM.Init("EHSvc.dll");
	HM.CreateMemoryDump();
	HM.GetDumpInfo(&HS_Memory_Start, &HS_Memory_End, &HS_Memory);
	HackShieldBypass();
	BW.AddString(EDIT_LOG, "Probably most detections are disabled!\r\n");

	Sleep(5000);

	//if(bHSless){
	//	HSLess();
	//	Sleep(3000);
	//}

	if(bAutoClose){
		//BW.UnInit();
	}
}

void create(HWND hWnd){
	BW.EditBox(EDIT_LOG, "AirBypass by ƒŠƒŒƒ~ƒg\r\nfor JMS and HS v5.6.34.449\r\n", 3,3, 394, 20);
	BW.ReadOnly(EDIT_LOG);
	BW.CheckBox(CHECK_AC, "Ž©“®I—¹", 3, 250, true);
	//BW.CheckBox(CHECK_HL, "HSLess", 100, 250);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AirBypass, NULL ,NULL, NULL);
}

void control(WORD wID){
	switch(wID){
	case CHECK_AC:
		bAutoClose = BW.CheckBoxStatus(wID);
		break;

	//case CHECK_HL:
		//bHSless = BW.CheckBoxStatus(wID);
		//break;

	default:
		break;
	}
}

void newthread(HINSTANCE hinstDLL){
	BW.Init(hinstDLL, "AirBypass", 400, 300, true, create, control);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)newthread, hinstDLL ,NULL, NULL);

	return TRUE;
}