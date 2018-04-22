#include"HeartBeat.h"

DWORD (*DecryptInput)(BYTE *Input, DWORD Length);
HMODULE EHSvc;

void WritePointer(void *Pointer, DWORD Offset){
	*(DWORD *)Pointer = (DWORD)EHSvc + Offset;
}

void FunctionImport(){
	EHSvc = LoadLibraryA("HShield/EHSvc.dll");
	WritePointer(&DecryptInput, 0xD24A8);
}

void HeartBeat(BYTE *Input, DWORD Length, BYTE *Output){
	int i;
	DWORD err;
	printf("HeartBeat...\n");
	err = DecryptInput(Input, Length);
	printf("ERROR:%08X", err);

	for(i=0; i<Length; i++){
		printf("%02X", Input[i]);
	}

	printf("\n", Length);
}