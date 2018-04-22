#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")
#include<stdio.h>
///////////////////////
DWORD HS_Start = NULL;
DWORD HS_End = NULL;
DWORD MS_Start = NULL;
DWORD MS_End = NULL;

///////////////////////
void ErrorMessage(char message[]){
	MessageBoxA(NULL, message, "Asm::ErrorMessage", NULL);
}

char StrArray[128] = "0123456789ABCDEF";

int ChartoByte(char Array[], BYTE b[]){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x2A){Array[i] = StrArray[rand()%0x10];}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		ErrorMessage("@ChartoByte");
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}

BOOL Enable_Write_Memory(DWORD Addr, int Size){
	DWORD old;
	if(VirtualProtect((DWORD *)Addr, Size, PAGE_EXECUTE_READWRITE, &old) == FALSE){
		ErrorMessage("Enable_Write_Memory");
		return FALSE;
	}

	return TRUE;
}

BOOL Write_code(DWORD Addr, char Array[], int nop_count){
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


DWORD _stdcall ReturnInspection(DWORD *param){
	DWORD retAddr = param[0];
	if(retAddr > HS_Start && retAddr < HS_End) return retAddr-HS_Start;
	if(retAddr > MS_Start && retAddr < MS_End) return retAddr-MS_Start;

	return 0;
}

void HSAPIHook(char *dll, char *func, char *code){
	int i;
	DWORD old;
	DWORD newmem = (DWORD)new BYTE[0x100];
	HMODULE hModule = GetModuleHandleA(dll);
	DWORD StartRoutine = (DWORD)GetProcAddress(hModule, func);
	VirtualProtect((DWORD *)(StartRoutine-5), 7, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)(newmem), 0x100, PAGE_EXECUTE_READWRITE, &old);

	*(DWORD *)(newmem) = 0x5060C48B;
	*(BYTE *)(newmem+4) = 0xE8;
	*(DWORD *)(newmem+4+1) = (DWORD)ReturnInspection-(newmem+4)-5;
	*(DWORD *)(newmem+4+1+4) = 0x7461C085;
	old = ChartoByte(code, (BYTE *)(newmem+14));
	*(BYTE *)(newmem+4+1+4+4) = old;
	*(WORD *)(newmem+4+1+4+4+1+old) = *(WORD *)(StartRoutine);
	*(BYTE *)(newmem+4+1+4+4+1+old+2) = 0xE9;
	*(DWORD *)(newmem+4+1+4+4+1+old+2+1) = (StartRoutine+2)-(newmem+4+1+4+4+1+old+2)-5;

	
	*(BYTE *)(StartRoutine-5) =0xE9;
	*(DWORD *)(StartRoutine-5+1) = newmem-(StartRoutine-5)-5;
	*(WORD *)(StartRoutine) = 0xF9EB;

}



DWORD _stdcall ReturnInspection2(DWORD *param){

		if(param[2]==0x80000040){
			SetLastError(0);
			return 1;
		}

	return 0;
}

void HSAPIHook2x(char *dll, char *func, char *code){
	int i;
	DWORD old;
	DWORD newmem = (DWORD)new BYTE[0x100];
	HMODULE hModule = GetModuleHandleA(dll);
	DWORD StartRoutine = (DWORD)GetProcAddress(hModule, func);
	VirtualProtect((DWORD *)(StartRoutine-5), 7, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((DWORD *)(newmem), 0x100, PAGE_EXECUTE_READWRITE, &old);

	*(DWORD *)(newmem) = 0x5060C48B;
	*(BYTE *)(newmem+4) = 0xE8;
	*(DWORD *)(newmem+4+1) = (DWORD)ReturnInspection2-(newmem+4)-5;
	*(DWORD *)(newmem+4+1+4) = 0x7461C085;
	old = ChartoByte(code, (BYTE *)(newmem+14));
	*(BYTE *)(newmem+4+1+4+4) = old;
	*(WORD *)(newmem+4+1+4+4+1+old) = *(WORD *)(StartRoutine);
	*(BYTE *)(newmem+4+1+4+4+1+old+2) = 0xE9;
	*(DWORD *)(newmem+4+1+4+4+1+old+2+1) = (StartRoutine+2)-(newmem+4+1+4+4+1+old+2)-5;

	
	*(BYTE *)(StartRoutine-5) =0xE9;
	*(DWORD *)(StartRoutine-5+1) = newmem-(StartRoutine-5)-5;
	*(WORD *)(StartRoutine) = 0xF9EB;

}


BOOL WaitforMS(){
	HMODULE MS = NULL;
	MODULEINFO MSInfo;

	while(!MS){
		MS = GetModuleHandleA(NULL);
		Sleep(50);
	}

	if(!GetModuleInformation(GetCurrentProcess(), MS, &MSInfo, sizeof(MODULEINFO))) return FALSE;
	
	MS_Start = (DWORD)MSInfo.lpBaseOfDll;
	MS_End = HS_Start + (DWORD)MSInfo.SizeOfImage;

	return TRUE;
}

BOOL WaitforHS(){
	HMODULE EHSvc = NULL;
	MODULEINFO EHSvcInfo;

	while(!EHSvc){
		EHSvc = GetModuleHandleA("EHSvc.dll");
		Sleep(50);
	}

	if(!GetModuleInformation(GetCurrentProcess(), EHSvc, &EHSvcInfo, sizeof(MODULEINFO))) return FALSE;
	
	HS_Start = (DWORD)EHSvcInfo.lpBaseOfDll;
	HS_End = HS_Start + (DWORD)EHSvcInfo.SizeOfImage;

	return TRUE;
}



///////////////////////

void hello(){

	HSAPIHook2x("kernel32.dll", "DeviceIoControl", "B8 01 00 00 00 C2 20 00");
	WaitforMS();
	WaitforHS();
	HSAPIHook("user32.dll", "EnumWindows", "31 C0 C2 08 00");
	HSAPIHook("kernel32.dll", "K32EnumProcesses", "31 C0 C2 0C 00");
	HSAPIHook("kernel32.dll", "K32GetMappedFileNameA", "31 C0 C2 10 00");

	HSAPIHook("kernel32.dll", "K32EnumProcessModules", "31 C0 C2 10 00");
	HSAPIHook("kernel32.dll", "K32EnumDeviceDrivers", "31 C0 C2 0C 00");
	HSAPIHook("kernel32.dll", "K32GetMappedFileNameW", "31 C0 C2 10 00");
	HSAPIHook("user32.dll", "FindWindowA", "31 C0 C2 08 00");
	HSAPIHook("user32.dll", "FindWindowExA", "31 C0 C2 10 00");
	HSAPIHook("user32.dll", "FindWindowW", "31 C0 C2 08 00");
	HSAPIHook("user32.dll", "FindWindowExW", "31 C0 C2 10 00");
	HSAPIHook("kernel32.dll", "Wow64GetThreadContext", "31 C0 C2 08 00");

	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hello, NULL, NULL, NULL);

	return TRUE;
}