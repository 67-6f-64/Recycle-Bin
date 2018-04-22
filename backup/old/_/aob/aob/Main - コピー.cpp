#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
	HWND hMaple;
	DWORD dwMaple;
	HANDLE hProcess;

	hMaple = FindWindowA(NULL, "MapleStory");
	GetWindowThreadProcessId(hMaple, &dwMaple);
	printf("%d\n", hMaple);
	printf("%d\n", dwMaple);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwMaple);
	printf("%d\n", hProcess);

	DWORD Addr_Start = 0x00401000;
	DWORD Addr_End = 0x02000000;
	DWORD i;
	unsigned char Aob[5]={0x8B, 0x4D, 0x18, 0x8B, 0x09};
	unsigned char rm[128];
	DWORD dw;
	BYTE wa;
	for(i = Addr_Start; i < Addr_End; i++){
		ReadProcessMemory(hProcess, (void *)i, &dw, 5, NULL);
		if(dw == 0x8B184D8B){
			ReadProcessMemory(hProcess, (void *)(i + 4), &wa, 1, NULL);
			if(wa==0x09){
				printf("%08X\n", i);
				break;
			}
		}
	}
	printf("End\n");
	//ReadProcessMemory(hProcess, (void *)0x00401000, &dw, 4, NULL);
	//printf("%08X\n", dw);

	system("pause");
}