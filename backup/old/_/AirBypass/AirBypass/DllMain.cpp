#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

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

unsigned char *Memory2;
DWORD EHSvc_Start = 0;
DWORD EHSvc_End = 0;

void MemoryDump2(LPCSTR lpModuleName){
	MODULEINFO hInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(lpModuleName), &hInfo, sizeof(MODULEINFO));
	Memory2= (unsigned char *)malloc(hInfo.SizeOfImage);
	RtlCopyMemory(Memory2, (DWORD *)hInfo.lpBaseOfDll, hInfo.SizeOfImage);
	EHSvc_Start = (DWORD)hInfo.lpBaseOfDll;
	EHSvc_End = EHSvc_Start+hInfo.SizeOfImage;
}

DWORD EHSvc_92812 = 0x92812;
DWORD EHSvc_360040 = 0x360040;
DWORD EHSvc_26005E = 0x26005E+2;
DWORD EHSvc_92819 = 0x92819;
DWORD EHSvc_360045 = 0x360045;
DWORD EHSvc_2528EB = 0x2528EB;

void _declspec(naked) HSCRC1(){
	_asm{
		push ecx
		cmp ecx,[EHSvc_Start]
		jb Ending1
		cmp ecx,[EHSvc_End]
		ja Ending1
		sub ecx,[EHSvc_Start]
		add ecx,Memory2
Ending1:
		mov dl,[ecx]
		pop ecx
		xor eax,edx
		mov ecx,[ebp+0x10]
		jmp dword ptr [EHSvc_92819]
	}
}


void _declspec(naked) HSCRC2(){
	_asm{
		push ebx
		cmp ebx,[EHSvc_Start]
		jb Ending2
		cmp ebx,[EHSvc_End]
		ja Ending2
		sub ebx,[EHSvc_Start]
		add ebx,Memory2
Ending2:
		add al,[ebx]
		pop ebx
		mov ebx,[esp]
		jmp dword ptr [EHSvc_360045]
	}
}


void _declspec(naked) HSCRC3(){
	_asm{
		push eax
		mov eax,[EHSvc_92812]
		sub eax,0x100
		cmp edx,eax
		jb Ending3
		add eax,0x200
		cmp edx,eax
		jb Modify3
		mov eax,[EHSvc_360040]
		sub eax,0x100
		cmp edx,eax
		jb Ending3
		add eax,0x200
		cmp edx,eax
		ja Ending3
Modify3:
		mov eax,edx
		sub eax,[EHSvc_Start]
		add eax,Memory2
		mov eax,[eax]
		mov dword ptr [esp+0x04],eax
Ending3:
		pop eax
		jmp dword ptr [EHSvc_2528EB]
	}
}

DWORD EHSvc_39ED7 = 0x39ED7;
DWORD EHSvc_39EDF = 0x39EDF;

void _declspec(naked) HSCRC(){
	_asm{
		push esi
		cmp esi,[dwMemoryStart]
		jb Ending_
		cmp esi,[dwMemoryEnd]
		ja Ending_
		sub esi,[dwMemoryStart]
		add esi,Memory
Ending_:
		lea edi,[ebp-0x1228]
		repe movsd
		pop esi
		jmp dword ptr [EHSvc_39EDF]
	}
}

DWORD EHSvc_145F0 = 0x145F0;
DWORD EHsvc_157048 = 0x157048;
DWORD EHSvc_927D8 = 0x927D8;//HSCRC1
DWORD EHSvc_9D02C = 0x9D02C;
DWORD EHSvc_92AED = 0x92AED;
DWORD EHSvc_5BF30 = 0x5BF30;

void _declspec(naked) SpoofTable(){
	_asm{
		cmp esi,[EHSvc_927D8]
		je SpoofPLZ
		cmp esi,[EHSvc_9D02C]
		je SpoofPLZ
		cmp esi,[EHSvc_92AED]
		je SpoofPLZ
		cmp esi,[EHSvc_5BF30]
		je SpoofPLZ
		jmp Ending_ST
SpoofPLZ:
		sub esi,[EHSvc_Start]
		add esi,Memory2
Ending_ST:
		ret
	}
}
void _declspec(naked) EncryptDecryptHook(){
	_asm{
		mov eax,[esp+0x08]
		mov ecx,[esp+0x0C]
		cmp eax,ecx
		jae Label2
		mov edx,[esp+0x04]
		push esi
Label1:
		mov esi,[eax]
		add eax,0x04
		xor esi,edx
		call SpoofTable
		cmp eax,ecx
		mov [eax-0x04],esi
		jb Label1
Label2:
		pop esi
		ret
	}
}

DWORD WaitForHS(){
	HMODULE EHSvc = 0;

	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}

	return (DWORD)EHSvc;
}

void HackShieldBypass(){

	MSCRCBypass();
	RemoveThemidaCRCs();
	VEHBypass();
	StopProcessListPacket();
	InternalDetection();

	DWORD EHSvc = WaitForHS();
	Sleep(10);
	MemoryDump2("EHSvc.dll");
	EHSvc_92812 += EHSvc;
	EHSvc_360040 += EHSvc;
	EHSvc_26005E += EHSvc;
	EHSvc_92819 += EHSvc;
	EHSvc_360045 += EHSvc;
	EHSvc_2528EB += EHSvc;
	EHSvc_39ED7 += EHSvc;
	EHSvc_39EDF += EHSvc;
	EHSvc_145F0 += EHSvc;
	EHsvc_157048 += EHSvc;
	EHSvc_927D8 += EHSvc;
	EHSvc_9D02C += EHSvc;
	EHSvc_92AED += EHSvc;
	EHSvc_5BF30 += EHSvc;

	Write_Hook("jmp", EHSvc_92812, (DWORD)HSCRC1);
	Write_Hook("jmp", EHSvc_360040, (DWORD)HSCRC2);
	Write_Hook("jmp", EHSvc_26005E, (DWORD)HSCRC3);
	Write_Hook("jmp", EHSvc_145F0, (DWORD)EncryptDecryptHook);
	Write_code(EHSvc+0x85C0, "31 C0 C2 0C 00");//ProcessScan
	Write_code(EHSvc+0x85E0, "31 C0 C2 10 00");//ModuleScan
	Write_code(EHSvc+0x70140, "31 C0 C3");//SelfMemoryScan

	Write_Hook("jmp", EHSvc_39ED7, (DWORD)HSCRC);

	MessageBoxA(NULL, "Bypass is installed!", "AirBypass v102.1", NULL);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}
	//EHSvcMemory = LoadLibraryA("HShield/Memory.dll");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackShieldBypass, hinstDLL, NULL, NULL);

	return TRUE;
}