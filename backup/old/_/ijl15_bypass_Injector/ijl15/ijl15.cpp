#include"ijl15.h"

namespace ijl15{
	DWORD dwInit;
	DWORD dwFree;
	DWORD dwRead;
	DWORD dwWrite;
	DWORD dwGetLibVersion;
	DWORD dwErrorStr;
}

void _declspec(naked) ijl15_Init(){_asm jmp dword ptr [ijl15::dwInit]}
void _declspec(naked) ijl15_Free(){_asm jmp dword ptr [ijl15::dwFree]}
void _declspec(naked) ijl15_Read(){_asm jmp dword ptr [ijl15::dwRead]}
void _declspec(naked) ijl15_Write(){_asm jmp dword ptr [ijl15::dwWrite]}
void _declspec(naked) ijl15_GetLibVersion(){_asm jmp dword ptr [ijl15::dwGetLibVersion]}
void _declspec(naked) ijl15_ErrorStr(){_asm jmp dword ptr [ijl15::dwErrorStr]}

BOOL ijl15::Initialization(){
	HMODULE ijl15;

	ijl15 = LoadLibraryA("DLL/ijl15.dll");
	
	if(ijl15 == 0){
		Error("Couldn't load dll/ijl15.dll, Thank you!", TRUE);
		return FALSE;
	}
	
	dwInit = (DWORD)GetProcAddress(ijl15, "ijlInit");
	dwFree = (DWORD)GetProcAddress(ijl15, "ijlFree");
	dwRead = (DWORD)GetProcAddress(ijl15, "ijlRead");
	dwWrite = (DWORD)GetProcAddress(ijl15, "ijlWrite");
	dwGetLibVersion = (DWORD)GetProcAddress(ijl15, "ijlGetLibVersion");
	dwErrorStr = (DWORD)GetProcAddress(ijl15, "ijlErrorStr");


	if(dwInit == NULL || dwFree == NULL || dwRead == NULL || dwWrite == NULL || dwGetLibVersion == NULL || dwErrorStr == NULL){
		Error("Couldn't find ijl15.dll's function, Thank you!", TRUE);
		return FALSE;
	}

	return TRUE;
}


void ijl15::Error(char message[], BOOL kill){
	MessageBoxA(NULL, message, "ijl15.dll", MB_OK);
	if(kill == TRUE){
		ExitProcess(0);
	}
}