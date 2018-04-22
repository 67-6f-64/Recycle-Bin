#pragma once
#include<Windows.h>

namespace Asm2{
	bool ChangeMemoryProtect(DWORD Addr, DWORD dwSize);
	DWORD AllocMemory(DWORD dwSize);
	void DeAllocMemory(DWORD Addr, DWORD dwSize);
	void WriteBYTE(PDWORD Addr, BYTE Code, bool Flag = true);
	void WriteWORD(PDWORD Addr, WORD Code, bool Flag = true);
	void WriteDWORD(PDWORD Addr, DWORD Code, bool Flag = true);
	DWORD MemorySubtraction(DWORD Prev, DWORD Next, bool Flag = false);
	void WriteCALL(PDWORD Addr, DWORD Next, bool Flag = true);
	void WriteJMP(PDWORD Addr, DWORD Next, bool Flag = true);
	void WriteCOPY(DWORD src, DWORD dwSize, PDWORD Addr, bool Flag = true);
}