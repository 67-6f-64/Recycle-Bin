#include<Windows.h>

namespace Asm{
	void Write_jmp(DWORD Prev, DWORD Next);
	void Write_call(DWORD Prev, DWORD Next);
	void Write_code(DWORD Addr, char Array[]);
	DWORD ChartoByte(char Array[], unsigned char *b);
}