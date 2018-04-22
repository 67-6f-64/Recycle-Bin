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

void HackShieldBypass(){
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

	BW.AddString(EDIT_LOG, "most detections are disabled now!\r\n");
}