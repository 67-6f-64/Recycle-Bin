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
		/*
		push eax
		mov eax,[HS_Memory]
		mov eax,[eax+0x14ED74]
		test eax,eax
		je MODOR2_Ret
		lea eax,[eax+0x28]
		cmp eax,ecx
		jne MODOR2_Ret
		mov eax,[HS_Memory]
		add eax,0x14E800
		mov dl,[eax]
MODOR2_Ret:
		pop eax
			*/
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
		mov ebx,[esp]
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
		cmp edx,0x92812 - 0x100
		jb Ending_HSCRC3_2
		cmp edx,0x360040 + 0x100
		ja Ending_HSCRC3_2
		cmp edx,0x92812 + 0x100
		jb Ending_HSCRC3_1
		cmp edx,0x360040 - 0x100
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
		*(DWORD *)(dwHSCRC5_Table + i*8) = ((*(DWORD *)(dwHSCRC5_Table + i*8)^*(DWORD *)(dwHSCRC5_Table + 0x24)) - HS_Memory_Start + HS_Memory)^(*(DWORD *)(dwHSCRC5_Table + 0x24));
	}
}

void _stdcall _HSLog(DWORD dwReturn, char *Buffer){
	FILE *fp;
	char FileName[256];
	DWORD pid = GetProcessId(GetCurrentProcess());
	sprintf(FileName, "Riremito_%04X.txt", pid);
	fp = fopen(FileName, "a");
	fprintf(fp, "EHSvc.dll+%X:%s\r\n", dwReturn-HS_Memory_Start, Buffer);
	fclose(fp);
}
//secrect code
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
//

void int3bp_Bypass(){
	DWORD old;
	DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "DbgBreakPoint");

	VirtualProtect((DWORD *)dwAddr, 1, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwAddr) = 0xCC;
}

DWORD MODOR2(DWORD dwKey, BYTE Output[], DWORD *dwUnknown){
	WORD *Next = (WORD *)(&Output[8]);
	*(DWORD *)(&Output[*Next]) = ~((0x0ACA3DBE & 0xFFFF)* (dwKey & 0xFFFF)) % 0xA8CFA835;//false
	*Next += 4;
	*(DWORD *)(&Output[*Next]) = ~((0x0ACA3DBE >> 16)* (dwKey & 0xFFFF)) % 0xA8CFA835;//true
	*Next += 4;
	*dwUnknown += 4;//wtf
	*(WORD *)&Output[0x258] = *Next;
	return 0;
}

void HackShieldBypass(){
	BW.AddString(EDIT_LOG, "Waiting for loading hackshield...\r\n");

	HM.Init("EHSvc.dll");

	HM.CreateMemoryDump();
	HM.GetDumpInfo(&HS_Memory_Start, &HS_Memory_End, &HS_Memory);

	HM.WriteHook(0x92812, JMP, HSCRC1_Hook, &HSCRC1_Ret, 2);//HSCRC1
	HM.WriteHook(0x360040, JMP, HSCRC2_Hook, &HSCRC2_Ret);//HSCRC2
	HM.WriteHook(0x26005E+2, JMP, HSCRC3_Hook);//HSCRC3
	HSCRC3_Ret = HS_Memory_Start + 0x2528EB;
	HM.WriteHook(0x39ED7, JMP, HSCRC4_Hook, &HSCRC4_Ret, 3);//HSCRC4
	HSCRC5_TableHack(HS_Memory_Start + 0x157048);

	HM.MemoryWriter(0x4F5B0, "31 C0 C2 04 00");//Process Scanner
	HM.MemoryWriter(0x56380, "31 C0 C2 04 00");//Module Scanner

	HM.MemoryWriter(0x10E20, "31 C0 C3");//HardwareBreakPoint Detection(Main)
	HM.MemoryWriter(0xF550, "31 C0 C3");//HardwareBreakPoint Detection2
	HM.MemoryWriter(0xF740, "31 C0 C3");//HardwareBreakPoint Detection3
	HM.MemoryWriter(0xFED0, "31 C0 C2 18 00");//HardwareBreakPoint Detection4
	
	HM.MemoryWriter(0x70140, "31 C0 C3");//SoftwareBreakPoint Detection
	HM.MemoryWriter(0xCEB67, "B8 00 00 00 00");//Memory Protection

	HM.MemoryWriter(0xA0D8A, "8B 85 E0 FC FF FF");//Load Original DLL
	HM.MemoryWriter(0x92FDA, "B8 01 00 00 00 C2 0C 00");//SC Module Check

	HM.MemoryWriter(0x4A10, "31 C0 C2 04 00");//restore ntdll
	
	HM.WriteHook(0x18371, JMP, HSLog, &HSLog_Ret);//HSLog

	HM.MemoryWriter(0x1AAD0, "31 C0 C3");//SelfCRC for allocated memory
	HM.MemoryWriter(0x930A1, "B8 01 00 00 00 C2 0C 00");//Disable Module CRC (a part of HSCRC1)
	//HM.MemoryWriter(0x1E2A0, "31 C0 C2 04 00");//Disable HSCRC crash or error or exception fuck

	HM.WriteHook(0xC26F1, JMP, (void (*)())MODOR2);

	/* this code is for logging thread
	DWORD old;
	DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");

	VirtualProtect((DWORD *)dwAddr, 5, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwAddr) = 0xE9;
	*(DWORD *)(dwAddr + 1) = (DWORD)EternalSleep - dwAddr - 5;

	VirtualProtect((DWORD *)dwAddr, 5, PAGE_EXECUTE, &old);
	*/

	//keeege
	BW.AddString(EDIT_LOG, "most detections are disabled now!\r\n");

	HM.MemoryWriter(0x181A0, "31 C0 C3");//Disable HS Log
	//AM.RestoreMemory(HS_Init, 5);
	HS_Init = 0xDEADBEEF;

	int3bp_Bypass();

}

///
