/*
	HackShield Bypass v5.7.6.502 (Memory Edit Version)
	Created by AIRRIDE
*/

#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

unsigned char *EHSvcMemory;
unsigned char *MapleStoryMemory;

DWORD EHSvc_Start = 0;
DWORD EHSvc_End = 0;

DWORD HSCRC1 = 0x92812;//For EHSvc's Memory(Easy to Find)
DWORD HSCRC1_Return = HSCRC1 + 0x07;
DWORD HSCRC2 = 0x360040;//For EHSvc's Memory(Easy to Find)
DWORD HSCRC2_Return = HSCRC2 + 0x05;
DWORD HSCRC3 = 0x26005E + 0x02;//For EHSvc's Memory and Temporary Memory(Easy to Find)
DWORD HSCRC3_Return = 0x2528EB;
DWORD HSCRC4 = 0x39ED7;//For MapleStory's Memory(Easy to Find)
DWORD HSCRC5_Table = 0x157048;//For EHSvc's Memory(Hard to Find)

DWORD ProcessScan = 0x4F5B0;
DWORD ModuleScan = 0x56380;
DWORD SelfMemoryScan = 0x70140;
DWORD MemoryProtection = 0xCEB67;
DWORD HardwareBreakPointDetection1 = 0xF550;
DWORD HardwareBreakPointDetection2 = 0xF740;
DWORD HardwareBreakPointDetection3 = 0xFED0;
DWORD HardwareBreakPointDetection4 = 0x10E20;

DWORD UnknownScan1 = 0x56400;
DWORD UnknownScan2 = 0x54800;
DWORD UnknownScan3 = 0x56220;
DWORD UnknownScan4 = 0x549E0;


void Init(){
	HSCRC1 += EHSvc_Start;
	HSCRC1_Return += EHSvc_Start;

	HSCRC2 += EHSvc_Start;
	HSCRC2_Return += EHSvc_Start;

	HSCRC3 += EHSvc_Start;
	HSCRC3_Return += EHSvc_Start;

	HSCRC4 += EHSvc_Start;
	HSCRC5_Table += EHSvc_Start;

	ProcessScan += EHSvc_Start;
	ModuleScan += EHSvc_Start;
	SelfMemoryScan += EHSvc_Start;
	MemoryProtection += EHSvc_Start;

	HardwareBreakPointDetection1 += EHSvc_Start;
	HardwareBreakPointDetection2 += EHSvc_Start;
	HardwareBreakPointDetection3 += EHSvc_Start;
	HardwareBreakPointDetection4 += EHSvc_Start;

	UnknownScan1 += EHSvc_Start;
	UnknownScan2 += EHSvc_Start;
	UnknownScan3 += EHSvc_Start;
	UnknownScan4 += EHSvc_Start;
}

void _declspec(naked) HSCRC1_Hook(){
	_asm{
		push ecx
		cmp ecx,[EHSvc_Start]
		jb Ending1
		cmp ecx,[EHSvc_End]
		ja Ending1
		sub ecx,[EHSvc_Start]
		add ecx,EHSvcMemory
Ending1:
		mov dl,[ecx]
		pop ecx
		xor eax,edx
		mov ecx,[ebp+0x10]
		jmp dword ptr [HSCRC1_Return]
	}
}


void _declspec(naked) HSCRC2_Hook(){
	_asm{
		push ebx
		cmp ebx,[EHSvc_Start]
		jb Ending2
		cmp ebx,[EHSvc_End]
		ja Ending2
		sub ebx,[EHSvc_Start]
		add ebx,EHSvcMemory
Ending2:
		add al,[ebx]
		pop ebx
		mov ebx,[esp]
		jmp dword ptr [HSCRC2_Return]
	}
}


void _declspec(naked) HSCRC3_Hook(){
	_asm{
		push eax
		mov eax,[HSCRC1]
		sub eax,0x100
		cmp edx,eax
		jb Ending3
		add eax,0x200
		cmp edx,eax
		jb Modify3
		mov eax,[HSCRC2]
		sub eax,0x100
		cmp edx,eax
		jb Ending3
		add eax,0x200
		cmp edx,eax
		ja Ending3
Modify3:
		mov eax,edx
		sub eax,[EHSvc_Start]
		add eax,EHSvcMemory
		mov eax,[eax]
		mov dword ptr [esp+0x04],eax
Ending3:
		pop eax
		jmp dword ptr [HSCRC3_Return]
	}
}

void HSCRC5_Hook(){
	DWORD *HSCRC_DC_Table = (DWORD *)HSCRC5_Table;
	DWORD i;

	for(i=0; i<4; i++){
		HSCRC_DC_Table[i*2] = ((HSCRC_DC_Table[i*2]^HSCRC_DC_Table[11])-EHSvc_Start+(DWORD)EHSvcMemory)^HSCRC_DC_Table[11];
	}

}

void FullAccess(DWORD dwAddr, DWORD dwSize){
	DWORD old;
	VirtualProtect((DWORD *)dwAddr, dwSize, PAGE_EXECUTE_READWRITE, &old);
}

void WriteJMP(DWORD dwPrev, DWORD dwNext){
	FullAccess(dwPrev, 5);
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext-dwPrev-5;
}

char StrArray[0x20] = "0123456789ABCDEF";

int ChartoByte(char Array[], BYTE b[]){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x2A){Array[i] = StrArray[rand()%0x10];}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}

BOOL WriteCODE(DWORD dwAddr, char Array[], int nop_count = 0){
	int i, Hit;
	BYTE ByteCode[0x100];//

	if(Array != NULL && Array[0] != 0x00){


		Hit = ChartoByte(Array, ByteCode);
		FullAccess(dwAddr, Hit + nop_count);

		for(i=0; i<Hit; i++){
			*(BYTE *)(dwAddr + i) = ByteCode[i];
		}

		if(nop_count == 0){
			return TRUE;
		}

	}else{
		Hit = 0;
	}

	for(i=0; i<nop_count; i++){
		*(BYTE *)(dwAddr + Hit + i) = 0x90;//nop
	}


	return TRUE;
}

void AirBypass(){
	HMODULE hEHSvc = 0;
	MODULEINFO EHSvcInfo;

	while(!hEHSvc){
		hEHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}

	GetModuleInformation(GetCurrentProcess(), hEHSvc, &EHSvcInfo, sizeof(MODULEINFO));
	EHSvcMemory = (unsigned char *)malloc(EHSvcInfo.SizeOfImage);
	RtlCopyMemory(EHSvcMemory, EHSvcInfo.lpBaseOfDll, EHSvcInfo.SizeOfImage);
	EHSvc_Start = (DWORD)EHSvcInfo.lpBaseOfDll;
	EHSvc_End = EHSvc_Start + EHSvcInfo.SizeOfImage;

	Init();

	WriteJMP(HSCRC1, (DWORD)HSCRC1_Hook);
	WriteJMP(HSCRC2, (DWORD)HSCRC2_Hook);
	WriteJMP(HSCRC3, (DWORD)HSCRC3_Hook);
	HSCRC5_Hook();

	WriteCODE(ProcessScan, "31 C0 C2 04 00");
	WriteCODE(ModuleScan, "31 C0 C2 04 00");
	WriteCODE(SelfMemoryScan, "31 C0 C3");
	WriteCODE(MemoryProtection, "B8 00 00 00 00");

	WriteCODE(HardwareBreakPointDetection1, "31 C0 C3");
	WriteCODE(HardwareBreakPointDetection2, "31 C0 C3");
	WriteCODE(HardwareBreakPointDetection3, "31 C0 C2 18 00");
	WriteCODE(HardwareBreakPointDetection4, "31 C0 C3");
	
	
	WriteCODE(UnknownScan1, "31 C0 C2 08 00");
	WriteCODE(UnknownScan2, "31 C0 C3");
	WriteCODE(UnknownScan3, "31 C0 C3");
	WriteCODE(UnknownScan4, "31 C0 C3");

	MessageBoxA(NULL, "Installed", "AirBypass", NULL);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AirBypass, NULL, NULL, NULL);

	return TRUE;
}