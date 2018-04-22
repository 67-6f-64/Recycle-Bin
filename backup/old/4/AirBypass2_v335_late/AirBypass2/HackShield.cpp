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

DWORD FT_Ret;
void _declspec(naked) FuckThread(){
	_asm{
		mov eax,[esp]
		sub eax,[HS_Memory_Start]
		cmp eax,0x3C9E3C
		je FT_FR
		push ebp
		mov ebp,esp
		push esi
		push edi
		jmp dword ptr [FT_Ret]
FT_FR:
		xor eax,eax
		ret
	}
}

DWORD Flag15 = 0;
DWORD In_15_Ret;
void _declspec(naked) In_15(){
	_asm{
		mov dword ptr [Flag15],1
		mov eax,esp
		push [eax+0x0C]
		push [eax+0x08]
		push [eax+0x04]
		call EHSvc15
		add esp,0x0C
		mov dword ptr [Flag15],0
		ret
EHSvc15:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [In_15_Ret]
	}
}

DWORD DIO_Ret;
DWORD DIO_Push;
void _declspec(naked) DIO(){
	_asm{
		cmp dword ptr [Flag15],0
		je _DIO
		nop
		nop
		nop
		nop
		nop
_DIO:
		push 0x10
		push [DIO_Push]
		jmp dword ptr [DIO_Ret]
	}
}

//0x024A61AE
DWORD In_6_Ret;

void _declspec(naked) In_6(){
	_asm{
		mov eax,esp	
		push [eax+0x20]
		push [eax+0x2C]
		push [eax+0x28]
		push [eax+0x24]
		push [eax+0x20]
		push [eax+0x1C]
		push [eax+0x18]
		push [eax+0x14]
		push 0x024A61AE//push [eax+0x10]
		push [eax+0x0C]
		push [eax+0x08]
		push [eax+0x04]
		call EHSvc_6
		ret 0x0030
EHSvc_6:
		push ebp
		mov ebp,esp
		push 0xFFFFFFFF
		jmp dword ptr [In_6_Ret]
	}
}


void HackShieldBypass(){
	DWORD old, dwAddr;


	//

	dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("KERNELBASE.dll"), "DeviceIoControl");

	VirtualProtect((DWORD *)dwAddr, 7, PAGE_EXECUTE_READWRITE, &old);
	DIO_Ret = dwAddr + 7;
	DIO_Push = *(DWORD *)(dwAddr+3);
	*(BYTE *)(dwAddr) = 0xE9;
	*(DWORD *)(dwAddr + 1) = (DWORD)DIO - dwAddr - 5;
	//

	BW.AddString(EDIT_LOG, "Waiting for loading hackshield...\r\n");

	HM.Init("EHSvc.dll");
	HM.CreateMemoryDump();
	HM.GetDumpInfo(&HS_Memory_Start, &HS_Memory_End, &HS_Memory);

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
	
	//EHSvc.dll+4360 load  copies of dlls
	//EHSvc.dll+4FC0 error code
	HM.MemoryWriter(0x9E7CA, "8B 85 E0 FC FF FF");//Load Original DLL
	HM.MemoryWriter(0x90A6A, "B8 01 00 00 00 C2 0C 00");//SC Module Check

	//HM.MemoryWriter(0x62C24, "90 90 90 90 90 90");//MultiClient FileCheck
	
	//Bypass Multi Client Detection
	//HM.MemoryWriter(0x617B5, "31 C0");
	//HM.MemoryWriter(0x617BC, "90 90 90 90 90 90");
	//HM.MemoryWriter(0x617C2, "B8 02 00 00 00 90 90 90");
	//HM.MemoryWriter(0x61858, "31 C0");
	
	//more
	//HM.MemoryWriter(0x6226F, "90 E9");

	//NEW!
	//HM.MemoryWriter(0x8D896, "31 C0 C3");//DriverCallBack

	//HM.MemoryWriter(0x62B59, "90 E9");//FileCheck
	
	HM.WriteHook(0x18011, JMP, HSLog, &HSLog_Ret);//HSLog


	//HM.WriteHook(0x53385D, CALL, AntiCrash);//Suspend

	//HM.WriteHook(0x223F0, JMP, ServiceDispatch, &SD_Ret);

	//HM.MemoryWriter(0x1D910, "31 C0 C2 04 00"); faggot routine



	HM.MemoryWriter(0x4A10, "31 C0 C2 04 00");//restore ntdll

	dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");

	VirtualProtect((DWORD *)dwAddr, 5, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwAddr) = 0xE9;
	*(DWORD *)(dwAddr + 1) = (DWORD)EternalSleep - dwAddr - 5;

	VirtualProtect((DWORD *)dwAddr, 5, PAGE_EXECUTE, &old);

	//keeege

	//HM.MemoryWriter(0x3C9E3C, "83 C4 0C 90 90 90 90 90 90 90");//FileCheck
	//HM.WriteHook(0xD271A, JMP, FuckThread, &FT_Ret);


//	HM.WriteHook(0x1BCE0, JMP, In_15, &In_15_Ret);

	//HM.MemoryWriter(0x134E0, "B8 01 00 00 00 C3");//Driver Init


	HM.WriteHook(0x1AB50, JMP, In_6, &In_6_Ret);

	BW.AddString(EDIT_LOG, "most detections are disabled now!\r\n");
}

///
