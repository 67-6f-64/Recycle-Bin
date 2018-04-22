#include<Windows.h>
#include<stdio.h>
//#include"HeartBeat.h"

DWORD (*_HeartBeat)(BYTE *Input, DWORD Length, BYTE *Output);
DWORD (_stdcall *_ServiceDispatch)(DWORD Service, void **Param, DWORD *ErrorCode);
DWORD _stdcall ServiceDispatch(DWORD Service, void **Param, DWORD *ErrorCode){
	printf("Service:%d\n", Service);
	switch(Service){
	case 4:
		return _ServiceDispatch(Service, Param, ErrorCode);//Initialize HShield/EHSvc.dll
	case 13:
		_HeartBeat((BYTE *)Param[0], (DWORD)Param[1], (BYTE *)Param[2]);
		break;
	default:
		break;
	}
	*ErrorCode = 0;
	return 1;
}

HMODULE EHSvc;
void WritePointer(void *Pointer, DWORD Offset){
	*(DWORD *)Pointer = (DWORD)EHSvc + Offset;
}

void FunctionImport(){
	EHSvc = LoadLibraryA("HShield/EHSvc.dll");
	WritePointer(&_HeartBeat, 0x97F45);
	WritePointer(&_ServiceDispatch, 0x252E0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		FunctionImport();
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		FunctionImport();
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();
		break;
	default:
		break;
	}
	return TRUE;
}