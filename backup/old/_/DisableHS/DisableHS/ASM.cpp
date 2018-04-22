#include"ASM.h"

void Asm::Write_jmp(DWORD Prev, DWORD Next){
	DWORD old;
	VirtualProtect((DWORD *)Prev, 5, PAGE_READWRITE, &old);
	*(DWORD *)Prev = 0xE9;
	*(DWORD *)(Prev+1) = Next - Prev - 5;
}

void Asm::Write_call(DWORD Prev, DWORD Next){
	DWORD old;
	VirtualProtect((DWORD *)Prev, 5, PAGE_READWRITE, &old);
	*(DWORD *)Prev = 0xE8;
	*(DWORD *)(Prev+1) = Next - Prev - 5;
}

void Asm::Write_code(DWORD Addr, char Array[]){
	DWORD i,Hit;
	unsigned char ByteCode[256];

	Hit = Asm::ChartoByte(Array, ByteCode);

	VirtualProtect((DWORD *)Addr, Hit, PAGE_READWRITE, &i);
	for(i=0; i<Hit; i++){
		*(byte *)(Addr+i) = ByteCode[i];
	}
}

DWORD Asm::ChartoByte(char Array[], unsigned char *b){
	DWORD i,Hit=0;
	for(i=0; Array[i]!=0x00; i++){
		if(Array[i] >= 0x61 && Array[i] <=0x66) {Array[i]-=0x20;}//a to A
		if(Array[i] >= 0x41 && Array[i] <=0x46) {b[Hit]=Array[i]-0x37; Hit++;}
		if(Array[i] >= 0x30 && Array[i] <=0x39) {b[Hit]=Array[i]-0x30; Hit++;}
	}

	if(Hit%2!=0) return -1;

	Hit=Hit/2;

	for(i=0; i<Hit; i++){
		b[i]=b[i*2]*0x10+b[i*2+1];
	}

	return Hit;
}