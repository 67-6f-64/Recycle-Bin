#include"Asm2.h"

bool Asm2::ChangeMemoryProtect(DWORD Addr, DWORD dwSize){
	DWORD old;
	if(VirtualProtect((DWORD *)Addr, dwSize, PAGE_EXECUTE_READWRITE, &old)){
		return true;
	}else{
		return false;
	}
}

DWORD Asm2::AllocMemory(DWORD dwSize){
	DWORD Addr = (DWORD)new BYTE[dwSize];
	if(ChangeMemoryProtect(Addr, dwSize)){
		return Addr;
	}else{
		DeAllocMemory(Addr, dwSize);
		return 0;
	}
}

void Asm2::DeAllocMemory(DWORD Addr, DWORD dwSize){
	delete [] (DWORD *)Addr;
}

void Asm2::WriteBYTE(PDWORD Addr, BYTE Code, bool Flag){
	*(BYTE *)Addr[0] = Code;
	if(!Flag) return;
	Addr[0]+=1;
}

void Asm2::WriteWORD(PDWORD Addr, WORD Code, bool Flag){
	*(WORD *)Addr[0] = Code;
	if(!Flag) return;
	Addr[0]+=2;
}

void Asm2::WriteDWORD(PDWORD Addr, DWORD Code, bool Flag){
	*(DWORD *)Addr[0] = Code;
	if(!Flag) return;
	Addr[0]+=4;
}

DWORD Asm2::MemorySubtraction(DWORD Prev, DWORD Next, bool Flag){
	return Prev-(Next-1)-5-Flag;
}

void Asm2::WriteCALL(PDWORD Addr, DWORD Next, bool Flag){
	WriteBYTE(Addr, 0xE8, Flag);
	WriteDWORD(Addr, MemorySubtraction(Addr[0], Next), Flag);
}

void Asm2::WriteJMP(PDWORD Addr, DWORD Next, bool Flag){
	WriteBYTE(Addr, 0xE9, Flag);
	WriteDWORD(Addr, MemorySubtraction(Addr[0], Next), Flag);
}


void Asm2::WriteCOPY(DWORD src, DWORD dwSize, PDWORD Addr, bool Flag){
	memcpy((DWORD *)Addr[0], (DWORD *)src, dwSize);
	if(!Flag) return;
	Addr[0]+=dwSize;
}
