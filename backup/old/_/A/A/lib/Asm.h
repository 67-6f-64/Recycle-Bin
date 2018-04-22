#include<Windows.h>
#pragma comment(lib, "lib/Asm.lib")

namespace Asm{
	DWORD Write_Hook(char code[], DWORD Prev, DWORD Next, int nop_count = 0);
	DWORD Write_Pointer_Hook(DWORD Pointer, DWORD HookAddr);
	BOOL Write_code(DWORD Addr, char Array[], int nop_count = 0);
}
