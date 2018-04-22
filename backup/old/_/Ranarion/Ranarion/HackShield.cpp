#include"HackShield.h"
#include<stdio.h>

#define WindowsUnknown 0
#define Windows7 7
#define Windows8 8

void WriteByte(BYTE temp, PDWORD CurrentAddr){
	*(BYTE *)CurrentAddr[0] = temp;
	CurrentAddr[0]+=1;
}

void WriteWord(WORD temp, PDWORD CurrentAddr){
	*(WORD *)CurrentAddr[0] = temp;
	CurrentAddr[0]+=2;
}

void WriteDword(DWORD temp, PDWORD CurrentAddr){
	*(DWORD *)CurrentAddr[0] = temp;
	CurrentAddr[0]+=4;
}

void WriteCall(DWORD Addr, PDWORD CurrentAddr){
	*(BYTE *)CurrentAddr[0] = 0xE8;
	*(DWORD *)(CurrentAddr[0]+1) = Addr-CurrentAddr[0]-5;
	CurrentAddr[0]+=5;
}

void DupeMemory(DWORD src, DWORD dwSize, PDWORD CurrentAddr){
	memcpy((DWORD *)CurrentAddr[0], (DWORD *)src, dwSize);
	CurrentAddr[0]+=dwSize;
}

char whitelist[4][128] = {"wow64win.dll", "wow64.dll", "wow64cpu.dll", "apisetschema.dll"};
char temp[512];
bool _stdcall DLLChecker(char Buffer[]){
	int i, slash, j;
	ZeroMemory(temp, 512);

	for(i=0; Buffer[i]; i+=2){
		if(Buffer[i] == '/' || Buffer[i] == '\\'){
			slash = i+2;
		}
	}

	i=0;
	for(; Buffer[slash]; slash+=2){
		temp[i] = Buffer[slash];
		i++;
	}

	temp[i] = '\0';
	bool aw = false;
	for(i=0; i<4; i++){
		if(strcmp(temp, whitelist[i])==0){
			return true;
		}
	}

	ZeroMemory(Buffer, slash);
	return false;
}


bool _stdcall blabla(HANDLE hProcess, DWORD dwFag){
	//HMODULE hWL[4] = {LoadLibraryA("wow64win.dll"), LoadLibraryA("wow64.dll"), LoadLibraryA("wow64cpu.dll"), LoadLibraryA("apisetschema.dll")};
	//DWORD dwWL_st[4];
	//DWORD dwWL_en[4];
	//MODULEINFO WLInfo[4];

	//int i;

	//if(hProcess!=GetCurrentProcess()){
	//	return false;
	//}
	//else{
	/*
		for(i=0; i<4; i++){
			if(hWL[i]){
				if(GetModuleInformation(GetCurrentProcess(), hWL[i], &WLInfo[i], sizeof(MODULEINFO))){
					dwWL_st[i] = (DWORD)WLInfo[i].lpBaseOfDll;
					dwWL_en[i] = (DWORD)WLInfo[i].lpBaseOfDll+(DWORD)WLInfo[i].SizeOfImage;
					if(dwWL_st[i] <= dwFag && dwFag <= dwWL_en[i]){
						return true;
					}
				}
			}
		}
		*/
	//}

	//if(0x00400000 <= dwFag && dwFag <= 0x02000000){
	//	return false;
	//}
	//return true;

	//return false;
	//FILE *fp;

	//fp = fopen("Addy.txt", "a");
	//fprintf(fp, "[%p]%p\n", hProcess, dwFag);
	//fclose(fp);

	return true;
}

void _declspec(naked)  DLLFucker(){
	_asm{
		add esp,0x04
		mov edx,esp
		pushad
		push [edx+0x08]
		push [edx+0x04]
		call blabla
		test eax,eax
		je Block
		popad
		ret 0x0018
Block:
		popad
		xor eax,eax
		add dword ptr [esp],0x5D
		mov word ptr [esp+0x18],0
		ret 0x0018
	}
}

void AirBypassInitialization(DWORD HS_START, DWORD HS_END){
	DWORD ReturnChecker = (DWORD)new BYTE[0x100];
	DWORD SystemCallHook = (DWORD)new BYTE[0x100];
	DWORD CurrentAddr;
	DWORD old;
	DWORD Sys64;

	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0xC0]
		mov dword ptr [Sys64],eax
	}

	VirtualProtect((DWORD *)ReturnChecker, 0x100, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)SystemCallHook, 0x100, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)Sys64, 7+5, PAGE_EXECUTE_READWRITE, &old);

	CurrentAddr = ReturnChecker;
	
	WriteWord(0xC031, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(HS_START, &CurrentAddr);
	WriteWord(0x0B72, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(HS_END, &CurrentAddr);
	WriteWord(0x0177, &CurrentAddr);
	WriteDword(0x0004C240, &CurrentAddr);

	CurrentAddr = SystemCallHook;

	///////////////
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x20, &CurrentAddr);
	WriteWord(0x5874, &CurrentAddr);
	//
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x16, &CurrentAddr);
	WriteWord(0x4374, &CurrentAddr);
	//
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x33, &CurrentAddr);
	WriteWord(0x3774, &CurrentAddr);
	//original code
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x04, &CurrentAddr);
	WriteWord(0x0974, &CurrentAddr);
	WriteByte(0x3D, &CurrentAddr);
	WriteDword(0x000000CA, &CurrentAddr);
	WriteWord(0x0D74, &CurrentAddr);
	WriteWord(0x22EB, &CurrentAddr);
	WriteWord(0x7A81, &CurrentAddr);
	WriteByte(0x14, &CurrentAddr);
	WriteDword(0x80000040, &CurrentAddr);
	WriteWord(0x1975, &CurrentAddr);
	WriteWord(0x10EB, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteDword(0x0824448B, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteCall(ReturnChecker, &CurrentAddr);
	WriteWord(0xC085, &CurrentAddr);
	WriteByte(0x58, &CurrentAddr);
	WriteWord(0x0774, &CurrentAddr);
	WriteWord(0xC031, &CurrentAddr);
	WriteDword(0x03240483, &CurrentAddr);
	WriteByte(0xC3, &CurrentAddr);
	DupeMemory(Sys64, 7, &CurrentAddr);
	//+1
	WriteByte(0x50, &CurrentAddr);
	WriteDword(0x5C24448B, &CurrentAddr);	
	WriteWord(0xE0EB, &CurrentAddr);
	//+2
	WriteDword(0x180C7A83, &CurrentAddr);
	WriteWord(0xEC75, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteByte(0x8B, &CurrentAddr);
	WriteWord(0x2484, &CurrentAddr);
	WriteDword(0x0000010C, &CurrentAddr);
	WriteWord(0xD0EB, &CurrentAddr);
	//+3
	WriteWord(0x7A81, &CurrentAddr);
	WriteByte(0x10, &CurrentAddr);
	WriteDword(0x00000210, &CurrentAddr);
	WriteWord(0xD975, &CurrentAddr);
	
	WriteByte(0x50, &CurrentAddr);
	WriteByte(0x8B, &CurrentAddr);
	WriteWord(0x2484, &CurrentAddr);
	WriteDword(0x00000268, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteCall(ReturnChecker, &CurrentAddr);
	WriteWord(0xC085, &CurrentAddr);
	WriteByte(0x58, &CurrentAddr);
	WriteWord(0xC674, &CurrentAddr);

	WriteByte(0xC7, &CurrentAddr);
	WriteWord(0x2404, &CurrentAddr);
	WriteDword((DWORD)DLLFucker, &CurrentAddr);
	WriteWord(0xBDEB, &CurrentAddr);

	//WriteDword(0x04244483, &CurrentAddr);
	//WriteByte(0x5D, &CurrentAddr);
	//WriteWord(0xB8EB, &CurrentAddr);
	//////////////////
	
	*(BYTE *)(Sys64+7) = 0xE9;
	*(DWORD *)(Sys64+7+1) = SystemCallHook-(Sys64+7)-5;
	*(WORD *)(Sys64) = 0x05EB;
}


void AirBypassInitialization2(DWORD HS_START, DWORD HS_END){
	DWORD ReturnChecker = (DWORD)new BYTE[0x100];
	DWORD SystemCallHook = (DWORD)new BYTE[0x100];
	DWORD CurrentAddr;
	DWORD old;
	DWORD Sys64;

	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0xC0]
		mov dword ptr [Sys64],eax
	}

	VirtualProtect((DWORD *)ReturnChecker, 0x100, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)SystemCallHook, 0x100, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)Sys64, 7+5, PAGE_EXECUTE_READWRITE, &old);

	CurrentAddr = ReturnChecker;

	WriteWord(0xC031, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(HS_START, &CurrentAddr);
	WriteWord(0x0B72, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(HS_END, &CurrentAddr);
	WriteWord(0x0177, &CurrentAddr);
	WriteDword(0x0004C240, &CurrentAddr);

	CurrentAddr = SystemCallHook;
	//xd
	WriteByte(0x3D, &CurrentAddr);
	WriteDword(0x000000E0, &CurrentAddr);
	WriteWord(0x2D74, &CurrentAddr);
	WriteByte(0x3D, &CurrentAddr);
	WriteDword(0x001B0006, &CurrentAddr);
	WriteWord(0x2D74, &CurrentAddr);
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x18, &CurrentAddr);
	WriteWord(0x3474, &CurrentAddr);
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x35, &CurrentAddr);
	WriteWord(0x4074, &CurrentAddr);
	WriteWord(0xF883, &CurrentAddr);
	WriteByte(0x22, &CurrentAddr);
	WriteWord(0x4974, &CurrentAddr);
	DupeMemory(Sys64, 7, &CurrentAddr);
	WriteWord(0xC031, &CurrentAddr);
	WriteByte(0xC3, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteCall(ReturnChecker, &CurrentAddr);
	WriteWord(0xC085, &CurrentAddr);
	WriteByte(0x58, &CurrentAddr);
	WriteWord(0xEB74, &CurrentAddr);
	WriteWord(0xF0EB, &CurrentAddr);

	WriteByte(0x50, &CurrentAddr);
	WriteByte(0x8B, &CurrentAddr);
	WriteWord(0x2444, &CurrentAddr);
	WriteByte(0x08, &CurrentAddr);
	WriteWord(0xECEB, &CurrentAddr);

	WriteByte(0x81, &CurrentAddr);
	WriteWord(0x247C, &CurrentAddr);
	WriteByte(0x1C, &CurrentAddr);
	WriteDword(0x80000040, &CurrentAddr);
	WriteWord(0xDF74, &CurrentAddr);
	WriteWord(0xD6EB, &CurrentAddr);

	WriteByte(0x83, &CurrentAddr);
	WriteWord(0x247C, &CurrentAddr);
	WriteByte(0x14, &CurrentAddr);
	WriteByte(0x18, &CurrentAddr);
	WriteWord(0xCF75, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteByte(0x8B, &CurrentAddr);
	WriteWord(0x2484, &CurrentAddr);
	WriteDword(0x000000E0, &CurrentAddr);
	WriteWord(0xCFEB, &CurrentAddr);
	
	WriteByte(0x83, &CurrentAddr);
	WriteWord(0x247C, &CurrentAddr);
	WriteByte(0x08, &CurrentAddr);
	WriteByte(0x05, &CurrentAddr);
	WriteWord(0xBE75, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteByte(0x8B, &CurrentAddr);
	WriteWord(0x2444, &CurrentAddr);
	WriteByte(0x5C, &CurrentAddr);
	WriteWord(0xC1EB, &CurrentAddr);

	WriteByte(0x81, &CurrentAddr);
	WriteWord(0x247C, &CurrentAddr);
	WriteByte(0x18, &CurrentAddr);
	WriteDword(0x00000210, &CurrentAddr);
	WriteWord(0xAD75, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteByte(0x8B, &CurrentAddr);
	WriteWord(0x2484, &CurrentAddr);
	WriteDword(0x00000280, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteCall(ReturnChecker, &CurrentAddr);
	WriteWord(0xC085, &CurrentAddr);
	WriteByte(0x58, &CurrentAddr);
	WriteWord(0x9A74, &CurrentAddr);
	WriteByte(0xC7, &CurrentAddr);
	WriteWord(0x2444, &CurrentAddr);
	WriteByte(0x04, &CurrentAddr);
	WriteDword(CurrentAddr+6, &CurrentAddr);
	WriteWord(0x97EB, &CurrentAddr);
	WriteByte(0xC9, &CurrentAddr);
	WriteByte(0xC2, &CurrentAddr);
	WriteWord(0x0010, &CurrentAddr);

	*(BYTE *)(Sys64+7) = 0xE9;
	*(DWORD *)(Sys64+7+1) = SystemCallHook-(Sys64+7)-5;
	*(WORD *)(Sys64) = 0x05EB;
}

void WaitForHS(int version){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;//, MSInfo;

	//Wait for HS
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}
	
	GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO));

	if(version==Windows7){
		AirBypassInitialization((DWORD)EHSvcInfo.lpBaseOfDll, (DWORD)EHSvcInfo.lpBaseOfDll+(DWORD)EHSvcInfo.SizeOfImage);
	}else{
		AirBypassInitialization2((DWORD)EHSvcInfo.lpBaseOfDll, (DWORD)EHSvcInfo.lpBaseOfDll+(DWORD)EHSvcInfo.SizeOfImage);
	}
}


int GetOSVersion(){
	OSVERSIONINFOA lpVersionInfo;
	lpVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA); 

	GetVersionExA(&lpVersionInfo);

	if(lpVersionInfo.dwMajorVersion!=6) return WindowsUnknown;

	switch(lpVersionInfo.dwMinorVersion){
		case 0:
			return WindowsUnknown;
		case 1:
			return Windows7;
		case 2:
		case 3:
			return Windows8;
	}

	return WindowsUnknown;
}

bool HackShieldBypass(){
	int YourOS = GetOSVersion();
	BOOL BitNumber;
	if(!YourOS) return false;

	IsWow64Process(GetCurrentProcess(), &BitNumber);

	if(!BitNumber) return false;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WaitForHS, (DWORD *)YourOS, NULL, NULL);

	return true;
}