#include<Windows.h>
#include<stdio.h>
#include"ijl15.h"
#include"CRCbypass.h"
#include"lib/Asm.h"

int InjectFromList(){
	char buf[0x100];
	char message[0x100];
	char InjectList[0x100] = "DLL/";
	FILE *fp;
	int i,j;

	fp = fopen("DLL/InjectList.txt", "r");

	if(fp == NULL){
		ijl15::Error("Coudn't find DLL/InjectList.txt", NULL);
		return 0;
	}

	for(i=0; fgets(buf, 0x100, fp)!=NULL; i++){

		ZeroMemory(&InjectList[4], 0x100 - 4);

		for(j=0; buf[j]!='/'; j++){
			InjectList[j+4] = buf[j];
		}

		if(InjectList[5] == 0x00){
			continue;
		}

		if(LoadLibraryA(InjectList) == NULL){
			sprintf(message, "Couldn't Inject %s", InjectList);
			ijl15::Error(message, NULL);
		}

	}

	return i;
}


void AutoInjector(int mode){
	CRC::CRCbypass(mode);
	InjectFromList();
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){return FALSE;}

	if(ijl15::Initialization() == FALSE){ return FALSE;}

	int mode;

	mode = MessageBoxA(NULL, "YES = HSLESS\nNO = HSCRC", "Select bypass", MB_YESNO);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AutoInjector, (LPVOID)mode, NULL, NULL);

	return TRUE;
}