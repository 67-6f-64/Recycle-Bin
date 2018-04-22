#include"Function.h"


void FullAccess(DWORD dwAddr, DWORD dwSize){
	DWORD old;
	VirtualProtect((DWORD *)dwAddr, dwSize, PAGE_EXECUTE_READWRITE, &old);
}

void WriteJMP(DWORD dwPrev, DWORD dwNext){
	FullAccess(dwPrev, 5);
	*(BYTE *)(dwPrev) = 0xE9;
	*(DWORD *)(dwPrev+1) = (DWORD)dwNext-dwPrev-5;
}

char StrArray[0x20] = "0123456789ABCDEF";

int ChartoByte(char Array[], BYTE b[]){
	int i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] == 0x2A){Array[i] = StrArray[rand()%0x10];}
		if(Array[i] >= 0x61 && Array[i] <=0x66) {b[Hit]=Array[i]-0x57; Hit++;}
		else if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		else if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0){
		return -1;
	}

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}

bool WriteCODE(DWORD dwAddr, char Array[], int nop_count){
	int i, Hit;
	BYTE ByteCode[0x100];//

	if(Array != NULL && Array[0] != 0x00){

		Hit = ChartoByte(Array, ByteCode);
		FullAccess(dwAddr, Hit + nop_count);

		for(i=0; i<Hit; i++){
			*(BYTE *)(dwAddr + i) = ByteCode[i];
		}

		if(nop_count == 0){
				return true;
		}

	}else{
		Hit = 0;
	}

	for(i=0; i<nop_count; i++){
		*(BYTE *)(dwAddr + Hit + i) = 0x90;//nop
	}

	return true;
}