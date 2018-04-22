#ifndef __APIHOOK_H__
#define __APIHOOK_H__

class apihook{
private:
	HMODULE hModule;

	void Error(char *message);
	void FullAccess(DWORD Address, DWORD Size);
	bool HotPatchCheck(DWORD Address);
	void backjmp(DWORD Address);
	DWORD createapi(DWORD Address, DWORD Size);
	void jmp(DWORD Prev, DWORD Next);

public:
	apihook(char *dll);
	void Hook(char *api, void *api_hook, void *_api);
};

#endif