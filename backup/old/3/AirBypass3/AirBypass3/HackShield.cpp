#include"HackShield.h"
#include"MapleStory.h"
DWORD EHSvc_Start, EHSvc_End, EHSvc_Memory;

DWORD HSCRC1_Ret;
void _declspec(naked) HSCRC1Hook(){
	_asm{
		cmp ecx,[EHSvc_Start]
		jb HSCRC1Hook_End
		cmp ecx,[EHSvc_End]
		ja HSCRC1Hook_End
		sub ecx,[EHSvc_Start]
		add ecx,[EHSvc_Memory]
HSCRC1Hook_End:
		mov dl,[ecx]
		xor eax,edx
		mov ecx,[ebp+0x10]
		jmp dword ptr [HSCRC1_Ret]
	}
}

DWORD HSCRC2_Ret;
void _declspec(naked) HSCRC2Hook(){
	_asm{
		cmp ecx,[EHSvc_Start]
		jb HSCRC2Hook_End
		cmp ecx,[EHSvc_End]
		ja HSCRC2Hook_End
		sub ecx,[EHSvc_Start]
		add ecx,[EHSvc_Memory]
HSCRC2Hook_End:
		add al,[ecx]
		pop ecx
		push ebx
		mov bl,0x33
		jmp dword ptr [HSCRC2_Ret]
	}
}

DWORD HSCRC3_Ret;
void _declspec(naked) HSCRC3Hook(){
	_asm{
		cmp edx,[EHSvc_Start]
		jb HSCRC3Hook_End
		cmp edx,[EHSvc_End]
		ja HSCRC3Hook_End
		push edx
		sub edx,[EHSvc_Start]
		cmp edx,0x9A342 - 0x100
		jb HSCRC3Hook_End_2
		cmp edx,0x373BD0 + 0x100
		ja HSCRC3Hook_End_2
		cmp edx,0x9A342 + 0x100
		jb HSCRC3Hook_End_1
		cmp edx,0x373BD0 - 0x100
		ja HSCRC3Hook_End_1
		jmp HSCRC3Hook_End_2
HSCRC3Hook_End_1:
		add edx,[EHSvc_Memory]
		mov edx,[edx]
		mov dword ptr [esp+0x04],edx
HSCRC3Hook_End_2:
		pop edx
HSCRC3Hook_End:
		jmp dword ptr [HSCRC3_Ret]
	}
}

DWORD HSCRC4_Ret;
void _declspec(naked) HSCRC4Hook(){
	_asm{
		push esi
		cmp esi,[Memory_Start]
		jb HSCRC4Hook_End
		cmp esi,[Memory_End]
		ja HSCRC4Hook_End
		sub esi,[Memory_Start]
		add esi,[Memory]
HSCRC4Hook_End:
		lea edi,[ebp-0x1228]
		repe movsd
		pop esi
		jmp dword ptr [HSCRC4_Ret]
	}
}

void FullAccess(DWORD Address, DWORD Size){
	DWORD OldProtect;
	VirtualProtect((void *)Address, Size, PAGE_EXECUTE_READWRITE, &OldProtect);
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

DWORD MODOR2(DWORD dwKey, BYTE Output[], DWORD *dwUnknown){
	WORD *Next = (WORD *)(&Output[8]);
	*(DWORD *)(&Output[*Next]) = ~((0xACA61AE & 0xFFFF)* (dwKey & 0xFFFF)) % 0xA8CFA835;//false
	*Next += 4;
	*(DWORD *)(&Output[*Next]) = ~((0xACA61AE >> 16)* (dwKey & 0xFFFF)) % 0xA8CFA835;//true
	*Next += 4;
	*dwUnknown += 4;//wtf
	*(WORD *)&Output[0x258] = *Next;
	return 0;
}

AirMemory HS;

void HackShieldBypass(){
	LoadLibraryA("HShield/EHSvc.dll");
	HS.Init("EHSvc.dll");
	HS.CreateMemoryDump();
	HS.GetDumpInfo(&EHSvc_Start, &EHSvc_End, &EHSvc_Memory);
	
	HSCRC5_TableHack2(EHSvc_Start + 0x162048, EHSvc_Start + 0x500, 0x500);
	
	HS.WriteHook(0x9A342, JMP, HSCRC1Hook, &HSCRC1_Ret, 2);
	HS.WriteHook(0x373BD0, JMP, HSCRC2Hook, &HSCRC2_Ret, 1);
	HS.WriteHook(0x26E13A, JMP, HSCRC3Hook);
	HSCRC3_Ret = EHSvc_Start + 0x26EF26;
	HS.WriteHook(0x40277, JMP, HSCRC4Hook, &HSCRC4_Ret, 3);
	
	HS.MemoryWriter(0x56200, "31 C0 C2 04 00");//Process Scanner
	HS.MemoryWriter(0x5D0A0, "31 C0 C2 04 00");//Module Scanner
	HS.MemoryWriter(0x11280, "31 C0 C3");//HardwareBreakPoint Detection(Main) Unchanged
	HS.MemoryWriter(0xF870, "31 C0 C3");//HardwareBreakPoint Detection2 Unchanged
	HS.MemoryWriter(0xFA60, "31 C0 C3");//HardwareBreakPoint Detection3 Unchanged
	HS.MemoryWriter(0x101F0, "31 C0 C2 18 00");//HardwareBreakPoint Detection4 Unchanged
	HS.MemoryWriter(0x770E0, "31 C0 C3");//SoftwareBreakPoint Detection
	HS.MemoryWriter(0xD88D6, "B8 00 00 00 00");//Memory Protection
	
	HS.WriteHook(0xC39E0, JMP, (void (*)())MODOR2);

	HS.MemoryWriter(0x19000, "31 C0 C3");//HS Log
}