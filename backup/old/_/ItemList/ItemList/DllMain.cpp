#include<Windows.h>
#include<stdio.h>

void GetAllItemList(){
	int i;
	DWORD GetAddr;
	FILE *fp;

	fp = fopen("ItemList.txt", "a");

	for(i=0; i<9999999; i++){

		GetAddr = 0;

		_asm{
			mov ebx,0x006D0550
			lea eax,[GetAddr]
			push [i]
			push eax
			mov ecx,0x014716A0
			mov ecx,[ecx]
			call ebx
		}

		if(*(BYTE *)GetAddr != 0x00){
			fprintf(fp, "%d:%s\n", i, (char *)GetAddr);
		}

	}

	fclose(fp);
	MessageBoxA(NULL, "END", "ItemList", NULL);

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){

	if(fdwReason != DLL_PROCESS_ATTACH){return FALSE;}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GetAllItemList, NULL, NULL, NULL);
	return TRUE;
}