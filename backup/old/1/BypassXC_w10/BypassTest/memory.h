#ifndef __MEMORY_H__
#define __MEMORY_H__

#define MAX_RESULT 256

class memory{
private:
	DWORD BaseAddress;
	DWORD ImageSize;
	DWORD AobResult[MAX_RESULT];
	DWORD CountResult;

	void Error(char *message);
	void FullAccess(DWORD Address, DWORD Size);

	bool HotPatchCheck(DWORD Address);
	void backjmp(DWORD Address);
	DWORD createfunction(DWORD Address, DWORD Size);

public:
	memory();
	memory(char *dll);
	void write(DWORD Address, char *code);
	void scan(char *Aob, char *code = NULL);
	DWORD get(DWORD Result = 0);
	DWORD count();
	void Hook(DWORD Address, void *function_hook, void *_function, DWORD Size = 0);
	void jmp(DWORD Prev, DWORD Next);

};

#endif