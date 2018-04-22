#include"AirMemory.h"
#pragma comment(lib, "AirMemory.lib")
#include"AirWindow.h"
#pragma comment(lib, "AirWindow.lib")

AirWindow AW;
AirMemory AM;

#define EDIT_LOG 100

void AddLog(DWORD dwAddr){
	AW.AddFormatString(EDIT_LOG, "Patched @%d\r\n", dwAddr);
}

void NoEncryption(){
	DWORD SetData, GetData;

	AM.Init();

	while(SetData = AM.AobScan("56 8B F1 8B 0D ?? ?? ?? ?? 41 B8 ?? ?? ?? ?? F7 E9 03 D1 C1 FA 06 8B C2 C1 E8 1F 03 C2 6B C0 6F 89 0D ?? ?? ?? ?? 2B C8 57 75 2D")){
		AM.MemoryWriter(SetData, "8B 44 24 04 89 01 31 C0 C2 04 00");
		AddLog(SetData);
	}
	//Double
	while(SetData = AM.AobScan("56 8B F1 8B 0D ?? ?? ?? ?? 41 B8 ?? ?? ?? ?? F7 E9 03 D1 C1 FA 06 8B C2 C1 E8 1F 03 C2 6B C0 6F 89 0D ?? ?? ?? ?? 2B C8 57 75 33")){
		AM.MemoryWriter(SetData, "DD 44 24 04 DD 19 C2 08 00");
		AddLog(SetData);
	}

	while(GetData = AM.AobScan("51 53 56 57 8B F9 8B 57 08 8B 02 89 44 24 0C 8A 42 04 84 C0")){
		AM.MemoryWriter(GetData, "8B 01 C3");
		AddLog(GetData);
	}
	//Double
	while(GetData = AM.AobScan("83 EC 08 57 8B F9 8B 57 08 8A 42 08 DD 02 DD 5C 24 04 84 C0 75 02")){
		AM.MemoryWriter(GetData, "DD 01 C3");
		AddLog(GetData);
	}
}

void Create(HWND hWnd){
	AW.EditBox(EDIT_LOG, "No Encryption for JMS/EMS or others by Riremito(AIRRIDE)\r\n", 3,3, 394, 20);
	AW.ReadOnly(EDIT_LOG);
	NoEncryption();
}

void Run(HINSTANCE hInstance){
	AW.Init(hInstance, "No Encryption", 400, 300, true, Create);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){
		return FALSE;
	}

	CreateThread(NULL, 1024, (LPTHREAD_START_ROUTINE)Run, hinstDLL ,NULL, NULL);

	return TRUE;
}