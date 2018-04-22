#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

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
	VirtualProtect((DWORD *)Sys64, 7, PAGE_EXECUTE_READWRITE, &old);

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
	WriteDword(0x04244483, &CurrentAddr);
	WriteByte(0x5D, &CurrentAddr);
	WriteWord(0xB8EB, &CurrentAddr);
	

	*(BYTE *)Sys64 = 0xE9;
	*(DWORD *)(Sys64+1) = SystemCallHook-Sys64-5;
}


void AirBypassInitialization2(DWORD HS_START, DWORD HS_END){
	DWORD ReturnChecker2 = (DWORD)new BYTE[0x100];
	DWORD SystemCallHook2 = (DWORD)new BYTE[0x100];
	DWORD CurrentAddr2;
	DWORD old2;
	DWORD Sys642;

	_asm{
		mov eax,fs:[0x18]
		mov eax,[eax+0xC0]
		mov dword ptr [Sys642],eax
	}

	VirtualProtect((DWORD *)ReturnChecker2, 0x100, PAGE_EXECUTE_READWRITE, &old2);
	VirtualProtect((DWORD *)SystemCallHook2, 0x100, PAGE_EXECUTE_READWRITE, &old2);
	VirtualProtect((DWORD *)Sys642, 7, PAGE_EXECUTE_READWRITE, &old2);

	CurrentAddr2 = ReturnChecker2;

	WriteWord(0xC031, &CurrentAddr2);
	WriteDword(0x04247C81, &CurrentAddr2);
	WriteDword(HS_START, &CurrentAddr2);
	WriteWord(0x0B72, &CurrentAddr2);
	WriteDword(0x04247C81, &CurrentAddr2);
	WriteDword(HS_END, &CurrentAddr2);
	WriteWord(0x0177, &CurrentAddr2);
	WriteDword(0x0004C240, &CurrentAddr2);

	CurrentAddr2 = SystemCallHook2;
	//xd
	WriteByte(0x3D, &CurrentAddr2);
	WriteDword(0x000000E0, &CurrentAddr2);
	WriteWord(0x2D74, &CurrentAddr2);
	WriteByte(0x3D, &CurrentAddr2);
	WriteDword(0x001B0006, &CurrentAddr2);
	WriteWord(0x2D74, &CurrentAddr2);
	WriteWord(0xF883, &CurrentAddr2);
	WriteByte(0x18, &CurrentAddr2);
	WriteWord(0x3474, &CurrentAddr2);
	WriteWord(0xF883, &CurrentAddr2);
	WriteByte(0x35, &CurrentAddr2);
	WriteWord(0x4074, &CurrentAddr2);
	WriteWord(0xF883, &CurrentAddr2);
	WriteByte(0x22, &CurrentAddr2);
	WriteWord(0x4974, &CurrentAddr2);
	DupeMemory(Sys642, 7, &CurrentAddr2);
	WriteWord(0xC031, &CurrentAddr2);
	WriteByte(0xC3, &CurrentAddr2);
	WriteByte(0x50, &CurrentAddr2);
	WriteCall(ReturnChecker2, &CurrentAddr2);
	WriteWord(0xC085, &CurrentAddr2);
	WriteByte(0x58, &CurrentAddr2);
	WriteWord(0xEB74, &CurrentAddr2);
	WriteWord(0xF0EB, &CurrentAddr2);

	WriteByte(0x50, &CurrentAddr2);
	WriteByte(0x8B, &CurrentAddr2);
	WriteWord(0x2444, &CurrentAddr2);
	WriteByte(0x08, &CurrentAddr2);
	WriteWord(0xECEB, &CurrentAddr2);

	WriteByte(0x81, &CurrentAddr2);
	WriteWord(0x247C, &CurrentAddr2);
	WriteByte(0x1C, &CurrentAddr2);
	WriteDword(0x80000040, &CurrentAddr2);
	WriteWord(0xDF74, &CurrentAddr2);
	WriteWord(0xD6EB, &CurrentAddr2);

	WriteByte(0x83, &CurrentAddr2);
	WriteWord(0x247C, &CurrentAddr2);
	WriteByte(0x14, &CurrentAddr2);
	WriteByte(0x18, &CurrentAddr2);
	WriteWord(0xCF75, &CurrentAddr2);
	WriteByte(0x50, &CurrentAddr2);
	WriteByte(0x8B, &CurrentAddr2);
	WriteWord(0x2484, &CurrentAddr2);
	WriteDword(0x000000E0, &CurrentAddr2);
	WriteWord(0xCFEB, &CurrentAddr2);
	
	WriteByte(0x83, &CurrentAddr2);
	WriteWord(0x247C, &CurrentAddr2);
	WriteByte(0x08, &CurrentAddr2);
	WriteByte(0x05, &CurrentAddr2);
	WriteWord(0xBE75, &CurrentAddr2);
	WriteByte(0x50, &CurrentAddr2);
	WriteByte(0x8B, &CurrentAddr2);
	WriteWord(0x2444, &CurrentAddr2);
	WriteByte(0x5C, &CurrentAddr2);
	WriteWord(0xC1EB, &CurrentAddr2);

	WriteByte(0x81, &CurrentAddr2);
	WriteWord(0x247C, &CurrentAddr2);
	WriteByte(0x18, &CurrentAddr2);
	WriteDword(0x00000210, &CurrentAddr2);
	WriteWord(0xAD75, &CurrentAddr2);
	WriteByte(0x50, &CurrentAddr2);
	WriteByte(0x8B, &CurrentAddr2);
	WriteWord(0x2484, &CurrentAddr2);
	WriteDword(0x00000280, &CurrentAddr2);
	WriteByte(0x50, &CurrentAddr2);
	WriteCall(ReturnChecker2, &CurrentAddr2);
	WriteWord(0xC085, &CurrentAddr2);
	WriteByte(0x58, &CurrentAddr2);
	WriteWord(0x9A74, &CurrentAddr2);
	WriteByte(0xC7, &CurrentAddr2);
	WriteWord(0x2444, &CurrentAddr2);
	WriteByte(0x04, &CurrentAddr2);
	WriteDword(CurrentAddr2+6, &CurrentAddr2);
	WriteWord(0x97EB, &CurrentAddr2);
	WriteByte(0xC9, &CurrentAddr2);
	WriteByte(0xC2, &CurrentAddr2);
	WriteWord(0x0010, &CurrentAddr2);

	*(BYTE *)Sys642 = 0xE9;
	*(DWORD *)(Sys642+1) = SystemCallHook2-Sys642-5;
}

#define Windows7 7
#define Windows8 8

void WaitForHS(int version){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;

	//Wait for HS
	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}

	GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO));
	//2
	switch(version){
		case Windows7:
			AirBypassInitialization((DWORD)EHSvcInfo.lpBaseOfDll, (DWORD)EHSvcInfo.lpBaseOfDll+(DWORD)EHSvcInfo.SizeOfImage);
			break;
		case Windows8:
			AirBypassInitialization2((DWORD)EHSvcInfo.lpBaseOfDll, (DWORD)EHSvcInfo.lpBaseOfDll+(DWORD)EHSvcInfo.SizeOfImage);
			break;
		default:
			break;
	}
}


int CheckOS(){
	OSVERSIONINFOA lpVersionInfo;
	lpVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA); 

	GetVersionExA(&lpVersionInfo);
	
	switch(lpVersionInfo.dwMajorVersion){
		case 6:
			if(lpVersionInfo.dwMinorVersion){
				return Windows7;
			}
			break;
		case 8:
			if(lpVersionInfo.dwMinorVersion){
				return Windows8;
			}
			break;
		default:
			break;
	}
	return 0;
}


void hello(){
	int version = CheckOS();
	BOOL checkbit;
	if(version > 0){
		IsWow64Process(GetCurrentProcess(), &checkbit);
			if(checkbit){
				MessageBoxA(NULL, "Press OK = Install HSBypass!\n\nCreated by AIRRIDE", "Public HackShield Bypass", MB_OK);
				WaitForHS(version);
				return;
			}
	}
	MessageBoxA(NULL, "Your OS is not supported Sorry!\nPlease Buy Windows7 or 8 x64\n\nCreated by AIRRIDE", "Public HackShield Bypass", MB_OK);
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, NULL, NULL, NULL);

	return TRUE;
}