#include"AirBypass.h"

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

void HSCRC5_TableHack2(DWORD dwHSCRC5_Table, DWORD dwFreeSpace, DWORD dwSize){
	int i;
	DWORD *HSCRC5_Table = (DWORD *)dwHSCRC5_Table;
	DWORD DecryptKey = HSCRC5_Table[8];
	DWORD WrittenSize = 0;

	FullAccess(dwFreeSpace, dwSize);

	for(i=0; i<8; i++){//Decrypt
		HSCRC5_Table[i] ^= DecryptKey;
	}

	for(i=0; i<4; i++){
		RtlCopyMemory((void *)(dwFreeSpace + WrittenSize), (void *)HSCRC5_Table[i*2], HSCRC5_Table[i*2+1]);
		HSCRC5_Table[i*2] = dwFreeSpace + WrittenSize;
		WrittenSize += HSCRC5_Table[i*2+1];
	}
	
	for(i=0; i<8; i++){//Encrypt
		HSCRC5_Table[i] ^= DecryptKey;
	}
}

void int3bp_Bypass(){
	DWORD old;
	DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "DbgBreakPoint");

	VirtualProtect((DWORD *)dwAddr, 1, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE *)(dwAddr) = 0xCC;
}

AirMemory HM;

void HackShieldBypass(){
	LoadLibraryA("HShield/EHSvc.dll");
	HM.Init("EHSvc.dll");

	HM.CreateMemoryDump();
	HM.GetDumpInfo(&HS_Memory_Start, &HS_Memory_End, &HS_Memory);

	HSCRC5_TableHack2(HS_Memory_Start + 0x164048, HS_Memory_Start + 0x500, 0x500);
	HM.WriteHook(0x9B252, JMP, HSCRC1_Hook, &HSCRC1_Ret, 2);//HSCRC1
	HM.WriteHook(0x37C217, JMP, HSCRC2_Hook, &HSCRC2_Ret, 1);//HSCRC2
	HM.WriteHook(0x26B6B7, JMP, HSCRC3_Hook);//HSCRC3
	HSCRC3_Ret = HS_Memory_Start + 0x26AEC2;
	HM.WriteHook(0x408F7, JMP, HSCRC4_Hook, &HSCRC4_Ret, 3);//HSCRC4
	
	HM.MemoryWriter(0x56BF0, "31 C0 C2 04 00");//Process Scanner
	HM.MemoryWriter(0x5D8B0, "31 C0 C2 04 00");//Module Scanner

	HM.MemoryWriter(0x11280, "31 C0 C3");//HardwareBreakPoint Detection(Main)
	HM.MemoryWriter(0xF870, "31 C0 C3");//HardwareBreakPoint Detection2
	HM.MemoryWriter(0xFA60, "31 C0 C3");//HardwareBreakPoint Detection3
	HM.MemoryWriter(0x101F0, "31 C0 C2 18 00");//HardwareBreakPoint Detection4
	
	HM.MemoryWriter(0x779A0, "31 C0 C3");//SoftwareBreakPoint Detection
	HM.MemoryWriter(0xDA5CA, "B8 00 00 00 00");//Memory Protection

	int3bp_Bypass();
}