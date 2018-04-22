#include<Windows.h>

#include<psapi.h>
#pragma comment(lib, "psapi.lib")
//w
char StrArray[128] = "0123456789ABCDEF";

BOOL Enable_Write_Memory(DWORD Addr, int Size){
	DWORD old;
	if(VirtualProtect((DWORD *)Addr, Size, PAGE_EXECUTE_READWRITE, &old) == FALSE){
		return FALSE;
	}

	return TRUE;
}


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

BOOL Write_code(DWORD Addr, char Array[], int nop_count = 0){
	int i, Hit;
	BYTE ByteCode[0x100];//

	if(Array != NULL && Array[0] != 0x00){


	Hit = ChartoByte(Array, ByteCode);

	if(Enable_Write_Memory(Addr, Hit) == FALSE){
		return FALSE;
	}

	for(i=0; i<Hit; i++){
		*(BYTE *)(Addr + i) = ByteCode[i];
	}

	if(nop_count == 0){
		return TRUE;
	}

	if(Enable_Write_Memory(Addr + Hit, nop_count) == FALSE){
		return FALSE;
	}

	}else{
		Hit = 0;
	}

	for(i=0; i<nop_count; i++){
		*(BYTE *)(Addr + Hit + i) = 0x90;//nop
	}


	return TRUE;
}

DWORD Write_Hook(char code[], DWORD Prev, DWORD Next, int nop_count = 0){
	int i;
	BOOL Flag = FALSE;

	if(Enable_Write_Memory(Prev, 7 + nop_count) == FALSE){
		return FALSE;
	}

	switch(*(DWORD *)code){
		case 0x00706D6A://jmp
			*(BYTE *)Prev = 0xE9;
			break;

		case 0x6C6C6163://call
			*(BYTE *)Prev = 0xE8;
			break;

		case 0x0000656A://je
			*(WORD *)Prev = 0x840F;
			Flag = TRUE;
			break;

		case 0x00656E6A://jne
			*(WORD *)Prev = 0x850F;
			Flag = TRUE;
			break;
			
		case 0x0000626A://jb
			*(WORD *)Prev = 0x820F;
			Flag = TRUE;
			break;

		case 0x0000616A://ja
			*(WORD *)Prev = 0x870F;
			Flag = TRUE;
			break;

		default:
			//ErrorMessage("@Write_Hook");
			return FALSE;
			break;
	}

	*(DWORD *)(Prev + 1 + Flag) = Next - Prev - 5 - Flag;

	if(nop_count == 0){
		return Prev + 5 + Flag;
	}

	for(i=0; i < nop_count; i++){
		*(BYTE *)(Prev + 5 + Flag + i) = 0x90;//nop
	}

	return Prev + 5 + nop_count + Flag;
}


int ChartoAob(char Array[], BYTE b[], BOOL m[]){
   int i,Hit=0;
   for(i=0; Array[i]!=0x00; i++){
     if(Array[i] == 0x3F){b[Hit]=0x00; m[Hit/2] = TRUE; Hit++;continue;}
     if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
     else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
     else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
   }

   if(Hit%2!=0){
     //ErrorMessage("@ChartoByte");
     return -1;
   }

   Hit=Hit/2;

   for(i=0; i<Hit; i++){
     b[i]=b[i*2]*0x10+b[i*2+1];
   }

   return Hit;
}


DWORD AobScan(char Array[], char code[] = NULL, DWORD Memory_Start = 0x00400000, DWORD Memory_End = 0x02000000){
   BYTE ByteCode[0x100];
   BOOL Mask[0x100] = {0};
   int hit, i;
   DWORD MS_Memory;
   hit = ChartoAob(Array, ByteCode, Mask);

   __try{

   for(MS_Memory = Memory_Start; MS_Memory < Memory_End; MS_Memory++){

     for(i=0; i<hit; i++){
       if(Mask[i] == TRUE){
         continue;
       }
         if(*(BYTE *)(MS_Memory + i)^ByteCode[i]){
           break;
         }
     }
     if(i == hit){
		 if(code == NULL){
			return MS_Memory;
		 }
			switch(*(DWORD *)code){
				case 0x00706D6A://jmp XXXXXXXX
				case 0x6C6C6163://call XXXXXXXX
					return *(DWORD *)(MS_Memory + 1) + MS_Memory + 5;

				case 0x0000656A://je XXXXXXXX
				case 0x00656E6A://jne XXXXXXXX
				case 0x0000626A://jb XXXXXXXX
				case 0x0000616A://ja XXXXXXXX
					return *(DWORD *)(MS_Memory + 2) + MS_Memory + 6;

				//mov eax,[XXXXXXXX]...
				case 0x00786165://eax
					return *(DWORD *)(MS_Memory + 1);
					
				//mov ecx,[XXXXXXXX]...
				case 0x00786265://ebx
				case 0x00786365://ecx
				case 0x00786465://ecx
				case 0x00697365://esi
				case 0x00496465://edi
					return *(DWORD *)(MS_Memory + 2);
			}
     }
   }

   }__except(EXCEPTION_EXECUTE_HANDLER){
     return 0;
   }

   return 0;
}
//

unsigned char *Memory;
DWORD dwMemoryStart;
DWORD dwMemoryEnd;
DWORD dwCRCMainStart;
DWORD dwCRCMainEnd;

void MemoryDump(){
	IMAGE_NT_HEADERS *nth = (IMAGE_NT_HEADERS *)((DWORD)GetModuleHandleA(NULL) + PIMAGE_DOS_HEADER(GetModuleHandleA(NULL))->e_lfanew);
	Memory = (unsigned char *)malloc(nth->OptionalHeader.SizeOfImage);
	RtlCopyMemory(Memory, (DWORD *)nth->OptionalHeader.ImageBase, nth->OptionalHeader.SizeOfImage);
	dwMemoryStart = (DWORD)nth->OptionalHeader.ImageBase;
	dwMemoryEnd = dwMemoryStart+(DWORD)nth->OptionalHeader.SizeOfImage;
}

void _declspec(naked) CRCMainHook(){
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
		cmp ecx,[dwMemoryStart]
		jb Ending
		cmp ecx,[dwMemoryEnd]
		ja Ending
		sub ecx,[dwMemoryStart]
		add ecx,Memory
Ending:
		jmp dwCRCMainEnd
	}
}


bool MSCRCBypass(){
	//dwCRCMainStart = AobScan("0F 85 ?? ?? ?? ?? 6A 00 E9");
	dwCRCMainStart = AobScan("3B 8D 70 FF FF FF 0F 85 ?? ?? ?? ?? 6A 00 E9")+6;
	dwCRCMainEnd = AobScan("8A 11 80 C2 01 8B 4D 18");

	if(!dwCRCMainStart || !dwCRCMainEnd) return false;

	MemoryDump();
	Write_Hook("jne", dwCRCMainStart, (DWORD)CRCMainHook);
	return true;
}

bool RemoveThemidaCRCs(){
	DWORD dwThemidaCRCs = AobScan("55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? 53 56 57 A1 ?? ?? ?? ?? 33 C5 50 8D 45 F4 64 A3 00 00 00 00 89 8D ?? ?? ?? ?? 6A 01 8B 8D ?? ?? ?? ?? E8");

	if(!dwThemidaCRCs) return false;

	Write_code(dwThemidaCRCs, "33 C0 C3");
	return true;
}

bool VEHBypass(){
	DWORD dwVEH = AobScan("55 8B EC 81 EC F0 02 00 00 A1 ?? ?? ?? ?? 33 C5 89 45 FC 53 56 57 6A 00 E9");

	if(!dwVEH) return false;

	Write_code(dwVEH, "33 C0 C3");
	return true;
}

bool StopProcessListPacket(){
	DWORD dwPL = AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 53 55 56 57 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 ?? ?? ?? ?? 8B AC 24 ?? ?? ?? ?? 33 DB 53 8B F9 E9");
	
	if(!dwPL) return false;

	Write_code(dwPL, "33 C0 C2 04 00");
	return true;
}

bool KillProcesses(){
	DWORD dwASPLunchr = AobScan("83 EC 0C 56 57 6A 00 8B F1 E9");
	DWORD mkd25tray = AobScan("83 EC 0C 56 57 6A 00 8B F1 E9", NULL, dwASPLunchr);
	DWORD dwAutoup = AobScan("56 8D 71 2C 8B CE E8");

	if(!dwASPLunchr || !mkd25tray || !dwAutoup) return false;

	Write_code(dwASPLunchr, "33 C0 C3");
	Write_code(mkd25tray, "33 C0 C3");
	Write_code(dwAutoup , "33 C0 C3");

	return true;

}

bool HSless(){
	DWORD dwHSless1 = AobScan("81 EC 20 01 00 00 A1 ?? ?? ?? ?? 33 C4 89 84 24 1C 01 00 00 55 56 6A 00 8B F1 E9");
	DWORD dwHSless2 = AobScan("6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 81 EC ?? ?? ?? ?? A1 ?? ?? ?? ?? 33 C4 89 84 24 ?? ?? ?? ?? 56 A1 ?? ?? ?? ?? 33 C4 50 8D 84 24 ?? ?? ?? ?? 64 A3 00 00 00 00 8B B4 24 ?? ?? ?? ?? 68 92 01 00 00");
	
	if(!dwHSless1 || !dwHSless2) return false;
	
	Write_code(dwHSless1 , "33 C0 C2 04 00");
	Write_code(dwHSless2 , "33 C0 C2 04 00");

	return true;
}

bool InternalDetection(){
	DWORD dwInternal = AobScan("83 EC 08 56 8B F1 8D 4E 20 E8");

	if(!dwInternal) return false;
	
	Write_code(dwInternal, "C3");

	return true;
}
//




//v5.7.6.502 (EMS)
DWORD EHSvc_Start = 0;
DWORD EHSvc_End = 0;

DWORD WaitForHS(){
	HMODULE EHSvc = 0;
	MODULEINFO EHSvcInfo;//, MSInfo;
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}
	EHSvc_Start = (DWORD)EHSvcInfo.lpBaseOfDll;
	EHSvc_End = EHSvc_Start + (DWORD)EHSvcInfo.SizeOfImage;
	return (DWORD)EHSvc;
}

DWORD dwDriverFlag;
void _declspec(naked) DriverHack(){
	_asm{
		mov eax,[dwDriverFlag]
		mov dword ptr [eax],0x101
		leave
		xor eax,eax
		ret
	}
}

DWORD EHSvc_B07E = 0xB07E;
DWORD EHSvc_4B72D = 0x4B72D;
DWORD EHSvc_124400_Mem;
DWORD EHSvc_124220_Mem;
DWORD EHSvc_48707 = 0x48707;


DWORD EHSvc_1241C4_Mem;
DWORD EHSvc_D6A27 = 0xD6A27;
DWORD EHSvc_12433C_Mem;
DWORD EHSvc_CE821 = 0xCE821;

void _declspec(naked) DetectionHook(){
	_asm{
		mov eax,[esp]
		cmp eax,[EHSvc_B07E]
		jne Ending_DH
		add dword ptr [esp],0x134
Ending_DH:
		jmp EHSvc_124400_Mem
	}
}

void _declspec(naked) CrashHook(){
	_asm{
		mov eax,[esp]
		cmp eax,[EHSvc_48707]
		jne Ending_CH
		mov eax,ExtraExit2
		mov [esp],eax
		mov eax,[ebp+0x04]
		cmp eax,[EHSvc_4B72D]
		jne Ending_CH
		mov eax,ExtraExit
		mov dword ptr [ebp+0x04],eax
Ending_CH:
		jmp EHSvc_124220_Mem
ExtraExit:
		leave
		xor eax,eax
		ret
ExtraExit2:
		leave
		xor eax,eax
		ret 0x0008
	}
}

void _declspec(naked) AssemblyHook(){
	_asm{
		mov eax,[esp]
		cmp eax,[EHSvc_D6A27]
		jne Ending_AH
		mov eax,ExitAH
		mov dword ptr [esp],eax
Ending_AH:
		jmp EHSvc_1241C4_Mem
ExitAH:
		leave
		xor eax,eax
		ret 0x0004
	}
}

void _declspec(naked) RejectDriver(){
	_asm{
		mov eax,[esp]
		cmp eax,[EHSvc_CE821]
		jne Ending_RD
		add dword ptr [esp],0x1C
		xor eax,eax
		ret 0x0020
Ending_RD:
		jmp EHSvc_12433C_Mem
	}
}

DWORD EHSvc_39F6A = 0x39F6A;

void _declspec(naked) HSCRC(){
	_asm{
		pushad
		mov esi,[ebp+0x08]//dwAddr
		sub esi,0x00400000
		add esi,Memory
		mov ecx,0x400//code size
		lea edi,[ebp-0x1228]
		repe movsd//memcpy
		popad
		jmp dword ptr [EHSvc_39F6A]
	}
}
#define ntdll_NtGetContextThread 0xCA
#define ntdll_NtQueryVirtualMemory 0x20
DWORD _stdcall SystemCallInspector2(DWORD dwEAX, DWORD *Argument){

	switch(dwEAX){
		case ntdll_NtGetContextThread:
			return 0;

		case ntdll_NtQueryVirtualMemory:
			if(EHSvc_39F6A && Argument[17] == EHSvc_39F6A){
				Argument[17] = (DWORD)HSCRC;
			}
			break;

		default:
			break;
	}

	return 0x1337;
}
DWORD _declspec(naked) GetSystemCallAddr(){
	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0xC0]
		ret
	}
}
void _declspec(naked) SystemCallReturn(){
	_asm{
		nop
		nop
		nop
		nop
		nop
		nop
		nop
	}
}
void _declspec(naked) IsCopiedDLL(){
	_asm{
		xor edi,edi
		cmp dword ptr [esp+0x04],0x12345678
		ja  ReturnCheck2
		inc edi
		jmp Ending_IO
 ReturnCheck2:
		cmp dword ptr [esp+0x04],0x87654321
		jb  Ending_IO
		inc edi
Ending_IO:
		ret 0x0004
	}
}

#define ntdll_ZwQueryInformationProcess 0x16
#define ntdll_NtProtectVirtualMemory 0x4D
#define ntdll_NtGetContextThread 0xCA
#define ntdll_NtMapViewOfSection 0x25
#define ntdll_NtProtectVirtualMemory 0x4D
//
#define ntdll_ZwQuerySystemInformation 0x33
#define ntdll_NtDeviceIoControlFile 0x04


bool IsEHsvc(DWORD dwAddr){
	if(!EHSvc_End){
		return false;
	}

	if(EHSvc_Start <= dwAddr && dwAddr <= EHSvc_End){
		return true;
	}
	return false;
}

DWORD _stdcall SystemCallInspector(DWORD dwEAX, DWORD *Argument){

	switch(dwEAX){
		case ntdll_ZwQuerySystemInformation://Process Detection1, comes from EnumProcesses
			if(Argument[0] == 0x05 && Argument[3] == 0x00 && IsEHsvc(Argument[20])){
				return 0;
			}
			break;

		case ntdll_ZwQueryInformationProcess://EnumProcessModules
			if(Argument[4] == 0x00 && Argument[3] == 0x18 && IsEHsvc(Argument[64])){
				return 0;
			}
			break;

		case ntdll_NtDeviceIoControlFile://Driver Loader, comes from DeviceIoControl
			if(Argument[5] == 0x80000040 && IsEHsvc(Argument[34])){
				return 0;
			}
			break;

		//case ntdll_NtGetContextThread://Hardware BreakPoint Detection
		//	if(IsEHsvc(Argument[-2])){
		//		return 0;
		//	}
		//	break;

		case ntdll_NtProtectVirtualMemory:
			break;
		
		//case ntdll_NtMapViewOfSection:
		//	if(IsEHsvc(Argument[13])){
		//		return 0;
		//	}
		//	break;
			

		default:
			break;
	}

	return 0x1337;
}
DWORD temp;
void _declspec(naked) SystemCallHook(){
	_asm{
		push edi
		cmp eax,0x1A3
		ja Ending
//Normal Section
		pushad
		push edx
		push eax
		call SystemCallInspector
		test eax,eax
		je Block0
		popad
//Copied Section
		push [esp+0x04]
		call IsCopiedDLL
		test edi,edi
		je Ending

		//lea edi,[esp+4]
		//pushad
		//push [edi]
		//push eax
		//call Logger
		//popad

		pushad
		push edx
		push eax
		call SystemCallInspector2
		test eax,eax
		je Block0
		popad
		
Ending:
		pop edi
		jmp SystemCallReturn
Block0:
		popad
		pop edi
		add dword ptr [esp],0x03
		xor eax,eax
		ret
Block1:
		popad
		pop edi
		add dword ptr [esp],0x03
		mov eax,1
		ret
Block334:
		popad
		pop edi
		add dword ptr [esp+0x04],0x21
		add dword ptr [esp],0x03
		xor eax,eax
		ret
	}
}


void HackShieldBypass(){
	HMODULE hNTDLL = GetModuleHandleA("ntdll.dll");
	MODULEINFO NTDLLInfo;
	GetModuleInformation((HANDLE)0xFFFFFFFF, hNTDLL, &NTDLLInfo, sizeof(MODULEINFO));
	DWORD dwSystemCall = NULL;
	DWORD old;
	DWORD EHSvc = WaitForHS();
	EHSvc_B07E += EHSvc;
	EHSvc_4B72D += EHSvc;
	EHSvc_48707 += EHSvc;
	EHSvc_D6A27 += EHSvc;
	EHSvc_CE821 += EHSvc;
	EHSvc_39F6A += EHSvc;
	EHSvc_124400_Mem = *(DWORD *)(EHSvc+0x124400);
	EHSvc_124220_Mem = *(DWORD *)(EHSvc+0x124220);
	*(DWORD *)(EHSvc+0x124400) = (DWORD)DetectionHook;
	*(DWORD *)(EHSvc+0x124220) = (DWORD)CrashHook;

	
	//EHSvc_12433C_Mem = *(DWORD *)(EHSvc+0x12433C);
	//*(DWORD *)(EHSvc+0x12433C) = (DWORD)RejectDriver;

	//EHSvc_1241C4_Mem = *(DWORD *)(EHSvc+0x1241C4);
	//*(DWORD *)(EHSvc+0x1241C4) = (DWORD)AssemblyHook;

	Write_code(EHSvc+0x546E85, "C9 31 C0 C3");//Callback

	//Write_code(EHSvc+0xB030, "31 C0 C2 04 00");//Detection
	//Write_code(EHSvc+0x4B670, "31 C0 C3");//Crash
	//Write_code(EHSvc+0x1E2A0, "31 C0 C2 04 00");
	//Write_code(EHSvc+0x1D640, "31 C0 C2 04 00");
	//Write_code(EHSvc+0x1DDE0, "31 C0 C2 04 00");
	//Write_code(EHSvc+0xCA9B0, "B8 06 00 00 00 C3");//Memory Protection
	//Write_code(EHSvc+0x39BD0, "31 C0 C2 0C 00");//Assembly
	//Write_code(EHSvc+0x9DE9D, "31 C0 C3");//callback1
	//Write_code(EHSvc+0xCE786, "31 C0 C3");//unk
	//Write_code(EHSvc+0xB0E95, "31 C0 C2 14 00");//Driver Callback
	//Write_Hook("jmp", EHSvc+0x3DEB08, (DWORD)DriverHack);//Main
	//dwDriverFlag = EHSvc+0x14E7EC;

	MSCRCBypass();
	RemoveThemidaCRCs();
	VEHBypass();
	StopProcessListPacket();
	InternalDetection();

	VirtualProtect((DWORD *)((DWORD)IsCopiedDLL+2+4), 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)((DWORD)IsCopiedDLL+2+4+4+2+1+2+4), 4, PAGE_EXECUTE_READWRITE, &old);
	
	*(DWORD *)((DWORD)IsCopiedDLL+2+4) = (DWORD)NTDLLInfo.lpBaseOfDll;
	*(DWORD *)((DWORD)IsCopiedDLL+2+4+4+2+1+2+4) = (DWORD)NTDLLInfo.lpBaseOfDll+(DWORD)NTDLLInfo.SizeOfImage;

	dwSystemCall = GetSystemCallAddr();

	VirtualProtect((DWORD *)dwSystemCall, 12, PAGE_EXECUTE_READWRITE, &old); 
	VirtualProtect((DWORD *)SystemCallReturn, 7, PAGE_EXECUTE_READWRITE, &old);
	memcpy((DWORD *)SystemCallReturn, (DWORD *)dwSystemCall, 7);
	
	*(BYTE *)(dwSystemCall+7) = 0xE9;
	*(DWORD *)(dwSystemCall+8) = (DWORD)SystemCallHook-(dwSystemCall+7)-5;
	*(WORD *)(dwSystemCall) = 0x05EB;//short jmp
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackShieldBypass, hinstDLL, NULL, NULL);

	return TRUE;
}