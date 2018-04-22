#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include"resource.h"

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

void AirBypassInitialization(DWORD HS_START, DWORD HS_END/*, DWORD MS_START, DWORD MS_END*/){
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
	
	/*
	WriteWord(0xC031, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(MS_START, &CurrentAddr);
	WriteWord(0x1F72, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(MS_END, &CurrentAddr);
	WriteWord(0x1472, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(HS_START, &CurrentAddr);
	WriteWord(0x0B72, &CurrentAddr);
	WriteDword(0x04247C81, &CurrentAddr);
	WriteDword(HS_END, &CurrentAddr);
	WriteWord(0x0177, &CurrentAddr);
	WriteDword(0x0004C240, &CurrentAddr);
	*/
	CurrentAddr = SystemCallHook;
	
	WriteByte(0x3D, &CurrentAddr);
	WriteDword(0x000000F6, &CurrentAddr);
	WriteWord(0x840F, &CurrentAddr);
	WriteDword(0x0000008B, &CurrentAddr);
	WriteByte(0x3D, &CurrentAddr);
	WriteDword(0x0000009A, &CurrentAddr);
	WriteWord(0x840F, &CurrentAddr);
	WriteDword(0x00000087, &CurrentAddr);


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
	WriteDword(0x04244483, &CurrentAddr);
	WriteByte(0x5D, &CurrentAddr);
	WriteWord(0xB8EB, &CurrentAddr);
	//////////////////
	WriteByte(0x50, &CurrentAddr);
	WriteDword(0x4024448B, &CurrentAddr);
	WriteWord(0x10EB, &CurrentAddr);
	WriteWord(0x7A81, &CurrentAddr);
	WriteByte(0x04, &CurrentAddr);
	WriteDword(0x001F0001, &CurrentAddr);
	WriteWord(0xAF75, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteDword(0x5C24448B, &CurrentAddr);
	WriteWord(0x96EB, &CurrentAddr);
	WriteByte(0x50, &CurrentAddr);
	WriteCall(ReturnChecker, &CurrentAddr);
	WriteWord(0xC085, &CurrentAddr);
	WriteByte(0x58, &CurrentAddr);
	WriteWord(0x9D74, &CurrentAddr);
	//WriteWord(0x94EB, &CurrentAddr);
	WriteByte(0xB8, &CurrentAddr);
	WriteDword(0xC0000034, &CurrentAddr);
	WriteWord(0x91EB, &CurrentAddr);

	*(BYTE *)Sys64 = 0xE9;
	*(DWORD *)(Sys64+1) = SystemCallHook-Sys64-5;
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

	*(BYTE *)Sys64 = 0xE9;
	*(DWORD *)(Sys64+1) = SystemCallHook-Sys64-5;
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
	//GetModuleInformation(GetCurrentProcess(), NULL, &MSInfo, sizeof(MODULEINFO));

	if(version==Windows7){
		AirBypassInitialization((DWORD)EHSvcInfo.lpBaseOfDll, (DWORD)EHSvcInfo.lpBaseOfDll+(DWORD)EHSvcInfo.SizeOfImage/*, (DWORD)MSInfo.lpBaseOfDll, (DWORD)MSInfo.lpBaseOfDll+(DWORD)MSInfo.SizeOfImage*/);
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

HWND blablalaaa = NULL;
INT_PTR CALLBACK SplashProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam){
	switch(uMsg){
		case WM_INITDIALOG:
			//hwnd = blablalaaa;
			break;
		case WM_CLOSE:
			EndDialog(hwnd, true);
			break;
	}
	return true;
}

void blabla2(HINSTANCE hinstDLL){
	DialogBoxA(hinstDLL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, SplashProc);
}
void blabla(HINSTANCE hinstDLL){
	HANDLE bla = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)blabla2, hinstDLL, NULL, NULL);
	Sleep(3000);
	TerminateThread(bla, NULL);
}

void hello(HINSTANCE hinstDLL){
	int YourOS = GetOSVersion();
	BOOL BitNumber;
	if(!YourOS){
		MessageBoxA(NULL, "Sorry, Your OS is not supported", "HackShield Bypass(FAIL)", MB_OK);
		ExitProcess(NULL);
		return;
	}

	IsWow64Process(GetCurrentProcess(), &BitNumber);

	if(!BitNumber){
		MessageBoxA(NULL, "Sorry, x86 OS is not supported\nPlease Buy x64 OS", "HackShield Bypass(FAIL)", MB_OK);
		ExitProcess(NULL);
		return;
	}
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)blabla, hinstDLL, NULL, NULL);

	//MessageBoxA(NULL, "Press OK = Install HSBypass!", "HackShield Bypass(SUCCESSFUL)", MB_OK);


	WaitForHS(YourOS);
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, hinstDLL, NULL, NULL);

	return TRUE;
}