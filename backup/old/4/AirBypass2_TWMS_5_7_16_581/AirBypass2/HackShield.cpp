#include"AirBypass2.h"

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
		sub esp,0x04
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
		cmp edx,0x9B252 - 0x100
		jb Ending_HSCRC3_2
		cmp edx,0x37C217 + 0x100
		ja Ending_HSCRC3_2
		cmp edx,0x9B252 + 0x100
		jb Ending_HSCRC3_1
		cmp edx,0x37C217 - 0x100
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
	for(i=0; i<4; i++){
		*(DWORD *)(dwHSCRC5_Table + i*8) = ((*(DWORD *)(dwHSCRC5_Table + i*8)^*(DWORD *)(dwHSCRC5_Table + 0x20)) - HS_Memory_Start + HS_Memory)^(*(DWORD *)(dwHSCRC5_Table + 0x20));
	}
}

/*
//Log
void _stdcall _HSLog(DWORD dwReturn, char *Buffer){
	FILE *fp;
	char FileName[256];
	DWORD pid = GetProcessId(GetCurrentProcess());
	sprintf(FileName, "Riremito_%04X.txt", pid);
	fp = fopen(FileName, "a");
	fprintf(fp, "EHSvc.dll+%X:%s\r\n", dwReturn-HS_Memory_Start, Buffer);
	fclose(fp);
}

DWORD HSLog_Ret;

void _declspec(naked) HSLog(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x08]//Buffer
		push [eax+0x448]//Return
		call _HSLog
		popad
		xor eax,eax
		add esp,0x0C
		pop edi
		pop ebx
		jmp dword ptr [HSLog_Ret]
	}
}
//

void AntiCrash(){
	FILE *fp;
	char FileName[256];
	DWORD pid = GetProcessId(GetCurrentProcess());
	sprintf(FileName, "Riremito_%04X.txt", pid);
	fp = fopen(FileName, "a");
	fprintf(fp, "Anti Crash %X\r\n", GetCurrentThreadId());
	fclose(fp);

	Sleep(INFINITE);
}

DWORD SD_Ret;
void _declspec(naked) ServiceDispatch(){
	_asm{
		cmp [esp+0x04],0x04//Init
		je SD_OK
		cmp [esp+0x04],0x05//DriverInit
		je SD_OK
		cmp [esp+0x04],0x0D//Beat
		je SD_OK
		mov eax,1
		ret 0x000C
SD_OK:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [SD_Ret]
	}
}


void _stdcall ThreadInfo(PHANDLE handle, DWORD dwFunction){
	if(dwFunction >= HS_Memory_Start && dwFunction <= HS_Memory_End){
	FILE *fp;
	char FileName[256];
	DWORD pid = GetProcessId(GetCurrentProcess());
	sprintf(FileName, "Riremito_%04X.txt", pid);
	fp = fopen(FileName, "a");
	fprintf(fp, "ThreadId = %X, EHSvc.dll+%X\r\n", GetThreadId(*handle), dwFunction-HS_Memory_Start);
	fclose(fp);
		//HM.WriteHook(dwFunction, JMP, AntiCrash);
	}
}

void _declspec(naked) EternalSleep(){
	_asm{
		mov eax,0xA5
		xor ecx,ecx
		lea edx,[esp+0x04]
		call fs:[0xC0]
		add esp,0x04
		
		push eax
		mov eax,esp
		pushad
		push [eax+0x18]
		push [eax+0x08]
		call ThreadInfo
		popad
		pop eax
		ret 0x002C
	}
}
*/


void int3bp_Bypass(){
	DWORD old;
	DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "DbgBreakPoint");

	VirtualProtect((DWORD *)dwAddr, 1, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwAddr) = 0xCC;
}

DWORD ARC_Ret;

void _declspec(naked) AddressRangeCheckBypass(){
	_asm{
		push eax
		mov eax,[HS_Memory]
		mov dword ptr [esp+0x08],eax
		pop eax
		jmp dword ptr [ARC_Ret]
	}
}

void HackShieldBypass(){
	BW.AddString(EDIT_LOG, "Waiting for loading hackshield...\r\n");

	HM.Init("EHSvc.dll");

	HM.CreateMemoryDump();
	HM.GetDumpInfo(&HS_Memory_Start, &HS_Memory_End, &HS_Memory);

	
	HM.WriteHook(0x9B252, JMP, HSCRC1_Hook, &HSCRC1_Ret, 2);//HSCRC1
	HM.WriteHook(0x37C217, JMP, HSCRC2_Hook, &HSCRC2_Ret, 1);//HSCRC2
	HM.WriteHook(0x26B6B7, JMP, HSCRC3_Hook);//HSCRC3
	HSCRC3_Ret = HS_Memory_Start + 0x26AEC2;
	HM.WriteHook(0x408F7, JMP, HSCRC4_Hook, &HSCRC4_Ret, 3);//HSCRC4
	HSCRC5_TableHack(HS_Memory_Start + 0x164048);
	
	HM.WriteHook(0x3E8F86, JMP, AddressRangeCheckBypass);//Address Range Check
	HM.WriteHook(0x3E8FA4, JMP, AddressRangeCheckBypass);//Address Range Check
	HM.WriteHook(0x3E8FC2, JMP, AddressRangeCheckBypass);//Address Range Check

	ARC_Ret = HS_Memory_Start + 0x26A3C2;
	
	
	HM.MemoryWriter(0x56BF0, "31 C0 C2 04 00");//Process Scanner
	HM.MemoryWriter(0x5D8B0, "31 C0 C2 04 00");//Module Scanner

	HM.MemoryWriter(0x11280, "31 C0 C3");//HardwareBreakPoint Detection(Main)
	HM.MemoryWriter(0xF870, "31 C0 C3");//HardwareBreakPoint Detection2
	HM.MemoryWriter(0xFA60, "31 C0 C3");//HardwareBreakPoint Detection3
	HM.MemoryWriter(0x101F0, "31 C0 C2 18 00");//HardwareBreakPoint Detection4
	
	HM.MemoryWriter(0x779A0, "31 C0 C3");//SoftwareBreakPoint Detection
	HM.MemoryWriter(0xDA5CA, "B8 00 00 00 00");//Memory Protection

	

	//HM.MemoryWriter(0xA9D1F, "8B 85 E0 FC FF FF");//Load Original DLL
	//HM.MemoryWriter(0x9BA1A, "B8 01 00 00 00 C2 0C 00");//SC Module Check

	//HM.MemoryWriter(0x4A10, "31 C0 C2 04 00");//restore ntdll
	
	//HM.WriteHook(0x191D1, JMP, HSLog, &HSLog_Ret);//HSLog


	/*
	DWORD old;
	DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");

	VirtualProtect((DWORD *)dwAddr, 5, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwAddr) = 0xE9;
	*(DWORD *)(dwAddr + 1) = (DWORD)EternalSleep - dwAddr - 5;

	VirtualProtect((DWORD *)dwAddr, 5, PAGE_EXECUTE, &old);

	*/

	//keeege
	BW.AddString(EDIT_LOG, "most detections are disabled now!\r\n");

	//HM.MemoryWriter(0x19000, "31 C0 C3");//HSLog
	//AM.RestoreMemory(HS_Init, 5);
	HS_Init = 0xDEADBEEF;

	int3bp_Bypass();
}