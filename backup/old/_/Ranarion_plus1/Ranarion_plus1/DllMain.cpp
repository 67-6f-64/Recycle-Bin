#include<Windows.h>
#include<psapi.h>
#pragma comment(lib, "psapi.lib")

DWORD dwFaggot_Start[4];
DWORD dwFaggot_End[4];
DWORD dwReturn;

DWORD _declspec(naked) _GetMappedFileNameA(HANDLE hProcess, LPVOID lpv, LPTSTR lpFilename, DWORD nSize){
	_asm{
		mov edi,edi
		push ebp
		mov ebp,esp
		jmp dwReturn
	}
}

DWORD dwBlala = 0;
DWORD _stdcall FileInspect(HANDLE hProcess, LPVOID lpv, LPTSTR lpFilename, DWORD nSize){
	//int i;
	//for(i=0; i<4; i++){
	//	if(dwFaggot_Start[i] <= (DWORD)lpv && (DWORD)lpv <= dwFaggot_End[i] ){
	//		return _GetMappedFileNameA(hProcess, lpv, lpFilename, nSize); 

	//	}
	//}

	if(hProcess != GetCurrentProcess()){

	//if((DWORD)lpv >= 0x00400000 && (DWORD)lpv <= 0x02000000){
		dwBlala = 0;
		return 0;
	//}

	}

	dwBlala =  _GetMappedFileNameA(hProcess, lpv, lpFilename, nSize);



	return 0;
}

void _declspec(naked) FaggotHook(){
	_asm{
		mov eax,esp
		pushad
		push [eax+0x10]
		push [eax+0x0C]
		push [eax+0x08]
		push [eax+0x04]
		call FileInspect
		popad
		mov eax,[dwBlala]
		ret 0x0010
	}
}

void SpoofScan(){
	//HMODULE hFaggot[4] = {LoadLibraryA("wow64win.dll"), LoadLibraryA("wow64.dll"), LoadLibraryA("wow64cpu.dll"), LoadLibraryA("apisetschema.dll")};
	//LPSTR Target[4] = {"wow64win.dll", "wow64.dll", "wow64cpu.dll", "apisetschema.dll"};
	//MODULEINFO FaggotInfo[4] = {0};
	int i;

	/*
	for(i=0; i<4; i++){
		while(!hFaggot[i]){
			hFaggot[i] = GetModuleHandleA(Target[i]);
			Sleep(50);
		}
		MessageBoxA(NULL, "test", "Ranarion_Plus1", NULL);
	}
	*/

	//for(i=0; i<4; i++){
	//	GetModuleInformation(GetCurrentProcess(), hFaggot[i], &FaggotInfo[i], sizeof(MODULEINFO));
	//	dwFaggot_Start[i] = (DWORD)FaggotInfo[i].lpBaseOfDll,
	//	dwFaggot_End[i] = (DWORD)FaggotInfo[i].lpBaseOfDll+(DWORD)FaggotInfo[i].SizeOfImage;
	//}

	DWORD dwAddr = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll"), "K32GetMappedFileNameA");
	DWORD old;
	VirtualProtect((DWORD *) dwAddr, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE *)(dwAddr) = 0xE9;
	*(DWORD *)(dwAddr+1) = (DWORD)FaggotHook-dwAddr-5;
	dwReturn = dwAddr+5;


	//MessageBoxA(NULL, "new HSCRC bypass is\nSuccessfully installed!", "Ranarion_Plus1", NULL);

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH) return FALSE;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SpoofScan, hinstDLL, NULL, NULL);
	return TRUE;
}